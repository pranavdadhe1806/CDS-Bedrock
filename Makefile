CC = gcc
CFLAGS ?= -std=c11 -Wall -Wextra -Wpedantic -g
INCLUDE := -I include

LIB_SRCS := \
	src/value.c \
	src/bedrock_array.c \
	src/bedrock_list.c \
	src/bedrock_stack.c \
	src/bedrock_queue.c \
	src/bedrock_bst.c \
	src/bedrock_hmap.c \
	src/bedrock_heap.c \
	src/bedrock_graph.c

TESTS := \
	test_bedrock_array \
	test_bedrock_list \
	test_bedrock_stack \
	test_bedrock_queue \
	test_stack \
	test_queue \
	test_bst \
	test_bedrock_hmap \
	test_bedrock_heap \
	test_bedrock_graph

ASAN_DIR := build/asan
ASAN_FLAGS ?= -fsanitize=address
ASAN_BINS := $(addprefix $(ASAN_DIR)/,$(addsuffix .exe,$(TESTS)))

.PHONY: asan clean-asan

$(ASAN_DIR):
	powershell -NoProfile -Command "New-Item -ItemType Directory -Force '$(ASAN_DIR)' | Out-Null"

$(ASAN_DIR)/%.exe: tests/%.c $(LIB_SRCS) | $(ASAN_DIR)
	$(CC) $(CFLAGS) $(ASAN_FLAGS) $(INCLUDE) $< $(LIB_SRCS) -o $@

asan: $(ASAN_BINS)
	@echo Running AddressSanitizer tests...
	@echo NOTE: MinGW GCC commonly does not support -fsanitize=leak; this target uses $(ASAN_FLAGS).
	$(ASAN_DIR)/test_bedrock_array.exe
	$(ASAN_DIR)/test_bedrock_list.exe
	$(ASAN_DIR)/test_bedrock_stack.exe
	$(ASAN_DIR)/test_bedrock_queue.exe
	$(ASAN_DIR)/test_stack.exe
	$(ASAN_DIR)/test_queue.exe
	$(ASAN_DIR)/test_bst.exe
	$(ASAN_DIR)/test_bedrock_hmap.exe
	$(ASAN_DIR)/test_bedrock_heap.exe
	$(ASAN_DIR)/test_bedrock_graph.exe

clean-asan:
	powershell -NoProfile -Command "Remove-Item -Recurse -Force '$(ASAN_DIR)' -ErrorAction SilentlyContinue"
