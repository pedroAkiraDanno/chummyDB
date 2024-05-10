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

// Example usage
int main() {
    struct Page page;
    struct Tuple tuple;

    // Initialize the page
    initPage(&page, 1);

    // Create a tuple and add it to the page
    strcpy(tuple.person.name, "John Doe");
    strcpy(tuple.person.address, "123 Main St, Anytown, USA");
    tuple.person.age = 30;
    tuple.length = sizeof(struct Person);
    addTupleToPage(&page, &tuple);

    // Display page information
    printf("Page No: %u\n", page.header.pageNo);
    printf("Next Page No: %u\n", page.header.nextPageNo);
    printf("Free Space: %u\n", page.header.freeSpace);
    printf("Number of Items: %u\n", page.header.nItems);

    // Display tuple information
    printf("Person Name: %s\n", tuple.person.name);
    printf("Person Address: %s\n", tuple.person.address);
    printf("Person Age: %d\n", tuple.person.age);

    return 0;
}
















/*

In this example:

The initPage function initializes a page with the provided page number.
The addTupleToPage function adds a tuple to the page, updating the page metadata accordingly.
In the main function, we create a page and a tuple, add the tuple to the page, and then print out information about the page and the tuple.
This example demonstrates how you can use the provided structures and functions to work with pages and tuples in a PostgreSQL-like storage system.






I've added a struct Person to represent a person's information.
The Tuple structure now includes a Person member instead of raw data.
We initialize a Page, create a Tuple with person data, and add it to the page.
Finally, we print out the information stored in the page.









Apologies for the oversight. The uint32_t type is defined in the stdint.h header, so we need to include that header to resolve the error. Here's the corrected code:



Sure! Here's an updated version of the code that adds multiple tuples to a page and demonstrates using multiple pages:

This demonstrates how you can work with multiple pages and add tuples to them. Each page maintains its own metadata and can store multiple tuples.



Sure, let's extend the Tuple structure to include a person's name, address, and age, and then add instances of this structure to the Page structure. Here's how you can do it:



In this example:

I've added a struct Person to represent a person's information.
The Tuple structure now includes a Person member instead of raw data.
We initialize a Page, create a Tuple with person data, and add it to the page.
Finally, we print out the information stored in the page.
This code demonstrates how you can include more complex structures within a tuple and then store instances of those tuples within a page.

























Sure, let's break down the code and visualize it step by step.

Code Explanation:

Structures:
struct Person: Represents information about a person, including their name, address, and age.
struct Tuple: Represents a tuple that will be stored within a page. In this case, each tuple contains a Person.
struct PageHeader: Metadata about the page, such as the page number, next page number, free space, and number of items.
struct Page: Represents a page, consisting of a header and data area where tuples are stored.


Functions:
void initPage(struct Page *page, uint32_t pageNo): Initializes a page with the provided page number.
void addTupleToPage(struct Page *page, const struct Tuple *tuple): Adds a tuple to a page.


Main Function:
Initializes a page.
Creates a tuple with person data (name, address, and age).
Adds the tuple to the page.
Displays information about the page and the tuple.






+-------------------------+
|        PageHeader       |
|-------------------------|
| pageNo: 1               |
| nextPageNo: 0           |
| freeSpace: 8156         |
| nItems: 1               |
+-------------------------+
|       Tuple (Person)    |
|-------------------------|
| Person Name: John Doe   |
| Person Address: ...     |
| Person Age: 30          |
+-------------------------+








In this visualization:

The PageHeader contains metadata about the page.
The Tuple contains information about a person, including their name, address, and age.
The page's free space decreases when a tuple is added.
This example shows only one tuple added to the page, but you can add multiple tuples depending on the available space.


This visual representation helps understand how data is organized within the page and how the page metadata and tuple data are structured.







*/


















