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
|   |-- heap.h
|   |-- graph.h
|   `-- value.h
|-- src/
|   |-- value.c
|   |-- bedrock_array.c
|   |-- bedrock_list.c
|   |-- bedrock_stack.c
|   |-- bedrock_queue.c
|   |-- bedrock_bst.c
|   |-- bedrock_hmap.c
|   |-- heap.c
|   `-- graph.c
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
- [ ] Heap
- [ ] Graph

## License

MIT. See [LICENSE](LICENSE).
