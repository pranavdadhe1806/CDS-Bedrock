#include "../include/bedrock_graph.h"
#include <stdio.h>
#include <stdlib.h>

static int _valid_vertex(Graph *graph, int vertex) {
    return graph != NULL && vertex >= 0 && vertex < graph->num_vertices;
}

static AdjNode *_create_adj_node(int vertex, void *weight) {
    AdjNode *node = malloc(sizeof(AdjNode));
    if (node == NULL) return NULL;

    node->vertex = vertex;
    node->weight = weight;
    node->next = NULL;
    return node;
}

static int _add_directed_edge(Graph *graph, int src, int dest, void *weight) {
    AdjNode *node = _create_adj_node(dest, weight);
    if (node == NULL) return 0;

    node->next = graph->adj_lists[src];
    graph->adj_lists[src] = node;
    return 1;
}

static int _remove_directed_edge(Graph *graph, int src, int dest, int free_weight) {
    AdjNode *current = graph->adj_lists[src];
    AdjNode *prev = NULL;

    while (current != NULL) {
        if (current->vertex == dest) {
            if (prev == NULL) {
                graph->adj_lists[src] = current->next;
            } else {
                prev->next = current->next;
            }

            if (free_weight && graph->free_fn != NULL) {
                graph->free_fn(current->weight);
            }
            free(current);
            return 1;
        }

        prev = current;
        current = current->next;
    }

    return 0;
}

static void _dfs_visit(Graph *graph, int vertex, int *visited, void (*visit)(int vertex)) {
    visited[vertex] = 1;
    if (visit != NULL) visit(vertex);

    AdjNode *current = graph->adj_lists[vertex];
    while (current != NULL) {
        if (!visited[current->vertex]) {
            _dfs_visit(graph, current->vertex, visited, visit);
        }
        current = current->next;
    }
}

Graph *graph_create(int num_vertices, int directed,
                    void (*print_fn)(const void *weight),
                    void (*free_fn)(void *weight)) {
    if (num_vertices <= 0) return NULL;

    Graph *graph = malloc(sizeof(Graph));
    if (graph == NULL) return NULL;

    graph->adj_lists = calloc((size_t)num_vertices, sizeof(AdjNode *));
    if (graph->adj_lists == NULL) {
        free(graph);
        return NULL;
    }

    graph->num_vertices = num_vertices;
    graph->directed = directed ? 1 : 0;
    graph->print_fn = print_fn;
    graph->free_fn = free_fn;
    return graph;
}

void graph_destroy(Graph *graph) {
    if (graph == NULL) return;

    for (int i = 0; i < graph->num_vertices; i++) {
        AdjNode *current = graph->adj_lists[i];
        while (current != NULL) {
            AdjNode *next = current->next;
            if (graph->free_fn != NULL) {
                graph->free_fn(current->weight);
            }
            free(current);
            current = next;
        }
    }

    free(graph->adj_lists);
    free(graph);
}

int graph_add_edge(Graph *graph, int src, int dest, void *weight) {
    if (!_valid_vertex(graph, src) || !_valid_vertex(graph, dest)) return 0;

    if (!_add_directed_edge(graph, src, dest, weight)) return 0;

    if (!graph->directed) {
        if (!_add_directed_edge(graph, dest, src, NULL)) {
            _remove_directed_edge(graph, src, dest, 1);
            return 0;
        }
    }

    return 1;
}

int graph_remove_edge(Graph *graph, int src, int dest) {
    if (!_valid_vertex(graph, src) || !_valid_vertex(graph, dest)) return 0;

    int removed = _remove_directed_edge(graph, src, dest, 1);
    if (!graph->directed) {
        _remove_directed_edge(graph, dest, src, 1);
    }

    return removed;
}

int graph_has_edge(Graph *graph, int src, int dest) {
    if (!_valid_vertex(graph, src) || !_valid_vertex(graph, dest)) return 0;

    AdjNode *current = graph->adj_lists[src];
    while (current != NULL) {
        if (current->vertex == dest) return 1;
        current = current->next;
    }

    return 0;
}

AdjNode *graph_get_adjacent(Graph *graph, int vertex) {
    if (!_valid_vertex(graph, vertex)) return NULL;
    return graph->adj_lists[vertex];
}

int graph_num_vertices(Graph *graph) {
    if (graph == NULL) return 0;
    return graph->num_vertices;
}

int graph_num_edges(Graph *graph) {
    if (graph == NULL) return 0;

    int count = 0;
    for (int i = 0; i < graph->num_vertices; i++) {
        AdjNode *current = graph->adj_lists[i];
        while (current != NULL) {
            count++;
            current = current->next;
        }
    }

    return graph->directed ? count : count / 2;
}

void graph_print(Graph *graph) {
    if (graph == NULL) {
        printf("<null graph>\n");
        return;
    }

    for (int i = 0; i < graph->num_vertices; i++) {
        printf("%d:", i);
        AdjNode *current = graph->adj_lists[i];
        while (current != NULL) {
            printf(" %d", current->vertex);
            if (current->weight != NULL && graph->print_fn != NULL) {
                printf("(");
                graph->print_fn(current->weight);
                printf(")");
            }
            current = current->next;
        }
        printf("\n");
    }
}

void graph_bfs(Graph *graph, int start_vertex, void (*visit)(int vertex)) {
    if (!_valid_vertex(graph, start_vertex)) return;

    int *visited = calloc((size_t)graph->num_vertices, sizeof(int));
    int *queue = malloc((size_t)graph->num_vertices * sizeof(int));
    if (visited == NULL || queue == NULL) {
        free(visited);
        free(queue);
        return;
    }

    int head = 0;
    int tail = 0;
    visited[start_vertex] = 1;
    queue[tail++] = start_vertex;

    while (head < tail) {
        int vertex = queue[head++];
        if (visit != NULL) visit(vertex);

        AdjNode *current = graph->adj_lists[vertex];
        while (current != NULL) {
            if (!visited[current->vertex]) {
                visited[current->vertex] = 1;
                queue[tail++] = current->vertex;
            }
            current = current->next;
        }
    }

    free(queue);
    free(visited);
}

void graph_dfs(Graph *graph, int start_vertex, void (*visit)(int vertex)) {
    if (!_valid_vertex(graph, start_vertex)) return;

    int *visited = calloc((size_t)graph->num_vertices, sizeof(int));
    if (visited == NULL) return;

    _dfs_visit(graph, start_vertex, visited, visit);
    free(visited);
}
