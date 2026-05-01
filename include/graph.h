#ifndef GRAPH_H
#define GRAPH_H

#include <stddef.h>
#include <stdint.h>

typedef struct AdjNode {
    int            vertex;
    void          *weight;        // NULL for unweighted graphs
    struct AdjNode *next;
} AdjNode;

typedef struct {
    AdjNode **adj_lists;          // array of adjacency lists, one per vertex
    int       num_vertices;
    int       directed;           // 1 = directed, 0 = undirected
    void    (*print_fn)(const void *weight);
    void    (*free_fn)(void *weight);
} Graph;

// Create a new graph with specified number of vertices
Graph* graph_create(int num_vertices, int directed,
                    void (*print_fn)(const void *weight),
                    void (*free_fn)(void *weight));

// Destroy the graph and free all memory
void graph_destroy(Graph *graph);

// Add an edge between two vertices
int graph_add_edge(Graph *graph, int src, int dest, void *weight);

// Remove an edge between two vertices
int graph_remove_edge(Graph *graph, int src, int dest);

// Check if an edge exists between two vertices
int graph_has_edge(Graph *graph, int src, int dest);

// Get the adjacency list for a vertex
AdjNode* graph_get_adjacent(Graph *graph, int vertex);

// Get the number of vertices in the graph
int graph_num_vertices(Graph *graph);

// Get the number of edges in the graph
int graph_num_edges(Graph *graph);

// Print the graph structure
void graph_print(Graph *graph);

// Perform BFS traversal starting from a vertex
void graph_bfs(Graph *graph, int start_vertex, void (*visit)(int vertex));

// Perform DFS traversal starting from a vertex
void graph_dfs(Graph *graph, int start_vertex, void (*visit)(int vertex));

#endif // GRAPH_H
