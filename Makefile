CC = gcc
CFLAGS = -std=c11 -Wall -Wextra -Wpedantic -g
INCLUDE = -I include

# Source files
SRCS = src/value.c src/bedrock_array.c src/bedrock_list.c src/bedrock_stack.c src/bedrock_queue.c src/bedrock_bst.c src/hmap.c src/heap.c src/graph.c
OBJS = $(SRCS:.c=.o)

# Test executables
TEST_BINS = tests/test_bedrock_array tests/test_bedrock_list tests/test_bedrock_stack tests/test_bedrock_queue tests/test_stack tests/test_queue tests/test_bst tests/test_hmap tests/test_heap tests/test_graph

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
	@./tests/test_bedrock_array
	@./tests/test_bedrock_list
	@./tests/test_bedrock_stack
	@./tests/test_bedrock_queue
	@./tests/test_stack
	@./tests/test_queue
	@./tests/test_bst
	@./tests/test_hmap
	@./tests/test_heap
	@./tests/test_graph
	@echo "All tests completed."

# Build individual test binaries
tests/test_bedrock_array: tests/test_bedrock_array.c src/value.o src/bedrock_array.o
	$(CC) $(CFLAGS) $(INCLUDE) $^ -o $@

tests/test_bedrock_list: tests/test_bedrock_list.c src/value.o src/bedrock_list.o
	$(CC) $(CFLAGS) $(INCLUDE) $^ -o $@

tests/test_bedrock_stack: tests/test_bedrock_stack.c src/value.o src/bedrock_stack.o
	$(CC) $(CFLAGS) $(INCLUDE) $^ -o $@

tests/test_bedrock_queue: tests/test_bedrock_queue.c src/value.o src/bedrock_queue.o
	$(CC) $(CFLAGS) $(INCLUDE) $^ -o $@

tests/test_stack: tests/test_stack.c src/bedrock_stack.o
	$(CC) $(CFLAGS) $(INCLUDE) $^ -o $@

tests/test_queue: tests/test_queue.c src/bedrock_queue.o
	$(CC) $(CFLAGS) $(INCLUDE) $^ -o $@

tests/test_bst: tests/test_bst.c src/value.o src/bedrock_bst.o
	$(CC) $(CFLAGS) $(INCLUDE) $^ -o $@

tests/test_hmap: tests/test_hmap.c src/hmap.o
	$(CC) $(CFLAGS) $(INCLUDE) $^ -o $@

tests/test_heap: tests/test_heap.c src/heap.o
	$(CC) $(CFLAGS) $(INCLUDE) $^ -o $@

tests/test_graph: tests/test_graph.c src/graph.o
	$(CC) $(CFLAGS) $(INCLUDE) $^ -o $@

clean:
	rm -f $(OBJS) $(TEST_BINS) $(EXAMPLE_BIN)
