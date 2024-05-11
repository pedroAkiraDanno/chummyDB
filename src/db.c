


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
#include <stdint.h> // Include this header for uint32_t

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
    struct Page page;
    struct Tuple tuple;

    // Initialize the page
    initPage(&page, 1);

    // Create a tuple and add it to the page
    strcpy(tuple.data, "Hello, World!");
    tuple.length = strlen(tuple.data) + 1; // Include the null terminator
    addTupleToPage(&page, &tuple);

    // Display page information
    printf("Page No: %u\n", page.header.pageNo);
    printf("Next Page No: %u\n", page.header.nextPageNo);
    printf("Free Space: %u\n", page.header.freeSpace);
    printf("Number of Items: %u\n", page.header.nItems);

    // Display tuple information
    printf("Tuple Length: %u\n", tuple.length);
    printf("Tuple Data: %s\n", tuple.data);

    return 0;
}














/*





COMPILE:    LINUX 

    apt-get install gcc
    vi db.c
    gcc db.c -o dbexec
    ./dbexec






COMPILE:    WINDOWS 
        cd C:\Users\pedro.akira\OneDrive - pwi.com.br\Documentos\GitHub\chummyDB\src
        gcc db.c -o dbexec
        .\dbexec 








*/









  /*****************************************************************************************************************
  *                                                   REFERENCIAS                                                  *
  * https://pt.stackoverflow.com/questions/104118/qual-%C3%A9-a-finalidade-dos-comandos-size-t-e-ssize-t-em-c      *
  ******************************************************************************************************************/














  














