#include "../include/bedrock.h"
#include <stdio.h>
#include <string.h>

static int pass_count = 0;
static int fail_count = 0;

#define FAIL(msg) do { \
    printf("  FAIL: %s\n", msg); \
    fail_count++; \
    return; \
} while(0)

#define ASSERT(cond, msg) do { \
    if (!(cond)) { \
        FAIL(msg); \
    } \
} while(0)

void test_all_operations(void) {
    printf("=== Testing All BRQueue Operations ===\n\n");
    
    // 1. Create queue
    printf("1. Creating queue...\n");
    BRQueue *queue = BRQueue_new();
    ASSERT(queue != NULL, "Failed to create queue");
    ASSERT(queue_size(queue) == 0, "New queue should have size 0");
    ASSERT(queue_is_empty(queue) == 1, "New queue should be empty");
    printf("   Created successfully\n\n");
    pass_count++;
    
    // 2. Enqueue mixed types (FIFO order)
    printf("2. Enqueueing mixed types...\n");
    enqueue(queue, "Pranav");
    enqueue(queue, 101);
    enqueue(queue, (char)'D');
    enqueue(queue, 26.37);
    ASSERT(queue_size(queue) == 4, "Size should be 4 after 4 enqueues");
    printf("   Enqueued: \"Pranav\", 101, 'D', 26.37\n\n");
    pass_count++;
    
    // 3. Print queue
    printf("3. Printing queue...\n   ");
    queue_print(queue);
    pass_count++;
    
    // 4. Peek front
    printf("4. Peeking front...\n");
    Value *front = queue_peek(queue);
    ASSERT(front != NULL && front->type == TYPE_STRING && strcmp(front->as.s, "Pranav") == 0, "Front should be \"Pranav\"");
    printf("   Front: ");
    value_print(front);
    printf("\n");
    ASSERT(queue_size(queue) == 4, "Size should still be 4 after peek");
    printf("   Size after peek: %d\n\n", queue_size(queue));
    pass_count++;
    
    // 5. Check size
    printf("5. Checking size...\n");
    ASSERT(queue_size(queue) == 4, "Size should be 4");
    printf("   Size: %d\n\n", queue_size(queue));
    pass_count++;
    
    // 6. Dequeue elements (FIFO order)
    printf("6. Dequeueing elements (FIFO order)...\n");
    
    // Dequeue "Pranav" (first in)
    Value *v1 = queue_dequeue(queue);
    ASSERT(v1 != NULL && v1->type == TYPE_STRING && strcmp(v1->as.s, "Pranav") == 0, "First dequeue should be \"Pranav\"");
    printf("   Dequeued: ");
    value_print(v1);
    printf("\n");
    value_free(v1);
    
    // Dequeue 101
    Value *v2 = queue_dequeue(queue);
    ASSERT(v2 != NULL && v2->type == TYPE_INT && v2->as.i == 101, "Second dequeue should be 101");
    printf("   Dequeued: ");
    value_print(v2);
    printf("\n");
    value_free(v2);
    
    // Dequeue 'D'
    Value *v3 = queue_dequeue(queue);
    ASSERT(v3 != NULL && v3->type == TYPE_CHAR && v3->as.c == 'D', "Third dequeue should be 'D'");
    printf("   Dequeued: ");
    value_print(v3);
    printf("\n");
    value_free(v3);
    
    // Dequeue 26.37 (last in)
    Value *v4 = queue_dequeue(queue);
    ASSERT(v4 != NULL && v4->type == TYPE_DOUBLE, "Fourth dequeue should be TYPE_DOUBLE");
    double diff = v4->as.d - 26.37;
    if (diff < 0) diff = -diff;
    ASSERT(diff < 0.001, "Fourth dequeue should be 26.37");
    printf("   Dequeued: ");
    value_print(v4);
    printf("\n");
    value_free(v4);
    
    ASSERT(queue_size(queue) == 0, "Size should be 0 after all dequeues");
    ASSERT(queue_is_empty(queue) == 1, "Queue should be empty");
    printf("   Size after dequeues: %d\n\n", queue_size(queue));
    pass_count++;
    
    // 7. Check is_empty
    printf("7. Checking is_empty...\n");
    ASSERT(queue_is_empty(queue) == 1, "Queue should be empty");
    printf("   Is empty: YES\n\n");
    pass_count++;
    
    // 8. Dequeue from empty
    printf("8. Dequeue from empty queue...\n");
    Value *empty_dequeue = queue_dequeue(queue);
    ASSERT(empty_dequeue == NULL, "Dequeue from empty should return NULL");
    printf("   Dequeue from empty: NULL (correct)\n\n");
    pass_count++;
    
    // 9. Enqueue after clear
    printf("9. Enqueue after clear...\n");
    enqueue(queue, 999);
    ASSERT(queue_size(queue) == 1, "Size should be 1 after enqueue on cleared queue");
    printf("   After enqueue: ");
    queue_print(queue);
    
    // Enqueue more
    enqueue(queue, "Test");
    enqueue(queue, (char)'X');
    ASSERT(queue_size(queue) == 3, "Size should be 3");
    printf("   After more enqueues: ");
    queue_print(queue);
    pass_count++;
    
    // 10. Test wrap-around behavior
    printf("10. Testing wrap-around behavior...\n");
    // Dequeue 2 elements
    Value *w1 = queue_dequeue(queue);  // 999
    value_free(w1);
    Value *w2 = queue_dequeue(queue);  // "Test"
    value_free(w2);
    // Enqueue 3 more (will wrap around)
    enqueue(queue, 111);
    enqueue(queue, 222);
    enqueue(queue, 333);
    ASSERT(queue_size(queue) == 4, "Size should be 4 after wrap test");
    printf("    After wrap-around: ");
    queue_print(queue);
    // Dequeue all and verify order
    Value *wa1 = queue_dequeue(queue);  // 'X'
    ASSERT(wa1->type == TYPE_CHAR && wa1->as.c == 'X', "Wrap test: first should be 'X'");
    value_free(wa1);
    Value *wa2 = queue_dequeue(queue);  // 111
    ASSERT(wa2->type == TYPE_INT && wa2->as.i == 111, "Wrap test: second should be 111");
    value_free(wa2);
    Value *wa3 = queue_dequeue(queue);  // 222
    ASSERT(wa3->type == TYPE_INT && wa3->as.i == 222, "Wrap test: third should be 222");
    value_free(wa3);
    Value *wa4 = queue_dequeue(queue);  // 333
    ASSERT(wa4->type == TYPE_INT && wa4->as.i == 333, "Wrap test: fourth should be 333");
    value_free(wa4);
    printf("    Wrap-around test passed\n\n");
    pass_count++;
    
    // 11. Clear queue
    printf("11. Clearing queue...\n");
    // Enqueue some elements first
    enqueue(queue, 111);
    enqueue(queue, 222);
    queue_clear(queue);
    ASSERT(queue_size(queue) == 0, "Size should be 0 after clear");
    ASSERT(queue_is_empty(queue) == 1, "Queue should be empty after clear");
    printf("    After clear: ");
    queue_print(queue);
    pass_count++;
    
    // 12. Enqueue after clear
    printf("12. Enqueue after clear...\n");
    enqueue(queue, "After Clear");
    ASSERT(queue_size(queue) == 1, "Size should be 1 after enqueue on cleared queue");
    printf("    After enqueue: ");
    queue_print(queue);
    pass_count++;
    
    // 13. Destroy queue
    printf("13. Destroying queue...\n");
    queue_clear(queue);  // Free remaining value
    BRQueue_destroy(queue);
    printf("    Destroyed successfully\n\n");
    pass_count++;
    
    printf("=== Summary ===\n");
    printf("Passed: %d\n", pass_count);
    printf("Failed: %d\n", fail_count);
}

int main(void) {
    test_all_operations();
    return fail_count > 0 ? 1 : 0;
}
