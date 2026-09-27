# IPv4 Address Parser

## Description

This project is a C++ program that finds and validates an IPv4 address from a line of text. The IPv4 address can also include an optional port number.

## Features

- IPv4 address validation
- Optional port validation
- Manual number conversion
- Detects invalid or incomplete addresses
- Supports IPv4 addresses inside other text
- Converts the IPv4 address to a 32-bit decimal value
- Continuous input until the user enters `END`


## How to Run

From the project folder, compile the program:

    g++ main.cpp -o ipv4

Then run:

    ./ipv4

The program will continue asking for input until you enter `END`.