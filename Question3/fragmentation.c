// BEST WORST NEXT FRAGMENTATION TEST


#include <stdio.h>
#include <stdlib.h>
#include <time.h>

typedef struct Block{
    int id;
    int size;
    int is_free;
    struct Block* next;
} Block;

// pointer for next fit
Block* lastPosition = NULL;

Block* createBlock(int id, int size, int is_free) {
    Block*newBlock = (Block*)malloc(sizeof(Block));
    newBlock->id = id;
    newBlock->size = size;
    newBlock->is_free = is_free;
    newBlock->next = NULL;
    return newBlock;
}

void printFreeList( Block* head) {
    printf("Free List: ");
    Block* current = head;
    while (current != NULL) {
        if (current->is_free) printf("[%d]", current->size);
        current = current->next;
    }
    printf("\n");
}

//common split funtion fragmentation
void split(Block*b, int req, int id) {
    if(b->size > req) {
        Block* newFree = createBlock(-1, b->size - req, 1);
        newFree->next = b->next;
        b->next = newFree;
        b->size = req;
    }
    b->is_free = 0;
    b->id = id;
 }

// 1. BEST FIT
int bestFit(Block* head, int req, int id) {
    Block *curr = head, *best = NULL;
    while (curr != NULL) {
        if (curr->is_free && curr->size >= req) {
            if (best == NULL || curr->size < best->size) best = curr;
        }
        curr = curr->next;
    }
    if (best) { split(best, req, id); return 1; }
    return 0;
}

// 2 WORST FIT
int worstFit(Block* head, int req, int id) {
    Block* curr = head, *worst = NULL;
    while (curr != NULL) {
        if(curr->is_free && curr->size >= req) {
            if ( worst == NULL || curr->size >worst->size) worst = curr;
        }
        curr = curr->next;
    }
    if (worst) {split(worst, req, id); return 1;}
    return 0;
}

//3 NEXT FIT
int nextFit(Block* head, int req, int id) {
    if(lastPosition == NULL) lastPosition = head;
    Block* start = lastPosition;
    do {
        if (lastPosition->is_free && lastPosition->size >= req) {
            split(lastPosition, req, id);
            lastPosition = lastPosition->next;
            return 1;
        }
        lastPosition = (lastPosition->next == NULL) ? head: lastPosition->next;

    } while (lastPosition != start);
    return 0;
}

void freeById(Block* head, int targetId) {
    Block* current = head;
    while (current != NULL) {
        if (!current->is_free && current->id == targetId) {
            current->is_free = 1;
            current->id = -1;
            return;
        }
        current = current->next;
    }
}

int main() {
    srand(time(NULL));
    Block* memory = createBlock(-1, 100,1);
    int ids[12], sizes[12];

    printf("--- Expriment 2 Testing ---\n");

    // to be able to use same random numbers
    for(int i=0; i<12; i++) sizes[i] = (rand() %10) + 3;

    // best fit, worst fit, next fit can be changed to
    for (int i = 0; i < 12; i++) {
        if (nextFit(memory, sizes[i], i)) {
            ids[i] = i;
            printf("Allocated %d\n", sizes[i]);
        }
    }
    // random delete
    for(int i=0; i<4; i++) freeById(memory, i); 
    
    printFreeList(memory);

    printf("\nFinal Test: Attempting 25 units...\n");
    if (bestFit(memory, 25, 99)) printf("SUCCESS!\n");
    else printf("FAILED!\n");

    return 0;

}
