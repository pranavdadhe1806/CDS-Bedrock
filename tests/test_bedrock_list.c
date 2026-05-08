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
    printf("=== Testing All LinkedList Operations ===\n\n");
    
    // 1. Create list
    printf("1. Creating list...\n");
    LinkedList *list = LinkedList_new();
    ASSERT(list != NULL, "Failed to create list");
    ASSERT(list_size(list) == 0, "New list should have size 0");
    printf("   Created successfully\n\n");
    pass_count++;
    
    // 2. Push back: int, double, char, string
    printf("2. Pushing back mixed types...\n");
    push_back(list, 101);
    push_back(list, 26.37);
    push_back(list, (char)'D');
    push_back(list, "Pranav");
    ASSERT(list_size(list) == 4, "Size should be 4 after 4 push_backs");
    printf("   After push_backs: ");
    list_print(list);
    pass_count++;
    
    // 3. Push front
    printf("3. Pushing front int...\n");
    push_front(list, 999);
    ASSERT(list_size(list) == 5, "Size should be 5 after push_front");
    printf("   After push_front: ");
    list_print(list);
    pass_count++;
    
    // 4. Peek front and back
    printf("4. Peeking front and back...\n");
    Value *front = peek_front(list);
    ASSERT(front != NULL && front->type == TYPE_INT && front->as.i == 999, "Front should be 999");
    printf("   Front: ");
    value_print(front);
    printf("\n");
    
    Value *back = peek_back(list);
    ASSERT(back != NULL && back->type == TYPE_STRING && strcmp(back->as.s, "Pranav") == 0, "Back should be \"Pranav\"");
    printf("   Back: ");
    value_print(back);
    printf("\n\n");
    pass_count++;
    
    // 5. Pop front
    printf("5. Popping front...\n");
    Value *popped_front = pop_front(list);
    ASSERT(popped_front != NULL && popped_front->type == TYPE_INT && popped_front->as.i == 999, "Popped front should be 999");
    ASSERT(list_size(list) == 4, "Size should be 4 after pop_front");
    printf("   Popped: ");
    value_print(popped_front);
    printf("\n   After pop_front: ");
    list_print(list);
    value_free(popped_front);
    pass_count++;
    
    // 6. Pop back
    printf("6. Popping back...\n");
    Value *popped_back = pop_back(list);
    ASSERT(popped_back != NULL && popped_back->type == TYPE_STRING && strcmp(popped_back->as.s, "Pranav") == 0, "Popped back should be \"Pranav\"");
    ASSERT(list_size(list) == 3, "Size should be 3 after pop_back");
    printf("   Popped: ");
    value_print(popped_back);
    printf("\n   After pop_back: ");
    list_print(list);
    value_free(popped_back);
    pass_count++;
    
    // 7. Get at each index
    printf("7. Getting each index...\n");
    Value *v0 = get_at(list, 0);
    ASSERT(v0 != NULL && v0->type == TYPE_INT && v0->as.i == 101, "Index 0 should be 101");
    
    Value *v1 = get_at(list, 1);
    ASSERT(v1 != NULL && v1->type == TYPE_DOUBLE, "Index 1 should be TYPE_DOUBLE");
    double diff = v1->as.d - 26.37;
    if (diff < 0) diff = -diff;
    ASSERT(diff < 0.001, "Index 1 should be approximately 26.37");
    
    Value *v2 = get_at(list, 2);
    ASSERT(v2 != NULL && v2->type == TYPE_CHAR && v2->as.c == 'D', "Index 2 should be 'D'");
    
    printf("   All indices correct\n\n");
    pass_count++;
    
    // 8. Insert at middle
    printf("8. Inserting at middle...\n");
    insert_at(list, 1, 555);
    ASSERT(list_size(list) == 4, "Size should be 4 after insert_at");
    printf("   After insert_at: ");
    list_print(list);
    pass_count++;
    
    // 9. Remove at middle
    printf("9. Removing at middle...\n");
    remove_at(list, 1);
    ASSERT(list_size(list) == 3, "Size should be 3 after remove_at");
    printf("   After remove_at: ");
    list_print(list);
    pass_count++;
    
    // 10. Update at
    printf("10. Updating at index 0...\n");
    update_at(list, 0, 202);
    Value *v_updated = get_at(list, 0);
    ASSERT(v_updated != NULL && v_updated->type == TYPE_INT && v_updated->as.i == 202, "Index 0 should now be 202");
    printf("   After update_at: ");
    list_print(list);
    pass_count++;
    
    // 11. Contains existing value
    printf("11. Checking contains...\n");
    int has_char = list_contains(list, (char)'D');
    ASSERT(has_char == 1, "Should contain 'D'");
    printf("   Contains 'D': YES\n");
    pass_count++;
    
    // 12. Contains missing value
    int has_missing = list_contains(list, 999);
    ASSERT(has_missing == 0, "Should not contain 999");
    printf("   Contains 999: NO\n\n");
    pass_count++;
    
    // 13. Size check
    printf("12. Checking size...\n");
    ASSERT(list_size(list) == 3, "Size should be 3");
    printf("   Size: %d\n\n", list_size(list));
    pass_count++;
    
    // 14. Reverse
    printf("13. Reversing list...\n");
    list_reverse(list);
    printf("   After reverse: ");
    list_print(list);
    
    // Verify order is reversed
    Value *r0 = get_at(list, 0);
    ASSERT(r0 != NULL && r0->type == TYPE_CHAR && r0->as.c == 'D', "After reverse, index 0 should be 'D'");
    
    Value *r2 = get_at(list, 2);
    ASSERT(r2 != NULL && r2->type == TYPE_INT && r2->as.i == 202, "After reverse, index 2 should be 202");
    pass_count++;
    
    // 15. Clear
    printf("14. Clearing list...\n");
    list_clear(list);
    ASSERT(list_size(list) == 0, "Size should be 0 after clear");
    printf("   After clear: ");
    list_print(list);
    pass_count++;
    
    // 16. Push after clear
    printf("15. Pushing after clear...\n");
    push_back(list, "Test");
    ASSERT(list_size(list) == 1, "Size should be 1 after push on cleared list");
    printf("   After push: ");
    list_print(list);
    pass_count++;
    
    // 17. Destroy
    printf("16. Destroying list...\n");
    list_clear(list);  // Clear remaining node first
    LinkedList_destroy(list);
    printf("   Destroyed successfully\n\n");
    pass_count++;
    
    printf("=== Summary ===\n");
    printf("Passed: %d\n", pass_count);
    printf("Failed: %d\n", fail_count);
}

int main(void) {
    test_all_operations();
    return fail_count > 0 ? 1 : 0;
}
