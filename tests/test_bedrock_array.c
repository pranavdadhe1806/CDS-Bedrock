#include "../include/bedrock.h"
#include <stdio.h>
#include <string.h>
#include <assert.h>

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
    printf("=== Testing All BRArray Operations ===\n\n");
    
    // 1. Create array
    printf("1. Creating array...\n");
    BRArray *arr = BRArray_new();
    ASSERT(arr != NULL, "Failed to create array");
    ASSERT(size(arr) == 0, "New array should have size 0");
    printf("   Created successfully\n\n");
    pass_count++;
    
    // 2. Push int, double, char, string
    printf("2. Pushing mixed types...\n");
    push(arr, "Pranav");
    push(arr, 101);
    push(arr, (char)'D');
    push(arr, 26.37);
    ASSERT(size(arr) == 4, "Size should be 4 after 4 pushes");
    printf("   Pushed: \"Pranav\", 101, 'D', 26.37\n\n");
    pass_count++;
    
    // 3. Print and verify output
    printf("3. Printing array...\n");
    printf("   Output: ");
    print(arr);
    printf("\n");
    pass_count++;
    
    // 4. Get each index and verify
    printf("4. Getting each index...\n");
    Value *v0 = get(arr, 0);
    ASSERT(v0 != NULL && v0->type == TYPE_STRING && strcmp(v0->as.s, "Pranav") == 0, "Index 0 should be \"Pranav\"");
    
    Value *v1 = get(arr, 1);
    ASSERT(v1 != NULL && v1->type == TYPE_INT && v1->as.i == 101, "Index 1 should be 101");
    
    Value *v2 = get(arr, 2);
    ASSERT(v2 != NULL && v2->type == TYPE_CHAR && v2->as.c == 'D', "Index 2 should be 'D'");
    
    Value *v3 = get(arr, 3);
    ASSERT(v3 != NULL && v3->type == TYPE_DOUBLE, "Index 3 should be TYPE_DOUBLE");
    // Use tolerance for floating point comparison
    double diff = v3->as.d - 26.37;
    if (diff < 0) diff = -diff;
    ASSERT(diff < 0.001, "Index 3 should be approximately 26.37");
    
    printf("   All indices correct\n\n");
    pass_count++;
    
    // 5. Insert at middle
    printf("5. Inserting at middle...\n");
    insert(arr, 2, 999);
    ASSERT(size(arr) == 5, "Size should be 5 after insert");
    
    Value *v_insert = get(arr, 2);
    ASSERT(v_insert != NULL && v_insert->type == TYPE_INT && v_insert->as.i == 999, "Index 2 should now be 999");
    
    printf("   After insert: ");
    print(arr);
    printf("\n");
    pass_count++;
    
    // 6. Delete at middle
    printf("6. Deleting at middle...\n");
    delete(arr, 2);
    ASSERT(size(arr) == 4, "Size should be 4 after delete");
    
    v2 = get(arr, 2);
    ASSERT(v2 != NULL && v2->type == TYPE_CHAR && v2->as.c == 'D', "Index 2 should be 'D' again after delete");
    
    printf("   After delete: ");
    print(arr);
    printf("\n");
    pass_count++;
    
    // 7. Pop
    printf("7. Popping last element...\n");
    Value *popped = pop(arr);
    ASSERT(popped != NULL && popped->type == TYPE_DOUBLE, "Popped value should be TYPE_DOUBLE");
    double pop_diff = popped->as.d - 26.37;
    if (pop_diff < 0) pop_diff = -pop_diff;
    ASSERT(pop_diff < 0.001, "Popped value should be approximately 26.37");
    ASSERT(size(arr) == 3, "Size should be 3 after pop");
    
    printf("   Popped: ");
    value_print(popped);
    printf("\n   After pop: ");
    print(arr);
    printf("\n");
    
    value_free(popped);
    pass_count++;
    
    // 8. Update index
    printf("8. Updating index 1...\n");
    update(arr, 1, 202);
    
    Value *v_updated = get(arr, 1);
    ASSERT(v_updated != NULL && v_updated->type == TYPE_INT && v_updated->as.i == 202, "Index 1 should now be 202");
    
    printf("   After update: ");
    print(arr);
    printf("\n");
    pass_count++;
    
    // 9. Contains existing value
    printf("9. Checking contains...\n");
    int has_pranav = contains(arr, "Pranav");
    ASSERT(has_pranav == 1, "Should contain \"Pranav\"");
    printf("   Contains \"Pranav\": YES\n");
    pass_count++;
    
    // 10. Contains missing value
    int has_missing = contains(arr, "Missing");
    ASSERT(has_missing == 0, "Should not contain \"Missing\"");
    printf("   Contains \"Missing\": NO\n\n");
    pass_count++;
    
    // 11. Size
    printf("10. Checking size...\n");
    ASSERT(size(arr) == 3, "Size should be 3");
    printf("   Size: %d\n\n", size(arr));
    pass_count++;
    
    // 12. Clear
    printf("11. Clearing array...\n");
    clear(arr);
    ASSERT(size(arr) == 0, "Size should be 0 after clear");
    printf("   After clear: ");
    print(arr);
    printf("\n");
    pass_count++;
    
    // 13. Destroy
    printf("12. Destroying array...\n");
    BRArray_destroy(arr);
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
