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

struct Tuple {
    uint16_t length;
    char data[PAGE_SIZE - sizeof(struct PageHeader) - sizeof(uint16_t)];
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

// Example usage
int main() {
    struct Page pages[3];
    struct Tuple tuple;

    // Initialize pages
    for (int i = 0; i < 3; i++) {
        initPage(&pages[i], i + 1);
    }

    // Create tuples and add them to pages
    for (int i = 0; i < 3; i++) {
        for (int j = 0; j < 3; j++) {
            strcpy(tuple.data, "Tuple ");
            char index[2];
            sprintf(index, "%d", j + 1);
            strcat(tuple.data, index);
            tuple.length = strlen(tuple.data) + 1; // Include the null terminator
            addTupleToPage(&pages[i], &tuple);
        }
    }

    // Display page information
    for (int i = 0; i < 3; i++) {
        printf("Page No: %u\n", pages[i].header.pageNo);
        printf("Next Page No: %u\n", pages[i].header.nextPageNo);
        printf("Free Space: %u\n", pages[i].header.freeSpace);
        printf("Number of Items: %u\n", pages[i].header.nItems);
        printf("\n");
    }

    return 0;
}
