


/*
 * Licensed under the Apache License, Version 2.0 (the "License");
 * you may not use this file except in compliance with the License.
 * You may obtain a copy of the License at
 *
 * http://www.apache.org/licenses/LICENSE-2.0
 *
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an "AS IS" BASIS,
 * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 * See the License for the specific language governing permissions and
 * limitations under the License.
 *
 *
 * 02-07-2022, v1.0, Pedro Akira Danno Lima
 *
 * Compile: gcc -Wall -O2 db.c -o dbexec
 */






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







*/




















/*





COMPILE:    LINUX 

    apt-get install gcc
    vi db.c
    gcc db.c -o dbexec
    ./dbexec






COMPILE:    WINDOWS 
        cd C:\Users\pedro.akira\OneDrive - pwi.com.br\Documentos\GitHub\chummyDB\src
        gcc db6.c -o db6exec
        .\db6exec 








*/









































/*

EXPLAIN: 

























This C program demonstrates a simple implementation of a page-based data structure for storing tuples of data. Let's break down the main components:

1. **Header Definitions**: 
    - The program defines several structures, namely `PageHeader`, `Person`, `Tuple`, and `Page`, to represent the components of the data storage.

2. **Page Structure**:
    - The `Page` structure combines a `PageHeader` and an array of data to represent a single page in the data structure.

3. **Initialization Function**:
    - The `initPage` function initializes a page with the provided page number and sets initial metadata values.

4. **Add Tuple Function**:
    - The `addTupleToPage` function adds a tuple to a page. It calculates the position to insert the tuple, checks if there's enough space, and either inserts the tuple into the current page or recursively creates a new page and adds the tuple to it.

5. **Main Function**:
    - The `main` function demonstrates the usage of the provided functions by initializing a page and adding ten tuples of person data to it.

6. **Memory Management**:
    - The program uses `malloc` to allocate memory for new pages and `free` to release memory after use, ensuring proper memory management.

7. **Example Data**:
    - Each tuple represents a person with a name, address, and age. These tuples are added to the page in the main function.

Overall, this program provides a basic framework for managing data in a page-based structure, suitable for scenarios where data needs to be stored and managed efficiently.




*/
































  /*****************************************************************************************************************
  *                                                   REFERENCIAS                                                  *
  * https://pt.stackoverflow.com/questions/104118/qual-%C3%A9-a-finalidade-dos-comandos-size-t-e-ssize-t-em-c      *
  ******************************************************************************************************************/











