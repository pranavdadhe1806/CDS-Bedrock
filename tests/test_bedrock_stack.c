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
    printf("=== Testing All BRStack Operations ===\n\n");
    
    // 1. Create stack
    printf("1. Creating stack...\n");
    BRStack *stack = BRStack_new();
    ASSERT(stack != NULL, "Failed to create stack");
    ASSERT(stack_size(stack) == 0, "New stack should have size 0");
    ASSERT(stack_is_empty(stack) == 1, "New stack should be empty");
    printf("   Created successfully\n\n");
    pass_count++;
    
    // 2. Push mixed types
    printf("2. Pushing mixed types...\n");
    stack_push(stack, "Pranav");
    stack_push(stack, 101);
    stack_push(stack, (char)'D');
    stack_push(stack, 26.37);
    ASSERT(stack_size(stack) == 4, "Size should be 4 after 4 pushes");
    printf("   Pushed: \"Pranav\", 101, 'D', 26.37\n\n");
    pass_count++;
    
    // 3. Print stack
    printf("3. Printing stack...\n   ");
    stack_print(stack);
    pass_count++;
    
    // 4. Peek top
    printf("4. Peeking top...\n");
    Value *top = stack_peek(stack);
    ASSERT(top != NULL && top->type == TYPE_DOUBLE, "Top should be TYPE_DOUBLE");
    double diff = top->as.d - 26.37;
    if (diff < 0) diff = -diff;
    ASSERT(diff < 0.001, "Top should be approximately 26.37");
    printf("   Top: ");
    value_print(top);
    printf("\n");
    ASSERT(stack_size(stack) == 4, "Size should still be 4 after peek");
    printf("   Size after peek: %d\n\n", stack_size(stack));
    pass_count++;
    
    // 5. Check size
    printf("5. Checking size...\n");
    ASSERT(stack_size(stack) == 4, "Size should be 4");
    printf("   Size: %d\n\n", stack_size(stack));
    pass_count++;
    
    // 6. Pop elements (LIFO order)
    printf("6. Popping elements (LIFO order)...\n");
    
    // Pop double (26.37)
    Value *v1 = stack_pop(stack);
    ASSERT(v1 != NULL && v1->type == TYPE_DOUBLE, "First pop should be TYPE_DOUBLE");
    double d1 = v1->as.d - 26.37;
    if (d1 < 0) d1 = -d1;
    ASSERT(d1 < 0.001, "First pop should be 26.37");
    printf("   Popped: ");
    value_print(v1);
    printf("\n");
    value_free(v1);
    
    // Pop char ('D')
    Value *v2 = stack_pop(stack);
    ASSERT(v2 != NULL && v2->type == TYPE_CHAR && v2->as.c == 'D', "Second pop should be 'D'");
    printf("   Popped: ");
    value_print(v2);
    printf("\n");
    value_free(v2);
    
    // Pop int (101)
    Value *v3 = stack_pop(stack);
    ASSERT(v3 != NULL && v3->type == TYPE_INT && v3->as.i == 101, "Third pop should be 101");
    printf("   Popped: ");
    value_print(v3);
    printf("\n");
    value_free(v3);
    
    // Pop string ("Pranav")
    Value *v4 = stack_pop(stack);
    ASSERT(v4 != NULL && v4->type == TYPE_STRING && strcmp(v4->as.s, "Pranav") == 0, "Fourth pop should be \"Pranav\"");
    printf("   Popped: ");
    value_print(v4);
    printf("\n");
    value_free(v4);
    
    ASSERT(stack_size(stack) == 0, "Size should be 0 after all pops");
    ASSERT(stack_is_empty(stack) == 1, "Stack should be empty");
    printf("   Size after pops: %d\n\n", stack_size(stack));
    pass_count++;
    
    // 7. Check is_empty
    printf("7. Checking is_empty...\n");
    ASSERT(stack_is_empty(stack) == 1, "Stack should be empty");
    printf("   Is empty: YES\n\n");
    pass_count++;
    
    // 8. Pop from empty stack
    printf("8. Popping from empty stack...\n");
    Value *empty_pop = stack_pop(stack);
    ASSERT(empty_pop == NULL, "Pop from empty should return NULL");
    printf("   Pop from empty: NULL (correct)\n\n");
    pass_count++;
    
    // 9. Push after clear
    printf("9. Pushing after clear...\n");
    stack_push(stack, 42);
    ASSERT(stack_size(stack) == 1, "Size should be 1 after push on cleared stack");
    printf("   After push: ");
    stack_print(stack);
    
    // Push more
    stack_push(stack, "Test");
    stack_push(stack, (char)'X');
    ASSERT(stack_size(stack) == 3, "Size should be 3");
    printf("   After more pushes: ");
    stack_print(stack);
    pass_count++;
    
    // 10. Clear stack
    printf("10. Clearing stack...\n");
    stack_clear(stack);
    ASSERT(stack_size(stack) == 0, "Size should be 0 after clear");
    ASSERT(stack_is_empty(stack) == 1, "Stack should be empty after clear");
    printf("    After clear: ");
    stack_print(stack);
    pass_count++;
    
    // 11. Push after clear
    printf("11. Pushing after clear...\n");
    stack_push(stack, "After Clear");
    ASSERT(stack_size(stack) == 1, "Size should be 1 after push on cleared stack");
    printf("    After push: ");
    stack_print(stack);
    pass_count++;
    
    // 12. Destroy stack
    printf("12. Destroying stack...\n");
    stack_clear(stack);  // Free remaining value
    BRStack_destroy(stack);
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
