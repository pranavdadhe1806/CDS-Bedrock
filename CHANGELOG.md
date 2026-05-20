# Changelog

All notable changes to CDS-Bedrock will be documented in this file.

The format is based on [Keep a Changelog](https://keepachangelog.com/en/1.0.0/)
and this project adheres to [Semantic Versioning](https://semver.org/spec/v2.0.0.html).

---

## [v0.1.0] - 2026-05-21

### Added
- Dynamic Array with automatic capacity doubling and integer overflow guards
- Doubly Linked List with O(1) push/pop at both ends and O(n) reversal
- Stack backed by a dynamic array with O(1) push/pop/peek
- Queue implemented as a circular buffer with O(1) enqueue/dequeue
- AVL Tree (self-balancing BST) with automatic Left-Left, Right-Right, Left-Right, Right-Left rotations
- Hash Map using open addressing with linear probing and FNV-1a 32-bit hashing
- Min/Max Heap backed by a dynamic array with O(log n) insert/extract and O(n) bulk heapify
- Graph with adjacency list representation, BFS, and iterative DFS
- Unified `_Generic` macro API in `bedrock.h` for type-safe, zero-boilerplate usage
- Strict memory ownership rules (OWNED vs BORROWED semantics) on all containers
- `const` correctness on all read-only accessors library-wide
- Tombstone-aware load factor check to prevent probe-chain degradation in HMap
- Iterative BST destroy to prevent stack overflow on deep trees
- AddressSanitizer CMake preset (`asan`) integrating `-fsanitize=address,undefined`
- 152 unit and edge-case tests across all data structures
- CMake install targets and `find_package(cds-bedrock CONFIG REQUIRED)` downstream support
- vcpkg overlay port under `ports/cds-bedrock`
- GitHub Actions CI: cross-platform build matrix (Linux, Windows, macOS), ASan workflow, and release pipeline

[v0.1.0]: https://github.com/pranavdadhe1806/CDS-Bedrock/releases/tag/v0.1.0
