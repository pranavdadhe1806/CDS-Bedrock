# CDS-Bedrock Makefile
#
# Usage:
#   make all      - compile all tests
#   make test     - compile and run all tests
#   make asan     - compile with AddressSanitizer and run tests (Linux/macOS)
#   make valgrind - run tests under Valgrind (Linux)
#   make clean    - remove compiled binaries

CC = gcc
CFLAGS = -std=c11 -Wall -Wextra -Wpedantic -g
INCLUDES = -Iinclude

SRCS = src/value.c src/bedrock_array.c src/bedrock_list.c \
       src/bedrock_stack.c src/bedrock_queue.c src/bedrock_bst.c \
       src/bedrock_hmap.c src/bedrock_heap.c src/bedrock_graph.c

TESTS = test_bedrock_array test_bedrock_list test_bedrock_stack \
        test_bedrock_queue test_bst \
        test_bedrock_hmap test_bedrock_heap test_bedrock_graph \
        test_edge_cases

TEST_BINS = $(addprefix tests/,$(TESTS))

.PHONY: all test asan valgrind clean help

all: $(TEST_BINS)

tests/%: tests/%.c $(SRCS)
	$(CC) $(CFLAGS) $(INCLUDES) $< $(SRCS) -o $@

test: all
	@set -e; \
	for t in $(TEST_BINS); do \
		printf "Running %-35s" "$$t"; \
		if ./$$t > /tmp/cds-bedrock-test.log 2>&1; then \
			echo "PASS"; \
		else \
			echo "FAIL"; \
			cat /tmp/cds-bedrock-test.log; \
			exit 1; \
		fi; \
	done

asan: CFLAGS += -fsanitize=address,undefined -fno-omit-frame-pointer
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
	valgrind --leak-check=full --error-exitcode=1 ./tests/test_edge_cases

clean:
	rm -f $(TEST_BINS)
	rm -f tests/*.exe
	rm -rf build/asan

help:
	@echo "Targets: all test asan valgrind clean help"
