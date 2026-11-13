# Contact Management System in C

A simple, menu-driven **Contact Management System** built using the C programming language and file handling.

## Features

- Add new contacts
- Display all saved contacts
- Search contacts by ID, name, or phone number
- Update existing contact details
- Delete contacts with confirmation
- Assign an automatic ID to each new contact
- Prevent duplicate phone numbers
- Save records permanently in a binary file (`contacts.dat`)

## Contact information stored

Each record contains:
- Contact ID
- Name
- Phone number
- Email address
- Residential address

## Technologies and concepts

- **Language:** C
- **File handling:** `fopen()`, `fread()`, `fwrite()`, `fclose()`, `remove()`, `rename()`
- **Other concepts:** Structures, functions, loops, conditional statements, strings

## Requirements

- GCC compiler (or another C compiler)
- Windows, Linux, or macOS

## How to compile and run

### Linux / macOS

```bash
gcc contact_manager.c -o contact_manager
./contact_manager
```

### Windows (MinGW)

```bash
gcc contact_manager.c -o contact_manager.exe
contact_manager.exe
```

## How to use

1. Run the program.
2. Select an option from the menu.
3. Enter the requested contact details.
4. Use the search, update, and delete options to manage saved records.
5. Select `0` to exit.

The program creates `contacts.dat` in the current working directory when the first contact is saved. Keep this file to retain your contacts between runs.

## Project structure

```text
Contact_Management_System/
├── contact_manager.c
├── README.md
├── .gitignore
└── LICENSE
```

## Learning purpose

This project demonstrates how structures and file handling can be used to build a basic record-management application in C. It is intended for beginners and academic practice.

## Author

Add your name here.
