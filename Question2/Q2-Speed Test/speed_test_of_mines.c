#include <stdio.h>
#include <stdlib.h>
#include <time.h>

#define STORAGE_SIZE 64

//  BITMAP LOGIC 
int test_bitmap[STORAGE_SIZE] = {0};

void bitmap_allocate_logic(int size) {
    int count = 0, start = -1;
    for (int i = 0; i < STORAGE_SIZE; i++) {
        if (test_bitmap[i] == 0) {
            if (count == 0) start = i;
            count++;
            if (count == size) {
                for (int j = start; j < start + size; j++) test_bitmap[j] = 1;
                return;
            }
        } else count = 0;
    }
}

void bitmap_reset() {
    for (int i = 0; i < STORAGE_SIZE; i++) test_bitmap[i] = 0;
}

//  LINKED LIST LOGIC 
struct TestNode { int id; struct TestNode* next; };
struct TestNode* test_free_list = NULL;

void list_add_block(int id) {
    struct TestNode* n = (struct TestNode*)malloc(sizeof(struct TestNode));
    n->id = id; n->next = test_free_list; test_free_list = n;
}

void list_allocate_logic(int size) {
    for (int i = 0; i < size && test_free_list != NULL; i++) {
        struct TestNode* temp = test_free_list;
        test_free_list = test_free_list->next;
        free(temp);
    }
}

int main() {
    // increased iterations to see measurable time 
    int iterations = 100000; 
    clock_t start, end;

    printf("Starting Speed Test with %d iterations...\n", iterations);

    // 1. BITMAP TEST
    start = clock();
    for(int i = 0; i < iterations; i++) {
        bitmap_allocate_logic(2);
        bitmap_reset(); // reset for next iteration
    }
    end = clock();
    printf("Bitmap Speed: %f seconds\n", (double)(end - start) / CLOCKS_PER_SEC);

    // 2. LINKED LIST TEST
    // setup list
    for(int i = 0; i < STORAGE_SIZE; i++) list_add_block(i);

    start = clock();
    for(int i = 0; i < iterations; i++) {
        list_allocate_logic(2);
    }
    end = clock();
    printf("Linked-List Speed: %f seconds\n", (double)(end - start) / CLOCKS_PER_SEC);

    return 0;
} 
