/*
 * CONTACT MANAGEMENT SYSTEM
 * Language: C
 * Concepts: Structures, functions, file handling, CRUD operations
 *
 * Data is stored in "contacts.dat" in the same folder as the program.
 * Compile: gcc contact_manager.c -o contact_manager
 * Run:     ./contact_manager       (Linux/macOS)
 *          contact_manager.exe    (Windows)
 */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>

#define DATA_FILE "contacts.dat"
#define TEMP_FILE "contacts.tmp"
#define NAME_LEN  60
#define PHONE_LEN 20
#define EMAIL_LEN 80
#define ADDRESS_LEN 120

typedef struct {
    int id;
    char name[NAME_LEN];
    char phone[PHONE_LEN];
    char email[EMAIL_LEN];
    char address[ADDRESS_LEN];
} Contact;

void readLine(const char *prompt, char *buffer, size_t size) {
    printf("%s", prompt);
    if (fgets(buffer, (int)size, stdin) == NULL) {
        buffer[0] = '\0';
        return;
    }
    buffer[strcspn(buffer, "\n")] = '\0';
}

int readInt(const char *prompt) {
    char input[40], *end;
    long value;
    while (1) {
        readLine(prompt, input, sizeof(input));
        value = strtol(input, &end, 10);
        while (isspace((unsigned char)*end)) end++;
        if (input[0] != '\0' && *end == '\0' && value >= 0 && value <= 2147483647L)
            return (int)value;
        printf("Please enter a valid positive number.\n");
    }
}

void displayContact(const Contact *c) {
    printf("\n----------------------------------------\n");
    printf("Contact ID : %d\n", c->id);
    printf("Name       : %s\n", c->name);
    printf("Phone      : %s\n", c->phone);
    printf("Email      : %s\n", c->email);
    printf("Address    : %s\n", c->address);
    printf("----------------------------------------\n");
}

int getNextId(void) {
    FILE *fp = fopen(DATA_FILE, "rb");
    Contact c;
    int maxId = 0;

    if (fp == NULL) return 1;
    while (fread(&c, sizeof(Contact), 1, fp) == 1) {
        if (c.id > maxId) maxId = c.id;
    }
    fclose(fp);
    return maxId + 1;
}

int phoneExists(const char *phone, int ignoredId) {
    FILE *fp = fopen(DATA_FILE, "rb");
    Contact c;

    if (fp == NULL) return 0;
    while (fread(&c, sizeof(Contact), 1, fp) == 1) {
        if (strcmp(c.phone, phone) == 0 && c.id != ignoredId) {
            fclose(fp);
            return 1;
        }
    }
    fclose(fp);
    return 0;
}

void addContact(void) {
    FILE *fp;
    Contact c = {0};

    c.id = getNextId();
    printf("\n--- Add New Contact ---\n");
    readLine("Enter name: ", c.name, sizeof(c.name));
    readLine("Enter phone number: ", c.phone, sizeof(c.phone));
    readLine("Enter email: ", c.email, sizeof(c.email));
    readLine("Enter address: ", c.address, sizeof(c.address));

    if (c.name[0] == '\0' || c.phone[0] == '\0') {
        printf("Name and phone number are required. Contact not saved.\n");
        return;
    }
    if (phoneExists(c.phone, -1)) {
        printf("This phone number already exists. Contact not saved.\n");
        return;
    }

    fp = fopen(DATA_FILE, "ab");
    if (fp == NULL) {
        perror("Could not open contact file");
        return;
    }
    if (fwrite(&c, sizeof(Contact), 1, fp) != 1) {
        printf("Error while saving contact.\n");
    } else {
        printf("Contact saved successfully. Your contact ID is %d.\n", c.id);
    }
    fclose(fp);
}

void viewAllContacts(void) {
    FILE *fp = fopen(DATA_FILE, "rb");
    Contact c;
    int count = 0;

    printf("\n--- All Contacts ---\n");
    if (fp == NULL) {
        printf("No contacts found yet.\n");
        return;
    }
    while (fread(&c, sizeof(Contact), 1, fp) == 1) {
        displayContact(&c);
        count++;
    }
    fclose(fp);
    if (count == 0) printf("No contacts found.\n");
    else printf("Total contacts: %d\n", count);
}

void searchContact(void) {
    FILE *fp = fopen(DATA_FILE, "rb");
    Contact c;
    char query[NAME_LEN];
    int found = 0;

    printf("\n--- Search Contact ---\n");
    if (fp == NULL) {
        printf("No contacts found yet.\n");
        return;
    }
    readLine("Enter contact ID, name, or phone number: ", query, sizeof(query));

    while (fread(&c, sizeof(Contact), 1, fp) == 1) {
        char idText[20];
        snprintf(idText, sizeof(idText), "%d", c.id);
        if (strcmp(idText, query) == 0 ||
            strstr(c.name, query) != NULL ||
            strstr(c.phone, query) != NULL) {
            displayContact(&c);
            found = 1;
        }
    }
    fclose(fp);
    if (!found) printf("No matching contact found.\n");
}

void updateContact(void) {
    FILE *fp = fopen(DATA_FILE, "rb");
    FILE *temp;
    Contact c;
    int id, found = 0;

    printf("\n--- Update Contact ---\n");
    if (fp == NULL) {
        printf("No contacts found yet.\n");
        return;
    }
    id = readInt("Enter the contact ID to update: ");
    temp = fopen(TEMP_FILE, "wb");
    if (temp == NULL) {
        perror("Could not create temporary file");
        fclose(fp);
        return;
    }

    while (fread(&c, sizeof(Contact), 1, fp) == 1) {
        if (c.id == id) {
            char newPhone[PHONE_LEN];
            found = 1;
            displayContact(&c);
            printf("Enter new details (press Enter to keep the existing value).\n");

            char value[ADDRESS_LEN];
            readLine("New name: ", value, sizeof(value));
            if (value[0]) snprintf(c.name, sizeof(c.name), "%s", value);

            readLine("New phone: ", newPhone, sizeof(newPhone));
            if (newPhone[0]) {
                if (phoneExists(newPhone, id)) {
                    printf("That phone number belongs to another contact. Keeping old phone.\n");
                } else {
                    snprintf(c.phone, sizeof(c.phone), "%s", newPhone);
                }
            }

            readLine("New email: ", value, sizeof(value));
            if (value[0]) snprintf(c.email, sizeof(c.email), "%s", value);

            readLine("New address: ", value, sizeof(value));
            if (value[0]) snprintf(c.address, sizeof(c.address), "%s", value);
        }
        if (fwrite(&c, sizeof(Contact), 1, temp) != 1) {
            printf("Error writing updated contact data.\n");
            fclose(fp);
            fclose(temp);
            remove(TEMP_FILE);
            return;
        }
    }

    fclose(fp);
    fclose(temp);

    if (!found) {
        remove(TEMP_FILE);
        printf("Contact ID not found.\n");
        return;
    }
    if (remove(DATA_FILE) != 0 || rename(TEMP_FILE, DATA_FILE) != 0) {
        printf("Could not replace the contact file. Check file permissions.\n");
        return;
    }
    printf("Contact updated successfully.\n");
}

void deleteContact(void) {
    FILE *fp = fopen(DATA_FILE, "rb");
    FILE *temp;
    Contact c;
    int id, found = 0;
    char confirm[10];

    printf("\n--- Delete Contact ---\n");
    if (fp == NULL) {
        printf("No contacts found yet.\n");
        return;
    }
    id = readInt("Enter the contact ID to delete: ");
    temp = fopen(TEMP_FILE, "wb");
    if (temp == NULL) {
        perror("Could not create temporary file");
        fclose(fp);
        return;
    }

    while (fread(&c, sizeof(Contact), 1, fp) == 1) {
        if (c.id == id) {
            found = 1;
            displayContact(&c);
            readLine("Are you sure you want to delete this contact? (y/n): ",
                     confirm, sizeof(confirm));
            if (tolower((unsigned char)confirm[0]) != 'y') {
                if (fwrite(&c, sizeof(Contact), 1, temp) != 1)
                    printf("Error writing contact data.\n");
                found = 0;
                continue;
            }
        } else {
            if (fwrite(&c, sizeof(Contact), 1, temp) != 1)
                printf("Error writing contact data.\n");
        }
    }

    fclose(fp);
    fclose(temp);

    if (!found) {
        remove(TEMP_FILE);
        printf("Contact was not deleted (ID not found or deletion cancelled).\n");
        return;
    }
    if (remove(DATA_FILE) != 0 || rename(TEMP_FILE, DATA_FILE) != 0) {
        printf("Could not replace the contact file. Check file permissions.\n");
        return;
    }
    printf("Contact deleted successfully.\n");
}

void showMenu(void) {
    printf("\n========================================\n");
    printf("       CONTACT MANAGEMENT SYSTEM\n");
    printf("========================================\n");
    printf("1. Add Contact\n");
    printf("2. View All Contacts\n");
    printf("3. Search Contact\n");
    printf("4. Update Contact\n");
    printf("5. Delete Contact\n");
    printf("0. Exit\n");
    printf("----------------------------------------\n");
}

int main(void) {
    int choice;

    while (1) {
        showMenu();
        choice = readInt("Enter your choice: ");

        switch (choice) {
            case 1: addContact(); break;
            case 2: viewAllContacts(); break;
            case 3: searchContact(); break;
            case 4: updateContact(); break;
            case 5: deleteContact(); break;
            case 0:
                printf("Thank you for using Contact Management System!\n");
                return 0;
            default:
                printf("Invalid choice. Please select from the menu.\n");
        }
    }
}
