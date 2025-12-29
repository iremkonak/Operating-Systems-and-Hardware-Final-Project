#include <stdio.h>
#include <stdlib.h>

#define DISK_SIZE 64

// --- bitmap implementation ---
void run_bitmap_trace(int sequence[], int n) {
    int bitmap[DISK_SIZE] = {0};
    printf("\n--- Step 3: Bitmap Allocation Trace (15 Steps) ---\n");

    for (int i = 0; i < n; i++) {
        int size = sequence[i];
        int start_index = -1;

        for (int j = 0; j <= DISK_SIZE - size; j++) {
            int can_fit = 1;
            for (int k = j; k < j + size; k++) {
                if (bitmap[k] == 1) { can_fit = 0; break; }
            }
            if (can_fit) {
                for (int k = j; k < j + size; k++) bitmap[k] = 1;
                start_index = j;
                break;
            }
        }

        printf("Step %d (Size %d): ", i + 1, size);
        for (int j = 0; j < DISK_SIZE; j++) printf("%d", bitmap[j]);
        if (start_index == -1) printf(" -> FAILED");
        printf("\n");
    }
}

// --- linked-list implementation ---
struct Node { int id; struct Node* next; };

void run_list_trace(int sequence[], int n) {
    struct Node* head = NULL;
    // Initialize disk with 64 free blocks
    for (int i = DISK_SIZE - 1; i >= 0; i--) {
        struct Node* newNode = (struct Node*)malloc(sizeof(struct Node));
        newNode->id = i; newNode->next = head; head = newNode;
    }

    printf("\n--- Step 3: Linked-List Allocation Trace (15 Steps) ---\n");
    int disk_visual[DISK_SIZE];

    for (int i = 0; i < n; i++) {
        for(int v=0; v<DISK_SIZE; v++) disk_visual[v] = 1; // assume full, we will mark free later
        
        struct Node* curr = head;
        while(curr) { disk_visual[curr->id] = 0; curr = curr->next; }

        int size = sequence[i];
        int allocated_count = 0;
        while (allocated_count < size && head != NULL) {
            struct Node* temp = head;
            head = head->next;
            free(temp);
            allocated_count++;
        }

        printf("Step %d (Size %d): ", i + 1, size);
        // print 1 for used, 0 for free 
        // since we already allocated, the visualizer needs to be updated or logic inverted
        // we show the state AFTER allocation
        curr = head;
        int final_visual[DISK_SIZE];
        for(int v=0; v<DISK_SIZE; v++) final_visual[v] = 1; 
        while(curr) { final_visual[curr->id] = 0; curr = curr->next; }
        
        for (int j = 0; j < DISK_SIZE; j++) printf("%d", final_visual[j]);
        if (allocated_count < size) printf(" -> FAILED");
        printf("\n");
    }
}

int main() {
    int sequence[] = {2, 3, 5, 2, 4, 6, 1, 3, 5, 2, 4, 3, 2, 1, 5};
    int n = 15;

    run_bitmap_trace(sequence, n);
    run_list_trace(sequence, n);

    return 0;
}
