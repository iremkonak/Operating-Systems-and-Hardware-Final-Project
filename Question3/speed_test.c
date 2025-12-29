// SPEED TEST FOT BEST FIT, WORST FIT, NEXT FIT

#include <stdio.h>
#include <stdlib.h>
#include <time.h>

typedef struct Block {
    int id;          
    int size;
    int is_free;
    struct Block *next;
} Block;

// pointer for the next fit algorithm
Block* lastPos = NULL;

Block* createBlock(int id, int size, int is_free) {
    Block* newBlock = (Block*)malloc(sizeof(Block));
    newBlock->id = id;
    newBlock->size = size;
    newBlock->is_free = is_free;
    newBlock->next = NULL;
    return newBlock;
}

//free up 1500 units of memory before each test (enough space for 1000 operations)
Block* resetMemory() {
    lastPos = NULL; 
    return createBlock(-1, 2000, 1);
}

void split(Block* b, int req, int id) {
    if (b->size > req) {
        Block* newFree = createBlock(-1, b->size - req, 1);
        newFree->next = b->next;
        b->next = newFree;
        b->size = req;
    }
    b->is_free = 0;
    b->id = id;
}

// Algoorithms

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

int worstFit(Block* head, int req, int id) {
    Block *curr = head, *worst = NULL;
    while (curr != NULL) {
        if (curr->is_free && curr->size >= req) {
            if (worst == NULL || curr->size > worst->size) worst = curr;
        }
        curr = curr->next;
    }
    if (worst) { split(worst, req, id); return 1; }
    return 0;
}

int nextFit(Block* head, int req, int id) {
    if (lastPos == NULL) lastPos = head;
    Block* start = lastPos;
    do {
        if (lastPos->is_free && lastPos->size >= req) {
            split(lastPos, req, id);
            lastPos = lastPos->next ? lastPos->next : head;
            return 1;
        }
        lastPos = (lastPos->next == NULL) ? head : lastPos->next;
    } while (lastPos != start);
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

// Speed Function
void runSpeedTest(const char* name, int (*algo)(Block*, int, int)) {
    Block* memory = resetMemory();
    int activeIds[100000];
    int activeCount = 0;

    clock_t start = clock();

    for (int i = 0; i < 100000; i++) {
        int size = (rand() % 10) + 1; 
        
        // 1. Allocate
        if (algo(memory, size, i)) {
            activeIds[activeCount++] = i;
        }

        // 2. Free (If there are blocks inside, delete a random one)
        if (activeCount > 0) {
            int randomIndex = rand() % activeCount;
            freeById(memory, activeIds[randomIndex]);
            // Update ID list
            activeIds[randomIndex] = activeIds[activeCount - 1];
            activeCount--;
        }
    }

    clock_t end = clock();
    double time_spent = (double)(end - start) / CLOCKS_PER_SEC;
    printf("%s Total Time: %f seconds\n", name, time_spent);
}

int main() {
    srand(time(NULL));
    
    printf("--- Experiment 3: Speed Test (200 Iterations) ---\n");
    runSpeedTest("Best Fit ", bestFit);
    runSpeedTest("Worst Fit", worstFit);
    runSpeedTest("Next Fit ", nextFit);

    return 0;
}
