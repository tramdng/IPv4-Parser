#include <iostream>
#include <string>
using namespace std;


// Check if a character can be part of an IPv4 candidate
bool isTokenChar(char c)
{
    return (c >= '0' && c <= '9') || c == '.' || c == ':';
}


// Convert digits to a number manually.
// No stoi(), atoi(), sscanf(), etc.
bool readNumber(const string& token, int& pos, int maxDigits, int maxValue, int& value)
{
    int start = pos;
    value = 0;

    // Must start with a digit
    if (pos >= (int)token.length() ||
        token[pos] < '0' || token[pos] > '9')
    {
        return false;
    }

    // Leading zero is only allowed when the number is exactly 0
    if (token[pos] == '0')
    {
        pos++;

        if (pos < (int)token.length() &&
            token[pos] >= '0' && token[pos] <= '9')
        {
            return false;
        }

        value = 0;
        return true;
    }

    // Read digits one at a time
    while (pos < (int)token.length() &&
           token[pos] >= '0' && token[pos] <= '9')
    {
        // Too many digits
        if (pos - start >= maxDigits)
        {
            return false;
        }

        value = value * 10 + (token[pos] - '0');

        // Number is too large
        if (value > maxValue)
        {
            return false;
        }

        pos++;
    }

    return true;
}


// Validate one complete candidate token
bool parseCandidate(const string& token,
                    unsigned long& address,
                    int& port)
{
    int pos = 0;
    int octets[4];

    // Read four octets
    for (int i = 0; i < 4; i++)
    {
        if (!readNumber(token, pos, 3, 255, octets[i]))
        {
            return false;
        }

        // First three octets must be followed by '.'
        if (i < 3)
        {
            if (pos >= (int)token.length() || token[pos] != '.')
            {
                return false;
            }

            pos++;
        }
    }

    port = -1;

    // Check for optional port
    if (pos < (int)token.length())
    {
        if (token[pos] != ':')
        {
            return false;
        }

        pos++;

        int portValue;

        if (!readNumber(token, pos, 5, 65535, portValue))
        {
            return false;
        }

        port = portValue;
    }

    // Nothing extra can remain in the candidate
    if (pos != (int)token.length())
    {
        return false;
    }

    // Build the 32-bit IPv4 value
    address =
        ((unsigned long)octets[0] << 24) |
        ((unsigned long)octets[1] << 16) |
        ((unsigned long)octets[2] << 8) |
        (unsigned long)octets[3];

    return true;
}


// Required extraction function
bool extractIPv4(const string& str,
                 unsigned long& outAddress,
                 int& outPort)
{
    outAddress = 0;
    outPort = -1;

    int i = 0;

    while (i < (int)str.length())
    {
        // Skip garbage characters
        if (!isTokenChar(str[i]))
        {
            i++;
            continue;
        }

        int start = i;

        // Find the complete run containing only digits, '.' and ':'
        while (i < (int)str.length() && isTokenChar(str[i]))
        {
            i++;
        }

        string candidate = str.substr(start, i - start);

        unsigned long address;
        int port;

        if (parseCandidate(candidate, address, port))
        {
            outAddress = address;
            outPort = port;
            return true;
        }
    }

    return false;
}


// Print the IPv4 address from its 32-bit value
void printAddress(unsigned long address)
{
    unsigned long a = (address >> 24) & 255;
    unsigned long b = (address >> 16) & 255;
    unsigned long c = (address >> 8) & 255;
    unsigned long d = address & 255;

    cout << a << "." << b << "." << c << "." << d;
}


int main()
{
    string input;

    while (true)
    {
        cout << "Enter a string (or 'END' to quit): ";
        getline(cin, input);

        if (input == "END")
        {
            cout << "Program terminated." << endl;
            break;
        }

        unsigned long address = 0;
        int port = -1;

        if (extractIPv4(input, address, port))
        {
            cout << "Extracted IPv4 address: ";
            printAddress(address);

            cout << " (decimal value: " << address << ", port: ";

            if (port == -1)
            {
                cout << "none";
            }
            else
            {
                cout << port;
            }

            cout << ")" << endl;
        }
        else
        {
            cout << "Invalid input: no valid IPv4 address found" << endl;
        }
    }

    return 0;
}