# CDS — Bedrock

<p align="center">
  <img src="https://img.shields.io/badge/C-11-blue?style=flat-square&logo=c" alt="C11">
  <img src="https://img.shields.io/badge/License-MIT-green?style=flat-square" alt="License: MIT">
  <img src="https://img.shields.io/badge/Build-Make-orange?style=flat-square" alt="Build: Make">
  <img src="https://img.shields.io/badge/Platform-Linux%2FmacOS-lightgrey?style=flat-square" alt="Platform: Linux/macOS">
</p>

<p align="center"><em>Foundational data structures, built from scratch in C.</em></p>

Eight fundamental data structures implemented in C11 with `void*` generics and manual memory management. Every structure works with any pointer type, follows the same callback contract, and gives you full control over memory ownership. Built to understand what higher-level languages abstract away — not to wrap around them.

---

## Why Bedrock

Java and Python hide the interesting parts. You call `.add()` and something happens in memory — you just never see it. This project is the opposite of that. Every structure here is built with explicit `malloc`, `free`, pointer arithmetic, and zero safety nets. You learn why array access is O(1) and linked list traversal isn't, why a circular buffer beats a naive queue, and what a hash collision actually looks like in memory. The name is intentional — this is the layer everything else is built on.

---

## Data Structures

| Structure | Internal Impl | Key Operations | Time Complexity | Status |
|-----------|---------------|----------------|-----------------|--------|
| Dynamic Array | Contiguous `void**` buffer | push, pop, insert, remove, get, set | Access: O(1) / Push: O(1) amortized / Insert: O(n) | ⬜ |
| Linked List | Doubly-linked nodes | push_front, push_back, pop_front, pop_back, insert, remove | Push/Pop: O(1) / Access: O(n) / Search: O(n) | ⬜ |
| Stack | Dynamic array (LIFO) | push, pop, peek | All: O(1) | ⬜ |
| Queue | Circular buffer (FIFO) | enqueue, dequeue, peek | All: O(1) | ⬜ |
| Binary Search Tree | Ordered node pointers | insert, remove, search, find_min, find_max | Avg: O(log n) / Worst: O(n) | ⬜ |
| Hash Map | Open addressing, linear probing | put, get, remove, contains | Avg: O(1) / Worst: O(n) | ⬜ |
| Heap | Binary heap over array | insert, extract, peek, heapify | Insert/Extract: O(log n) / Peek: O(1) | ⬜ |
| Graph | Adjacency list | add_edge, remove_edge, bfs, dfs | Add Edge: O(1) / Traversal: O(V + E) | ⬜ |

---

## Design Principles

**Generics via void***

Every structure stores `void*` pointers. The same Dynamic Array works for `int*`, `char*`, or any struct — the library never needs to know what it's holding. Casting is the caller's responsibility.

```c
int x = 42;
dyn_array_push(arr, &x);
int *val = (int *)dyn_array_get(arr, 0);
```

**Callback Contract**

Each structure accepts up to three function pointers at creation time, covering comparison, debug printing, and cleanup. Not every structure needs all three — Stack and Queue skip the comparator, BST and Heap require it.

```c
int  (*cmp)     (const void *a, const void *b); // returns <0, 0, >0
void (*print_fn)(const void *data);             // debug printing
void (*free_fn) (void *data);                   // pass NULL to opt out
```

**Memory Ownership**

The library never copies your data — it stores the exact pointer you pass in. If you provide a `free_fn`, it gets called on every data pointer during `destroy` and `clear`. Pass `NULL` and you manage memory yourself.

```c
// Library frees your data on destroy
DynArray *arr = dyn_array_create(cmp, print_fn, free);
dyn_array_destroy(arr); // calls free() on every element

// You manage your own memory
DynArray *arr = dyn_array_create(cmp, print_fn, NULL);
dyn_array_destroy(arr); // only frees the array internals
```

---

## Project Structure

```
cds/
├── include/
│   ├── dyn_array.h
│   ├── linked_list.h
│   ├── stack.h
│   ├── queue.h
│   ├── bst.h
│   ├── hmap.h
│   ├── heap.h
│   └── graph.h
├── src/
│   ├── dyn_array.c
│   ├── linked_list.c
│   ├── stack.c
│   ├── queue.c
│   ├── bst.c
│   ├── hmap.c
│   ├── heap.c
│   └── graph.c
├── tests/
│   ├── test_dyn_array.c
│   ├── test_linked_list.c
│   ├── test_stack.c
│   ├── test_queue.c
│   ├── test_bst.c
│   ├── test_hmap.c
│   ├── test_heap.c
│   └── test_graph.c
├── examples/
│   └── example.c
├── Makefile
└── README.md
```

---

## Build & Run

Build everything:

```bash
make all
```

Run all tests (each binary runs independently — one failure won't block the rest):

```bash
make test
```

Run the example program:

```bash
./examples/example
```

Check for memory leaks (requires Valgrind):

```bash
make valgrind
```

Clean build artifacts:

```bash
make clean
```

---

## Progress

- [ ] Dynamic Array
- [ ] Linked List
- [ ] Stack
- [ ] Queue
- [ ] Binary Search Tree (BST)
- [ ] Hash Map
- [ ] Heap
- [ ] Graph

---

## Author

[pranavdadhe1806](https://github.com/pranavdadhe1806) · [CDS — Bedrock](https://github.com/pranavdadhe1806/CDS-Bedrock)
