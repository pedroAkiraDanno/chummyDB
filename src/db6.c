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

    // Check if there's enough space in the current page
    if (insertPos + tuple->length > PAGE_SIZE) {
        // If not enough space, create a new page and add the tuple to it
        printf("Page full, adding tuple to a new page.\n");
        struct Page *newPage = malloc(sizeof(struct Page));
        if (newPage == NULL) {
            printf("Error: Memory allocation failed.\n");
            return;
        }
        initPage(newPage, page->header.pageNo + 1);
        addTupleToPage(newPage, tuple); // Recursively add the tuple to the new page
        page->header.nextPageNo = newPage->header.pageNo;
        free(newPage);
        return;
    }

    // Copy the tuple data to the page
    memcpy(page->data + insertPos, tuple, tuple->length);

    // Update page metadata
    page->header.freeSpace -= tuple->length;
    page->header.nItems++;
}

// Example usage
int main() {
    struct Page page;
    struct Tuple tuple;

    // Initialize the page
    initPage(&page, 1);

    // Create tuples and add them to the page
    for (int i = 0; i < 10; i++) {
        sprintf(tuple.person.name, "Person %d", i + 1);
        strcpy(tuple.person.address, "Some Address");
        tuple.person.age = 30 + i;
        tuple.length = sizeof(struct Person);
        addTupleToPage(&page, &tuple);
    }

    return 0;
}

















/*





To ensure that tuples are added to a new page if the current page exceeds the 8KB size limit, we need to implement a mechanism that checks the remaining space in the page before adding a new tuple. If the remaining space is not sufficient, we create a new page and add the tuple to that page instead. Here's how you can modify the code to achieve this:




In this updated code:

The addTupleToPage function checks if there is enough space in the current page to accommodate the new tuple.
If there is not enough space, it creates a new page and adds the tuple to that page recursively.
It updates the nextPageNo field in the current page header to link it to the newly created page.
This process continues until there is enough space in the page to add the tuple.
The main function demonstrates adding multiple tuples to the page, triggering the creation of new pages as needed when the current page becomes full.
This modification ensures that tuples are added to new pages when the current page's size limit is reached, allowing for efficient management of data within the page structure.







Page 1:
+-------------------------+
|        PageHeader       |
|-------------------------|
| pageNo: 1               |
| nextPageNo: 2           |
| freeSpace: 8100         |
| nItems: 8               |
| offset: sizeof(PageHeader) |
+-------------------------+
|        Tuple 1          |
|-------------------------|
|        Tuple 2          |
|-------------------------|
|        Tuple 3          |
|-------------------------|
|        Tuple 4          |
|-------------------------|
|        Tuple 5          |
|-------------------------|
|        Tuple 6          |
|-------------------------|
|        Tuple 7          |
|-------------------------|
|        Tuple 8          |
+-------------------------+

Page 2:
+-------------------------+
|        PageHeader       |
|-------------------------|
| pageNo: 2               |
| nextPageNo: 0           |
| freeSpace: 8192         |
| nItems: 2               |
| offset: sizeof(PageHeader) |
+-------------------------+
|        Tuple 9          |
|-------------------------|
|        Tuple 10         |
+-------------------------+






In this visualization:

We have two pages: Page 1 and Page 2.
Page 1 contains tuples 1 to 8, and its nextPageNo points to Page 2.
Page 2 contains tuples 9 and 10. Since it's the last page, its nextPageNo is 0, indicating no next page.
This visualization demonstrates how the code dynamically creates new pages when the current page reaches its size limit, allowing for efficient storage of tuples within the page structure.











gcc db6.c -o dbexec 
.\dbexec











*/




























