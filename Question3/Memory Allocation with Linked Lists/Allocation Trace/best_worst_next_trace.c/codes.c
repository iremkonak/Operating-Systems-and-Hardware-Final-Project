// BEST FIT TRACE

#include <stdio.h>
#include <stdlib.h>

// Structure to represent a memory block
typedef struct Block {
    int size;
    int is_free; // 1 if the block is free, 0 if allocated
    struct Block *next;
} Block;

// Function to create a new memory block
Block* createBlock(int size, int is_free) {
    Block* newBlock = (Block*)malloc(sizeof(Block));
    newBlock->size = size;
    newBlock->is_free = is_free;
    newBlock->next = NULL;
    return newBlock;
}

// Function to print the sizes of all currently free blocks
void printFreeList(Block* head) {
    printf("Free List: ");
    Block* current = head;
    int found = 0;
    while (current != NULL) {
        if (current->is_free) {
            printf("[%d] ", current->size);
            found = 1;
        }
        current = current->next;
    }
    if (!found) printf("(No free blocks)");
    printf("\n");
}

// Implementation of the Best Fit Allocation algorithm
void allocateBestFit(Block* head, int requestSize) {
    Block* current = head;
    Block* bestBlock = NULL;

    // Traverse to find the smallest block that fits the request
    while (current != NULL) {
        if (current->is_free && current->size >= requestSize) {
            if (bestBlock == NULL || current->size < bestBlock->size) {
                bestBlock = current;
            }
        }
        current = current->next;
    }

    if (bestBlock != NULL) {
        // If the block is larger than needed, split it
        if (bestBlock->size > requestSize) {
            Block* newFreeBlock = createBlock(bestBlock->size - requestSize, 1);
            newFreeBlock->next = bestBlock->next;
            bestBlock->next = newFreeBlock;
            bestBlock->size = requestSize;
        }
        bestBlock->is_free = 0;
        printf("Allocated %d units.\n", requestSize);
    } else {
        printf("Failed to allocate %d units (No suitable block).\n", requestSize);
    }
}

// Function to free the FIRST block found with the specified size
void freeBlockBySize(Block* head, int targetSize) {
    Block* current = head;
    while (current != NULL) {
        if (!current->is_free && current->size == targetSize) {
            current->is_free = 1;
            printf("Freed block of size %d.\n", targetSize);
            return;
        }
        current = current->next;
    }
    printf("Block of size %d not found to free.\n", targetSize);
}

int main() {
    // Initial memory setup: One large block of 100 units
    Block* memoryHead = createBlock(100, 1);
    
    // Trace sequence from the experiment
    int trace[] = {10, 5, 20, -5, 12, -10, 8, 6, 7, 3, 10};
    int n = sizeof(trace) / sizeof(trace[0]);

    printf("--- Best Fit Memory Allocation Trace ---\n");
    printFreeList(memoryHead);
    printf("----------------------------------------\n");

    for (int i = 0; i < n; i++) {
        int request = trace[i];
        if (request > 0) {
            printf("\nRequest: Allocate %d\n", request);
            allocateBestFit(memoryHead, request);
        } else {
            printf("\nRequest: Free %d\n", -request);
            freeBlockBySize(memoryHead, -request);
        }
        printFreeList(memoryHead);
    }

    return 0;
}


// WORST FIT TRACE

#include <stdio.h>
#include <stdlib.h>

typedef struct Block {
    int size;
    int is_free;
    struct Block *next;
} Block;

Block* createBlock(int size, int is_free) {
    Block* newBlock = (Block*)malloc(sizeof(Block));
    newBlock->size = size;
    newBlock->is_free = is_free;
    newBlock->next = NULL;
    return newBlock;
}

void printFreeList(Block* head) {
    printf("Free List: ");
    Block* current = head;
    while (current != NULL) {
        if (current->is_free) printf("[%d] ", current->size);
        current = current->next;
    }
    printf("\n");
}

// WORST FIT: Finds the LARGEST available block
void allocateWorstFit(Block* head, int requestedSize) {
    Block* current = head;
    Block* worstBlock = NULL;

    while (current != NULL) {
        if (current->is_free && current->size >= requestedSize) {
            if (worstBlock == NULL || current->size > worstBlock->size) {
                worstBlock = current;
            }
        }
        current = current->next;
    }

    if (worstBlock != NULL) {
        if (worstBlock->size > requestedSize) {
            Block* newFreeBlock = createBlock(worstBlock->size - requestedSize, 1);
            newFreeBlock->next = worstBlock->next;
            worstBlock->next = newFreeBlock;
            worstBlock->size = requestedSize;
        }
        worstBlock->is_free = 0;
        printf("Allocated %d units (Worst Fit)\n", requestedSize);
    } else {
        printf("Failed to allocate %d units\n", requestedSize);
    }
}

void freeBlockBySize(Block* head, int targetSize) {
    Block* current = head;
    while (current != NULL) {
        if (!current->is_free && current->size == targetSize) {
            current->is_free = 1;
            printf("Freed block of size %d\n", targetSize);
            return;
        }
        current = current->next;
    }
}

int main() {
    Block* memoryHead = createBlock(100, 1);
    int trace[] = {10, 5, 20, -5, 12, -10, 8, 6, 7, 3, 10};
    int n = sizeof(trace) / sizeof(trace[0]);

    printf("--- Worst Fit Memory Allocation ---\n");
    for (int i = 0; i < n; i++) {
        if (trace[i] > 0) allocateWorstFit(memoryHead, trace[i]);
        else freeBlockBySize(memoryHead, -trace[i]);
        printFreeList(memoryHead);
    }
    return 0;
}


// NEXT FIT TRACE

#include <stdio.h>
#include <stdlib.h>

typedef struct Block {
    int size;
    int is_free;
    struct Block *next;
} Block;

// to keep track of the last allocated block
Block* lastPos = NULL;

Block* createBlock(int size, int is_free) {
    Block* newBlock = (Block*)malloc(sizeof(Block));
    newBlock->size = size;
    newBlock->is_free = is_free;
    newBlock->next = NULL;
    return newBlock;
}

void printFreeList(Block* head) {
    printf("Free List: ");
    Block* current = head;
    while (current != NULL) {
        if (current->is_free) printf("[%d] ", current->size);
        current = current->next;
    }
    printf("\n");
}

void allocateNextFit(Block* head, int requestedSize) {
    // if it's the first time, start from head
    if (lastPos == NULL) lastPos = head;

    Block* current = lastPos;
    Block* startNode = lastPos;

    // search from the last position until the end of the list
    do {
        if (current->is_free && current->size >= requestedSize) {
            if (current->size > requestedSize) {
                Block* newFreeBlock = createBlock(current->size - requestedSize, 1);
                newFreeBlock->next = current->next;
                current->next = newFreeBlock;
                current->size = requestedSize;
            }
            current->is_free = 0;
            lastPos = current->next; // update last position
            printf("Allocated %d units (Next Fit)\n", requestedSize);
            return;
        }
        current = (current->next == NULL) ? head : current->next; // loop back to head
    } while (current != startNode);

    printf("Failed to allocate %d units\n", requestedSize);
}

void freeBlockBySize(Block* head, int targetSize) {
    Block* current = head;
    while (current != NULL) {
        if (!current->is_free && current->size == targetSize) {
            current->is_free = 1;
            printf("Freed block of size %d\n", targetSize);
            return;
        }
        current = current->next;
    }
}

int main() {
    Block* memoryHead = createBlock(100, 1);
    lastPos = memoryHead;

    int trace[] = {10, 5, 20, -5, 12, -10, 8, 6, 7, 3, 10};
    int n = sizeof(trace) / sizeof(trace[0]);

    printf("--- Next Fit Memory Allocation ---\n");
    for (int i = 0; i < n; i++) {
        if (trace[i] > 0) allocateNextFit(memoryHead, trace[i]);
        else freeBlockBySize(memoryHead, -trace[i]);
        printFreeList(memoryHead);
    }
    return 0;
}

//
