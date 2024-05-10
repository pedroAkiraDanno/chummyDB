#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <stdint.h>

// Define the structures
#define PAGE_SIZE 8192

struct PageHeader {
    uint32_t pageNo;
    uint32_t nextPageNo;
    uint16_t freeSpace;
    uint16_t nItems;
};

struct Person {
    char name[50];
    char address[100];
    int age;
};

struct Tuple {
    uint16_t length;
    struct Person person;
};

struct Page {
    struct PageHeader header;
    char data[PAGE_SIZE - sizeof(struct PageHeader)];
};

// Function to initialize a page
void initPage(struct Page *page, uint32_t pageNo) {
    page->header.pageNo = pageNo;
    page->header.nextPageNo = 0;
    page->header.freeSpace = PAGE_SIZE - sizeof(struct PageHeader);
    page->header.nItems = 0;
}

// Function to add a tuple to a page
void addTupleToPage(struct Page *page, const struct Tuple *tuple) {
    // Calculate the position to insert the tuple
    uint32_t insertPos = PAGE_SIZE - page->header.freeSpace - sizeof(struct Tuple);

    // Copy the tuple data to the page
    memcpy(page->data + insertPos, tuple, sizeof(struct Tuple));

    // Update page metadata
    page->header.freeSpace -= sizeof(struct Tuple);
    page->header.nItems++;
}

// Function to search for a person by name in a page
void searchPersonByName(const struct Page *page, const char *name) {
    for (int i = 0; i < page->header.nItems; i++) {
        const struct Tuple *tuple = (const struct Tuple *)(page->data + (sizeof(struct Tuple) * i));

        // Compare the name with the name in the tuple
        if (strcmp(tuple->person.name, name) == 0) {
            // Person found, print details
            printf("Person found:\n");
            printf("Name: %s\n", tuple->person.name);
            printf("Address: %s\n", tuple->person.address);
            printf("Age: %d\n", tuple->person.age);
            return; // Stop searching after finding the first match
        }
    }

    // If person is not found
    printf("Person with name '%s' not found.\n", name);
}

// Function to delete a person by name from a page
void deletePersonByName(struct Page *page, const char *name) {
    for (int i = 0; i < page->header.nItems; i++) {
        struct Tuple *tuple = (struct Tuple *)(page->data + (sizeof(struct Tuple) * i));

        // Compare the name with the name in the tuple
        if (strcmp(tuple->person.name, name) == 0) {
            // Person found, remove the tuple
            // Move the remaining tuples to cover the deleted tuple
            uint32_t moveSize = (page->header.nItems - i - 1) * sizeof(struct Tuple);
            memmove(page->data + (sizeof(struct Tuple) * i), page->data + (sizeof(struct Tuple) * (i + 1)), moveSize);

            // Update page metadata
            page->header.freeSpace += sizeof(struct Tuple);
            page->header.nItems--;

            printf("Person with name '%s' deleted.\n", name);
            return; // Stop after deleting the first match
        }
    }

    // If person is not found
    printf("Person with name '%s' not found.\n", name);
}

// Example usage
int main() {
    struct Page page;
    struct Tuple tuple;

    // Initialize the page
    initPage(&page, 1);

    // Create tuples and add them to the page
    strcpy(tuple.person.name, "John Doe");
    strcpy(tuple.person.address, "123 Main St, Anytown, USA");
    tuple.person.age = 30;
    tuple.length = sizeof(struct Person);
    addTupleToPage(&page, &tuple);

    strcpy(tuple.person.name, "Jane Smith");
    strcpy(tuple.person.address, "456 Elm St, Anytown, USA");
    tuple.person.age = 25;
    addTupleToPage(&page, &tuple);

    // Search for a person by name
    searchPersonByName(&page, "John Doe");
    searchPersonByName(&page, "Jane Smith");

    // Delete a person by name
    deletePersonByName(&page, "John Doe");
    deletePersonByName(&page, "Alice Johnson");

    return 0;
}








/*


gcc db4.c -o exec 
.\exec





*/





