# CDS-Bedrock

Foundational data structures built from scratch in C11.

![C11](https://img.shields.io/badge/C-11-blue?style=flat-square&logo=c)
![License: MIT](https://img.shields.io/badge/License-MIT-green?style=flat-square)
![Build: CMake](https://img.shields.io/badge/Build-CMake-orange?style=flat-square)

CDS-Bedrock is a C11 data structures library with explicit memory management and a small public API. It is packaged with CMake for local builds, GitHub release artifacts, and vcpkg integration.

## Data Structures

| Structure | Internal Implementation | Key Operations | Complexity |
|-----------|--------------------------|----------------|------------|
| Dynamic Array | Contiguous buffer | push, pop, insert, delete, get, update | Access: O(1), Insert/Delete: O(n) |
| Linked List | Doubly linked nodes | push_front, push_back, pop, remove, reverse | Push/Pop: O(1), Search: O(n) |
| Stack | Dynamic array | push, pop, peek | O(1) |
| Queue | Circular buffer | enqueue, dequeue, peek | O(1) |
| Binary Search Tree | Ordered nodes | insert, remove, search, traversals | Avg: O(log n), Worst: O(n) |
| Hash Map | Open addressing, linear probing | put, get, remove, contains | Avg: O(1), Worst: O(n) |
| Heap | Binary heap over array | insert, extract, peek, heapify | Insert/Extract: O(log n) |
| Graph | Adjacency list | add_edge, remove_edge, bfs, dfs | Traversal: O(V + E) |

## API and Memory Ownership

CDS-Bedrock provides a unified, type-safe API using C11 `_Generic` macros (found in `bedrock.h`). You do not need to manually wrap native types; the macros automatically dispatch to the correct function.

```c
BRArray *arr = BRArray_new();
// _Generic dispatch automatically handles int, double, char, and const char*
insert(arr, 0, 42);
push(arr, 3.14);
push(arr, "Hello World");
```

### Memory Ownership Rules

CDS-Bedrock enforces strict memory ownership boundaries to prevent memory leaks and dangling pointers:

1. **Insertion (Takes Ownership)**: When you insert data (`push`, `enqueue`, `insert`, `put`), the container dynamically allocates a `Value` wrapper and takes ownership of it. If you insert a string, a copy of the string is duplicated onto the heap.
2. **Access (Borrowed Pointers)**: Operations that read data without removing it (`get`, `peek`, `find_min`, `hmap_get`) return **borrowed pointers**. You can read from these pointers, but you **MUST NOT** free them.
3. **Removal (Transfers Ownership)**: Operations that remove and return data (`pop`, `dequeue`, `extract`) transfer ownership to the caller. You **MUST** call `value_free()` on the returned pointer when you are done.
   ```c
   Value *val = pop(arr);
   if (val) {
       printf("Popped: "); value_print(val);
       value_free(val); // Caller is responsible for freeing
   }
   ```
4. **Destruction (Cascading Free)**: When a container is destroyed (e.g., `BRArray_destroy(arr)`), it automatically frees all remaining items it owns.

### AVL Tree and FNV-1a Hashing

- The Binary Search Tree (`BRBST`) is implemented as an **AVL Tree**, guaranteeing O(log n) performance by automatically performing tree rotations during insertions and removals.
- The Hash Map (`HMap`) utilizes the **FNV-1a 32-bit** hashing algorithm for excellent distribution and collision resistance across all supported types.

## Project Structure

```text
cds-bedrock/
|-- include/
|   |-- bedrock.h
|   |-- bedrock_array.h
|   |-- bedrock_list.h
|   |-- bedrock_stack.h
|   |-- bedrock_queue.h
|   |-- bedrock_bst.h
|   |-- bedrock_hmap.h
|   |-- bedrock_heap.h
|   |-- bedrock_graph.h
|   `-- value.h
|-- src/
|   |-- value.c
|   |-- bedrock_array.c
|   |-- bedrock_list.c
|   |-- bedrock_stack.c
|   |-- bedrock_queue.c
|   |-- bedrock_bst.c
|   |-- bedrock_hmap.c
|   |-- bedrock_heap.c
|   `-- bedrock_graph.c
|-- tests/
|-- examples/
|-- cmake/
|-- ports/cds-bedrock/
|-- CMakeLists.txt
|-- CMakePresets.json
|-- LICENSE
`-- README.md
```

## Build And Test

Configure:

```bash
cmake -S . -B build
```

Build:

```bash
cmake --build build
```

Run tests:

```bash
ctest --test-dir build --output-on-failure
```

Install locally:

```bash
cmake --install build --prefix install
```

Preset workflow:

```bash
cmake --preset default
cmake --build --preset default
ctest --preset default
```

## CMake Consumption

After installing the package, consume it from another CMake project with:

```cmake
find_package(cds-bedrock CONFIG REQUIRED)
target_link_libraries(my_app PRIVATE cds-bedrock::cds-bedrock)
```

## vcpkg Overlay Port

This repository includes an overlay port under `ports/cds-bedrock`.

```bash
vcpkg install cds-bedrock --overlay-ports=ports
```

## Progress

- [x] Dynamic Array
- [x] Linked List
- [x] Stack
- [x] Queue
- [x] Binary Search Tree
- [x] Hash Map
- [x] Heap
- [x] Graph

## License

MIT. See [LICENSE](LICENSE).
