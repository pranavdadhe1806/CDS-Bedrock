#ifndef BEDROCK_GRAPH_H
#define BEDROCK_GRAPH_H

typedef struct AdjNode {
    int             vertex;
    void           *weight;
    struct AdjNode *next;
} AdjNode;

typedef struct {
    AdjNode **adj_lists;
    int       num_vertices;
    int       directed;
    void    (*print_fn)(const void *weight);
    void    (*free_fn)(void *weight);
} Graph;

Graph   *graph_create(int num_vertices, int directed,
                      void (*print_fn)(const void *weight),
                      void (*free_fn)(void *weight));
void     graph_destroy(Graph *graph);
int      graph_add_edge(Graph *graph, int src, int dest, void *weight);
int      graph_remove_edge(Graph *graph, int src, int dest);
int      graph_has_edge(Graph *graph, int src, int dest);
AdjNode *graph_get_adjacent(Graph *graph, int vertex);
int      graph_num_vertices(Graph *graph);
int      graph_num_edges(Graph *graph);
void     graph_print(Graph *graph);
void     graph_bfs(Graph *graph, int start_vertex, void (*visit)(int vertex));
void     graph_dfs(Graph *graph, int start_vertex, void (*visit)(int vertex));

#endif /* BEDROCK_GRAPH_H */
