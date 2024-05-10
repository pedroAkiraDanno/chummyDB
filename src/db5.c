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
    uint16_t offset; // New field for offset
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
    page->header.offset = sizeof(struct PageHeader); // Initialize offset after the header
}

// Function to add a tuple to a page
void addTupleToPage(struct Page *page, const struct Tuple *tuple) {
    // Calculate the position to insert the tuple
    uint32_t insertPos = PAGE_SIZE - page->header.freeSpace - tuple->length;

    // Copy the tuple data to the page
    memcpy(page->data + insertPos, tuple, tuple->length);

    // Update page metadata
    page->header.freeSpace -= tuple->length;
    page->header.nItems++;
}

// Function to search for a person by name in a page
void searchPersonByName(const struct Page *page, const char *name) {
    for (int i = 0; i < page->header.nItems; i++) {
        const struct Tuple *tuple = (const struct Tuple *)(page->data + page->header.offset + (sizeof(struct Tuple) * i));

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
        struct Tuple *tuple = (struct Tuple *)(page->data + page->header.offset + (sizeof(struct Tuple) * i));

        // Compare the name with the name in the tuple
        if (strcmp(tuple->person.name, name) == 0) {
            // Person found, remove the tuple
            // Move the remaining tuples to cover the deleted tuple
            uint32_t moveSize = (PAGE_SIZE - (page->header.offset + (sizeof(struct Tuple) * (i + 1)))) - page->header.freeSpace;
            memmove(page->data + page->header.offset + (sizeof(struct Tuple) * i), page->data + page->header.offset + (sizeof(struct Tuple) * (i + 1)), moveSize);

            // Update page metadata
            page->header.freeSpace += tuple->length;
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







I added a new field offset to the PageHeader structure, representing the offset within the page where the data starts.
When initializing the page, offset is set to the size of the header, indicating that the data starts immediately after the header.
In the addTupleToPage function, we calculate the insert position based on the offset and the tuple's length.
In the searchPersonByName and deletePersonByName functions, we use the offset to locate tuples within the page.
This modification allows us to support an offset within the page structure, making it more flexible for handling variable-length tuples and data.











+-------------------------+
|        PageHeader       |
|-------------------------|
| pageNo: 1               |
| nextPageNo: 0           |
| freeSpace: 8156         |
| nItems: 2               |
| offset: sizeof(PageHeader) |
+-------------------------+
|        Tuple 1          |
|-------------------------|
|        Tuple 2          |
|-------------------------|
|        Free Space       |
|                         |
|                         |
|                         |
|                         |
+-------------------------+











In this visualization:

PageHeader contains metadata about the page, including the page number, next page number, free space, number of items, and offset.
Tuple 1 and Tuple 2 represent the tuples stored within the page.
The free space area shows the available space for adding new tuples.
With the added offset field, the page structure can effectively handle variable-length tuples and data, allowing for more flexibility in managing the page's contents.












*/





























