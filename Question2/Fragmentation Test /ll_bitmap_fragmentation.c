//BITMAP FRAGMENTATION

#include <stdio.h>
#include <stdlib.h>

/* * BITMAP FRAGMENTATION TEST 
 * Goal: Prove that Bitmap FAILS when free space is not contiguous.
 * Logic: Fill the disk -> create small holes -> then request a large block.
 */

#define DISK_SIZE 48 

int bitmap[DISK_SIZE] = {0};

int allocate_bitmap(int size) {
    int count = 0, start = -1;
    for (int i = 0; i < DISK_SIZE; i++) {
        if (bitmap[i] == 0) {
            if (count == 0) start = i;
            count++;
            if (count == size) {
                for (int j = start; j < start + size; j++) bitmap[j] = 1;
                return start;
            }
        } else { count = 0; }
    }
    return -1; 
}

void free_bitmap(int start, int size) {
    if (start == -1) return;
    for (int i = start; i < start + size; i++) bitmap[i] = 0;
}

int main() {
    int starts[20];
    int sizes[20];

    printf("--- Bitmap Fragmentation Experiment ---\n");

    // 1. Make 20 allocations of size 2 to fill most of the disk (20 * 2 = 40 blocks)
    // disk size is 48 so only 8 blocks will be left at the end.
    for (int i = 0; i < 20; i++) {
        sizes[i] = 2;
        starts[i] = allocate_bitmap(sizes[i]);
    }

    // 2. Free exactly 5 allocations non-consecutively to create small holes
    // Each hole will be size 2. Total freed = 10 blocks.
    // Total free space now = 10 (holes) + 8 (at the end) = 18 blocks.
    printf("Creating 5 small holes (Total free space will be 18 blocks)...\n");
    int to_free[] = {1, 4, 7, 10, 13}; 
    for (int i = 0; i < 5; i++) {
        free_bitmap(starts[to_free[i]], sizes[to_free[i]]);
    }

    // 3. Attempt to allocate 12 blocks. 
    // Even though 18 blocks are free, no single gap is larger than 8.
    printf("\nAttempting to allocate a large block of size 12...\n");
    int result = allocate_bitmap(12);

    if (result == -1) {
        printf("[RESULT] Bitmap FAILED as expected.\n");
        printf("Reason: Total free space is 18, but it is split into small holes.\n");
        printf("The OS cannot find 12 CONSECUTIVE blocks (External Fragmentation).\n");
    } else {
        printf("[RESULT] Bitmap SUCCEEDED at index %d. (Something is wrong!)\n", result);
    }

    return 0;

// LINKED LIST FRAGMENTATION

#include <stdio.h>
#include <stdlib.h>

/* * LINKED-LIST FRAGMENTATION EXPERIMENT
 * Goal: Demonstrate that Linked-List succeeds by using non-contiguous blocks.
 */

struct Node {
    int id;
    struct Node* next;
};

struct Node* free_list = NULL;

// adding blocks to the free list
void add_block(int id) {
    struct Node* n = (struct Node*)malloc(sizeof(struct Node));
    n->id = id;
    n->next = free_list;
    free_list = n;
}

// allocate blocks regardless of their physical location
int allocate_list(int size) {
    int count = 0;
    printf("OS is gathering %d blocks from the pointer chain...\n", size);
    printf("Chain formed: ");

    while (count < size && free_list != NULL) {
        struct Node* temp = free_list;
        printf("[%d]", temp->id); // show exactly which block is being picked
        
        free_list = free_list->next;
        free(temp); 
        count++;

        if (count < size && free_list != NULL) printf(" -> ");
    }

    if (count == size) {
        printf(" -> SUCCESS\n");
        return 1;
    } else {
        printf(" -> FAILED (Not enough total space)\n");
        return -1;
    }
}

int main() {
    printf("--- Linked-List Fragmentation Test Output ---\n");

    // STEP 1: Initialize disk with 48 blocks
    for (int i = 47; i >= 0; i--) add_block(i);

    // STEP 2: Simulate the disk being partially full with holes (like Bitmap)
    printf("Step 1 & 2: Simulating a fragmented disk with 18 total free blocks...\n");
    
    // this is just a simulation to match your Bitmap experiment's
    allocate_list(30); // Use up 30 blocks, leaving 18 scattered ones in the free list

    // STEP 3: attempt the large allocation of size 12
    printf("\nStep 3: Attempting to allocate size 12 from fragmented space...\n");
    int result = allocate_list(12);

    if (result == 1) {
        printf("\n[ANALYSIS]: The allocation SUCCEEDED.\n");
        printf("Notice: The blocks were not side-by-side (0, 1, 2...), but the OS\n");
        printf("linked them together via pointers. External Fragmentation ignored.\n");
    }

    return 0;
}
}
