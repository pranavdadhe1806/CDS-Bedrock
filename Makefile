CC = gcc
CFLAGS = -std=c11 -Wall -Wextra -Wpedantic -g
INCLUDE = -I include

# Source files
SRCS = src/dyn_array.c src/linked_list.c src/stack.c src/queue.c src/bst.c src/hmap.c src/heap.c src/graph.c
OBJS = $(SRCS:.c=.o)

# Test executables
TEST_BINS = tests/test_dyn_array tests/test_linked_list tests/test_stack tests/test_queue tests/test_bst tests/test_hmap tests/test_heap tests/test_graph

# Example executable
EXAMPLE_BIN = examples/example

.PHONY: all test clean

all: $(OBJS) $(EXAMPLE_BIN)

# Build object files
src/%.o: src/%.c
	$(CC) $(CFLAGS) $(INCLUDE) -c $< -o $@

# Build example
$(EXAMPLE_BIN): examples/example.c $(OBJS)
	$(CC) $(CFLAGS) $(INCLUDE) $^ -o $@

# Build and run all tests
test: $(TEST_BINS)
	@echo "Running all tests..."
	@./tests/test_dyn_array
	@./tests/test_linked_list
	@./tests/test_stack
	@./tests/test_queue
	@./tests/test_bst
	@./tests/test_hmap
	@./tests/test_heap
	@./tests/test_graph
	@echo "All tests completed."

# Build individual test binaries
tests/test_dyn_array: tests/test_dyn_array.c src/dyn_array.o
	$(CC) $(CFLAGS) $(INCLUDE) $^ -o $@

tests/test_linked_list: tests/test_linked_list.c src/linked_list.o
	$(CC) $(CFLAGS) $(INCLUDE) $^ -o $@

tests/test_stack: tests/test_stack.c src/stack.o
	$(CC) $(CFLAGS) $(INCLUDE) $^ -o $@

tests/test_queue: tests/test_queue.c src/queue.o
	$(CC) $(CFLAGS) $(INCLUDE) $^ -o $@

tests/test_bst: tests/test_bst.c src/bst.o
	$(CC) $(CFLAGS) $(INCLUDE) $^ -o $@

tests/test_hmap: tests/test_hmap.c src/hmap.o
	$(CC) $(CFLAGS) $(INCLUDE) $^ -o $@

tests/test_heap: tests/test_heap.c src/heap.o
	$(CC) $(CFLAGS) $(INCLUDE) $^ -o $@

tests/test_graph: tests/test_graph.c src/graph.o
	$(CC) $(CFLAGS) $(INCLUDE) $^ -o $@

clean:
	rm -f $(OBJS) $(TEST_BINS) $(EXAMPLE_BIN)
