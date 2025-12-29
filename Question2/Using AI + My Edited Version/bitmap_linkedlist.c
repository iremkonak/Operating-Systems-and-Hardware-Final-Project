// BITMAP AI VERSION 

#include <stdio.h>

#define DISK_SIZE 32
int disk[DISK_SIZE] = {0}; 

void allocate_bitmap(int size) {
    int count = 0;
    int start = -1;

    for(int i = 0; i < DISK_SIZE; i++) {
        if(disk[i] == 0) { 
            if(count == 0) start = i;
            count++;
            
            if(count == size) { 
                for(int j = start; j < start + size; j++) {
                    disk[j] = 1; 
                }
                printf("Allocated %d blocks at index %d\n", size, start);
                return;
            }
        } else {
            count = 0; 
            start = -1;
        }
    }
    printf("Failed to allocate %d blocks.\n", size);
}

int main() {
    printf("--- Final Project Question 2: Bitmap Allocation ---\n");
    allocate_bitmap(5); 
    
    printf("Disk State: ");
    for(int i = 0; i < DISK_SIZE; i++) printf("%d", disk[i]);
    printf("\n");
    
    return 0;
}



// LINKED LIST AI VERSION

#include <stdio.h>
#include <stdlib.h>

struct Block {
    int block_number;
    struct Block* next_block;
};

struct Block* free_list = NULL;

void add_block(int number) {
    struct Block* new_node = (struct Block*)malloc(sizeof(struct Block));
    new_node->block_number = number;
    new_node->next_block = free_list;
    free_list = new_node;
}

void allocate_linked_list(int amount) {
    if (free_list == NULL) {
        printf("Error: Disk is completely full!\n");
        return;
    }

    printf("List: %d blocks being allocated... Chain: ", amount);
    for (int i = 0; i < amount; i++) {
        if (free_list != NULL) {
            struct Block* to_be_deleted = free_list;
            printf("Block %d", to_be_deleted->block_number);
            
            free_list = free_list->next_block; 
            free(to_be_deleted); 
            
            if (i < amount - 1) printf(" -> ");
            else printf(" -> end\n"); 
        }
    }
}

int main() {
    printf("--- Final Project Question 2: Linked-List Allocation ---\n");

    for (int i = 7; i >= 0; i--) {
        add_block(i);
    }

    allocate_linked_list(3); 
    return 0;
}


// BITMAP MY VERSION

#include <stdio.h>
/* * FINAL PROJECT - QUESTION 2: BITMAP ALLOCATION
 * Purpose: To simulate how an Operating System manages disk space 
 * using a binary map (0 for free, 1 for used).
 * Key Insight: Bitmap is efficient for checking availability but requires 
 * contiguous (consecutive) blocks for allocation.
 */

#define STORAGE_SIZE 32 // Size of simulated disk space
/* * FUNCTION: request_bitmap_allocation
 * Intent: To find the first available sequence of blocks that fits the requested size.
 * This follows the "First Fit" principle for consecutive memory.
 */

// The Bitmap: 0 means block is free, 1 means block is used [cite: 23, 24]
int disk_bitmap[STORAGE_SIZE] = {0};

// Function to find and allocate consecutive free blocks [cite: 25, 26]
void request_bitmap_allocation(int requested_size) {
    int count = 0;  // Counter to consecutive free blocks found
    int first_index = -1;
// through the entire disk to scan for empty space
    for (int i = 0; i < STORAGE_SIZE; i++) {
        if (disk_bitmap[i] == 0) { // Check if block is free [cite: 23]
            if (count == 0) first_index = i;
            count++;
            
            // If enough consecutive blocks are found [cite: 25]
            if (count == requested_size) {
                for (int j = first_index; j < first_index + requested_size; j++) {
                    disk_bitmap[j] = 1; // Mark blocks as allocated
                }
                printf("Successfully allocated %d blocks starting at index %d\n", requested_size, first_index);
                return;
            }
        } else {
            // Reset logic: If a used block (1) is hit, any previous sequence is invalid.
            // This is why Bitmap suffers from "External Fragmentation". (I mentioned first question.)
            count = 0; // Reset counter if a used block is encountered
        }
    }
    printf("Allocation failed: Not enough consecutive space for %d blocks.\n", requested_size);
}

int main() {
    printf("--- Experiment 3: Bitmap Allocation Trace ---\n");
    int trace_sequence[] = {2, 3, 5, 2, 4, 6, 1, 3, 5, 2, 4, 3, 2, 1, 5}; // [cite: 68]

    for (int i = 0; i < 15; i++) {
        request_bitmap_allocation(trace_sequence[i]);
        printf("Step %d (Size %d): ", i + 1, trace_sequence[i]);
        for (int j = 0; j < STORAGE_SIZE; j++) printf("%d", disk_bitmap[j]); // [cite: 69]
        printf("\n");
    }
    return 0;
} 

// MY VERSION LINKED LIST

#include <stdio.h>
#include <stdlib.h>

/* * FINAL PROJECT - QUESTION 2: LINKED-LIST ALLOCATION
 * Purpose: To simulate disk space management using a pointer-based chain.
 * Key Insight: In this method, the Operating System does NOT require 
 * consecutive blocks. Each block knows where the next available block is.
 */

// structure for a linked disk block [cite:32]
struct DiskNode {
    int block_id; // for identifier for the disk block
    struct DiskNode* next_available; // pointer to the next free block in the chain
};

struct DiskNode* free_list_head = NULL;

/* * FUNCTION: register_free_block
 * Intent: To initialize the disk by linking blocks together.
 * This builds the initial "Free List" that the OS will use for future requests.
 */
 
void register_free_block(int id) {
    struct DiskNode* new_node = (struct DiskNode*)malloc(sizeof(struct DiskNode));
    new_node->block_id = id;
    // Link the new block to the current head (building the chain backwards)
    new_node->next_available = free_list_head;
    free_list_head = new_node;
}
// allocate blocks by following the pointer chain [cite: 35]
void request_list_allocation (int size) {
    if (free_list_head == NULL) {
        printf("Error: No free blocks left.\n");
        return;
    }
    printf("Allocating %d blocks. Chain: ", size);
    for (int i = 0; i < size; i++) {
        if (free_list_head != NULL) {
            struct DiskNode* temp = free_list_head;
            printf("Block %d", temp->block_id);

            free_list_head = free_list_head->next_available;
            free(temp);

            if (i < size - 1) printf(" -> ");
            else printf(" -> end\n");
        }
    }
}
 
int main() {
    printf("---Final Project: Linked-List Allocation ---\n");
    //initialize with 32 free blocks
    for (int i = 31; i >= 0; i--) register_free_block(i);

    request_list_allocation(5);
    return 0;
}


