# CDS-Bedrock - WSL First Run Checklist
# 1. cd /mnt/c/Users/<you>/path/to/cds-bedrock
# 2. make clean
# 3. make all        - should compile with zero warnings
# 4. make test       - all tests should pass
# 5. make asan       - no memory errors reported
# 6. make valgrind   - 0 bytes lost in every test
# If any step fails, fix it before moving to the next

CC = gcc
CFLAGS = -std=c11 -Wall -Wextra -Wpedantic -g
INCLUDES = -Iinclude

SRCS = src/value.c src/bedrock_array.c src/bedrock_list.c \
       src/bedrock_stack.c src/bedrock_queue.c src/bedrock_bst.c \
       src/bedrock_hmap.c src/bedrock_heap.c src/bedrock_graph.c

TESTS = test_bedrock_array test_bedrock_list test_bedrock_stack \
        test_bedrock_queue test_stack test_queue test_bst \
        test_bedrock_hmap test_bedrock_heap test_bedrock_graph

TEST_BINS = $(addprefix tests/,$(TESTS))

.PHONY: all test asan valgrind clean

all: $(TEST_BINS)

tests/%: tests/%.c $(SRCS)
	$(CC) $(CFLAGS) $(INCLUDES) $< $(SRCS) -o $@

test: all
	@set -e; \
	for t in $(TEST_BINS); do \
		printf "Running %-28s" "$$t"; \
		if ./$$t > /tmp/cds-bedrock-test.log 2>&1; then \
			echo "PASS"; \
		else \
			echo "FAIL"; \
			cat /tmp/cds-bedrock-test.log; \
			exit 1; \
		fi; \
	done

asan: CFLAGS += -fsanitize=address -fsanitize=leak
asan: clean all test

valgrind: all
	valgrind --leak-check=full --error-exitcode=1 ./tests/test_bedrock_array
	valgrind --leak-check=full --error-exitcode=1 ./tests/test_bedrock_list
	valgrind --leak-check=full --error-exitcode=1 ./tests/test_bedrock_stack
	valgrind --leak-check=full --error-exitcode=1 ./tests/test_bedrock_queue
	valgrind --leak-check=full --error-exitcode=1 ./tests/test_bst
	valgrind --leak-check=full --error-exitcode=1 ./tests/test_bedrock_hmap
	valgrind --leak-check=full --error-exitcode=1 ./tests/test_bedrock_heap
	valgrind --leak-check=full --error-exitcode=1 ./tests/test_bedrock_graph

clean:
	rm -f $(TEST_BINS)
	rm -f tests/*.exe
	rm -rf build/asan
