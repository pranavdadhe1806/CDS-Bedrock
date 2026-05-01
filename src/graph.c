#include "../include/graph.h"
#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <assert.h>

/*
 * Graph (Adjacency List)
 * 
 * A graph represented using adjacency lists. Can be directed or undirected.
 * Supports weighted and unweighted graphs.
 * 
 * Time Complexities:
 * - Add Edge: O(1)
 * - Remove Edge: O(degree)
 * - Has Edge: O(degree)
 * - BFS/DFS: O(V + E)
 */

Graph* graph_create(int num_vertices, int directed,
                    void (*print_fn)(const void *weight),
                    void (*free_fn)(void *weight)) {
    (void)num_vertices;
    (void)directed;
    (void)print_fn;
    (void)free_fn;
    // TODO: implement
    return NULL;
}

void graph_destroy(Graph *graph) {
    (void)graph;
    // TODO: implement
}

int graph_add_edge(Graph *graph, int src, int dest, void *weight) {
    (void)graph;
    (void)src;
    (void)dest;
    (void)weight;
    // TODO: implement
    return 0;
}

int graph_remove_edge(Graph *graph, int src, int dest) {
    (void)graph;
    (void)src;
    (void)dest;
    // TODO: implement
    return 0;
}

int graph_has_edge(Graph *graph, int src, int dest) {
    (void)graph;
    (void)src;
    (void)dest;
    // TODO: implement
    return 0;
}

AdjNode* graph_get_adjacent(Graph *graph, int vertex) {
    (void)graph;
    (void)vertex;
    // TODO: implement
    return NULL;
}

int graph_num_vertices(Graph *graph) {
    (void)graph;
    // TODO: implement
    return 0;
}

int graph_num_edges(Graph *graph) {
    (void)graph;
    // TODO: implement
    return 0;
}

void graph_print(Graph *graph) {
    (void)graph;
    // TODO: implement
}

void graph_bfs(Graph *graph, int start_vertex, void (*visit)(int vertex)) {
    (void)graph;
    (void)start_vertex;
    (void)visit;
    // TODO: implement
}

void graph_dfs(Graph *graph, int start_vertex, void (*visit)(int vertex)) {
    (void)graph;
    (void)start_vertex;
    (void)visit;
    // TODO: implement
}
