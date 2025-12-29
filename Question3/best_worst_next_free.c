// BEST - FIT
#include <stdio.h>
#include <stdlib.h>

struct FreeSegment {
    int start_address;      // Segment starting address
    int total_length;       // Total length of the segment
    struct FreeSegment* next; //Pointer to the next segment
};

struct FreeSegment* free_list_head = NULL;

// STEP 2: Function to insert an empty segment
void add_free_segment(int start, int length) {
    struct FreeSegment* new_node = (struct FreeSegment*)malloc(sizeof(struct FreeSegment));
    new_node->start_address = start;
    new_node->total_length = length;
    new_node->next = free_list_head;
    free_list_head = new_node;
}

// STEP 3: Best Fit Allocation Algorithm
int allocate_best_fit(int requested_size) {
    struct FreeSegment *current = free_list_head;
    struct FreeSegment *best_segment = NULL;
    
    printf("\n--- Best Fit: Searching for %d units ---\n", requested_size);

    while (current != NULL) {
        // We are checking the segments that meet our needs.
        if (current->total_length >= requested_size) {
            // Update if we find a more suitable (smaller) candidate
            if (best_segment == NULL || current->total_length < best_segment->total_length) {
                best_segment = current;
            }
        }
        current = current->next;
    }

    if (best_segment != NULL) {
        int allocated_address = best_segment->start_address;
        printf("SELECTED: Segment at %d (Length: %d)\n", best_segment->start_address, best_segment->total_length);
        
        // Update remaining space
        best_segment->start_address += requested_size;
        best_segment->total_length -= requested_size;
        
        return allocated_address;
    }

    printf("FAILED: No suitable segment found.\n");
    return -1;
}

int main() {
    // Test Scenario: We divide the memory into 3 parts. 
    add_free_segment(80, 10); // smallest candidate
    add_free_segment(30, 40); // biggest candidate
    add_free_segment(0, 15);  // miidle candidate

    // 8 birim yer istiyoruz. Best Fit'in 10'luk olanı seçmesi lazım
    int address = allocate_best_fit(8);
    
    if (address != -1) {
        printf("SUCCESS: Memory starts at %d\n", address);
    }

    return 0;



  // WORST FIT

  #include <stdio.h>
#include <stdlib.h>

struct FreeSegment {
    int start_address;      // Segment starting address
    int total_length;       // Total length of the segment
    struct FreeSegment* next; 
};

struct FreeSegment* free_list_head = NULL;

// Function that adds an empty segment to the list
void add_free_segment(int start, int length) {
    struct FreeSegment* new_node = (struct FreeSegment*)malloc(sizeof(struct FreeSegment));
    new_node->start_address = start;
    new_node->total_length = length;
    new_node->next = free_list_head;
    free_list_head = new_node;
}

// Worst Fit Allocation Algorithm
int allocate_worst_fit(int requested_size) {
    struct FreeSegment *current = free_list_head;
    struct FreeSegment *worst_candidate = NULL;
    
    printf("\n--- Worst Fit Searching for %d units ---\n", requested_size);

   // Step 1: Scan the entire list and find the "largest" suitable segment.
    while (current != NULL) {
        if (current->total_length >= requested_size) {
            printf("Checking segment [%d, %d]: Valid candidate.\n", current->start_address, current->total_length);
            // If a larger vacancy is found, the candidate will be updated.
            if (worst_candidate == NULL || current->total_length > worst_candidate->total_length) {
                worst_candidate = current;
            }
        }
        current = current->next;
    }

    // Step 2: If the strongest candidate is found, allocate it and divide the segment.
    if (worst_candidate != NULL) {
        int allocated_address = worst_candidate->start_address;
        printf("SELECTED: Largest segment starts at %d (Length: %d)\n", worst_candidate->start_address, worst_candidate->total_length);
        
       // The remaining space is updated and kept in the list. 
        worst_candidate->start_address += requested_size;
        worst_candidate->total_length -= requested_size;
        
        return allocated_address;
    }

    printf("FAILED: No suitable segment found.\n");
    return -1;
}

int main() {
    printf("--- Final Project: Worst Fit Memory Experiment ---\n");

    // Scenario: Memory is fragmented. [
    add_free_segment(80, 10); // Segment A (small)
    add_free_segment(30, 40); // Segment B (biggest)
    add_free_segment(0, 15);  // Segment C (middle)

    // We want 8 units of space. 
   // Worst Fit should select the 40-unit option. 
    int result_address = allocate_worst_fit(8);

    if (result_address != -1) {
        printf("\nSUCCESS: Memory allocated at address %d\n", result_address);
        printf("ANALYSIS: Remaining space in this block is now %d units.\n", 40 - 8);
    }

    return 0;
}




// NEXT FIT


  #include <stdio.h>
#include <stdlib.h>

struct FreeSegment {
    int start_address;
    int total_length;
    struct FreeSegment* next;
};

struct FreeSegment* free_list_head = NULL;
// Marker that keeps track of where we last left off
struct FreeSegment* current_search_ptr = NULL;

// Function that adds an empty segment to the list
void add_segment(int start, int length) {
    struct FreeSegment* new_node = (struct FreeSegment*)malloc(sizeof(struct FreeSegment));
    new_node->start_address = start;
    new_node->total_length = length;
    new_node->next = free_list_head;
    free_list_head = new_node;
}

// Next Fit Allocation Algorithm
int allocate_next_fit(int requested_size) {
    if (free_list_head == NULL) return -1;

    // If it's your first time using it, start from the beginning.
    if (current_search_ptr == NULL) current_search_ptr = free_list_head;

    struct FreeSegment *starting_point = current_search_ptr;
    printf("\n--- Next Fit: Searching for %d units starting from address %d ---\n", 
           requested_size, current_search_ptr->start_address);

    do {
        // Has a suitable segment been found
        if (current_search_ptr->total_length >= requested_size) {
            int allocated_addr = current_search_ptr->start_address;
            printf("SUCCESS: Found at address %d (Remaining in segment: %d)\n", 
                   allocated_addr, current_search_ptr->total_length - requested_size);

            // Split and update the segment
            current_search_ptr->start_address += requested_size;
            current_search_ptr->total_length -= requested_size;

            // IMPORTANT: We leave the pointer here for the next search.
            return allocated_addr;
        }

        // Next segment
        current_search_ptr = current_search_ptr->next;

        // If you've reached the end of the list, go back to the beginning. 
        if (current_search_ptr == NULL) {
            current_search_ptr = free_list_head;
            printf("End of list reached. Wrapping around to the beginning...\n");
        }

    } while (current_search_ptr != starting_point); // Stop if the entire list has been returned

    printf("FAILED: No suitable space found after full scan.\n");
    return -1;
}

int main() {
    printf("--- Final Project: Next Fit Experiment ---\n");

    // Scenario: Memory is fragmented.
    add_segment(80, 20); // Segment 3
    add_segment(40, 30); // Segment 2
    add_segment(0, 10);  // Segment 1
    
    // 1. Allocation: We request 5 units. They will be placed in Segment 1.
    allocate_next_fit(5);
    
    // 2. Allocation: We want 15 units. Next Fit does not start from the beginning,
    // It continues from where Segment 1 left off and finds Segment 2.
    allocate_next_fit(15);
    
    // 3. Allocation: We request 25 units. Since there is not enough space in Segment 2,
    // It will jump to Segment 3.
    allocate_next_fit(25);

    return 0;
}


  // FREE


  #include <stdio.h>
#include <stdlib.h>

struct FreeSegment {
    int start_address;
    int total_length;
    struct FreeSegment* next;
};

struct FreeSegment* free_list_head = NULL;

// A function that frees memory and merges adjacent blocks.
void free_memory(int start, int length) {
    struct FreeSegment* new_node = (struct FreeSegment*) malloc(sizeof(struct FreeSegment));
    new_node->start_address = start;
    new_node->total_length = length;
    new_node->next = NULL;

    // STEP 1: Add the new block to the list
    if (free_list_head == NULL || start < free_list_head -> start_address) {
        new_node->next = free_list_head;
        free_list_head = new_node;
    } else {
        struct FreeSegment* curr = free_list_head;
        while (curr->next != NULL && curr->next->start_address < start) {
            curr = curr->next;
        }
        new_node->next = curr->next;
        curr->next = new_node;
    }
    printf("\n--- Freeing: Address %d, Size %d ---\n", start, length);

    // STEP 2: Merge adjacent blocks
    struct FreeSegment* curr = free_list_head;
    while (curr != NULL && curr->next != NULL) {
        // If the end of the current block is equal to the beginning of the next, merge.
        if (curr->start_address + curr->total_length == curr->next->start_address) {
            printf("MERGING: Block at %d and Block at %d are now ONE.\n", 
                   curr->start_address, curr->next->start_address);
            
            curr->total_length += curr->next->total_length; // add lengths
            struct FreeSegment* temp = curr->next;
            curr->next = curr->next->next; // Remove the intermediate node from the list.
            free(temp); // clear memory
        } else {
            curr = curr->next;
        }
    }
} 

void display_list() {
    struct FreeSegment* curr = free_list_head;
    printf("FreeList: ");
    while(curr) {
        printf("[%d, %d] -> ", curr->start_address, curr->total_length);
        curr = curr->next;
    }
    printf("NULL\n");
}


void add_segment_manual(int s, int l) {
    struct FreeSegment* n = (struct FreeSegment*)malloc(sizeof(struct FreeSegment));
    n->start_address = s; n->total_length = l; n->next = free_list_head;
    free_list_head = n;
}

int main() {
    printf("--- Final Project: Free and Merging Experiment ---\n");

    // There are two separete spaces
    add_segment_manual(20, 10); // 20-30 empty
    add_segment_manual(0, 10);  // 0-10 empty
    display_list();

    // Scenario: We leave the 10-20 interval free.
    // The three segments (0-10, 10-20, 20-30) must combine to form a single [0, 30] block.
    free_memory(10, 10);
    
    display_list();
    return 0;
}
}
