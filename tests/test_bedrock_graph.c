#include "../include/bedrock_graph.h"
#include <stdio.h>
#include <stdlib.h>

static int pass_count = 0;
static int fail_count = 0;
static int visit_order[16];
static int visit_count = 0;

#define TEST(name) printf("  %-32s", name)
#define PASS() do { printf("PASS\n"); pass_count++; } while (0)
#define FAIL(msg) do { printf("FAIL - %s\n", msg); fail_count++; return; } while (0)
#define ASSERT(cond, msg) do { if (!(cond)) { FAIL(msg); } } while (0)

static void record_visit(int vertex) {
    if (visit_count < 16) {
        visit_order[visit_count++] = vertex;
    }
}

static void reset_visits(void) {
    visit_count = 0;
    for (int i = 0; i < 16; i++) {
        visit_order[i] = -1;
    }
}

static int *make_weight(int value) {
    int *weight = malloc(sizeof(int));
    if (weight != NULL) *weight = value;
    return weight;
}

void test_graph_create(void) {
    TEST("graph_create");
    Graph *graph = graph_create(4, 1, NULL, NULL);
    ASSERT(graph != NULL, "graph_create returned NULL");
    ASSERT(graph_num_vertices(graph) == 4, "vertex count should be 4");
    ASSERT(graph_num_edges(graph) == 0, "new graph should have no edges");
    graph_destroy(graph);
    PASS();
}

void test_graph_add_edge_directed(void) {
    TEST("graph_add_edge_directed");
    Graph *graph = graph_create(3, 1, NULL, NULL);
    ASSERT(graph != NULL, "graph_create returned NULL");
    ASSERT(graph_add_edge(graph, 0, 1, NULL) == 1, "add edge should succeed");
    ASSERT(graph_has_edge(graph, 0, 1) == 1, "directed edge 0->1 should exist");
    ASSERT(graph_has_edge(graph, 1, 0) == 0, "reverse edge should not exist");
    ASSERT(graph_num_edges(graph) == 1, "edge count should be 1");
    graph_destroy(graph);
    PASS();
}

void test_graph_add_edge_undirected(void) {
    TEST("graph_add_edge_undirected");
    Graph *graph = graph_create(3, 0, NULL, NULL);
    ASSERT(graph != NULL, "graph_create returned NULL");
    ASSERT(graph_add_edge(graph, 0, 1, NULL) == 1, "add edge should succeed");
    ASSERT(graph_has_edge(graph, 0, 1) == 1, "edge 0->1 should exist");
    ASSERT(graph_has_edge(graph, 1, 0) == 1, "edge 1->0 should exist");
    ASSERT(graph_num_edges(graph) == 1, "undirected edge count should be 1");
    graph_destroy(graph);
    PASS();
}

void test_graph_remove_edge(void) {
    TEST("graph_remove_edge");
    Graph *graph = graph_create(3, 0, NULL, NULL);
    ASSERT(graph != NULL, "graph_create returned NULL");
    graph_add_edge(graph, 0, 1, NULL);
    ASSERT(graph_remove_edge(graph, 0, 1) == 1, "remove should find edge");
    ASSERT(graph_has_edge(graph, 0, 1) == 0, "forward edge should be gone");
    ASSERT(graph_has_edge(graph, 1, 0) == 0, "reverse edge should be gone");
    ASSERT(graph_remove_edge(graph, 0, 2) == 0, "missing remove should return 0");
    graph_destroy(graph);
    PASS();
}

void test_graph_get_adjacent(void) {
    TEST("graph_get_adjacent");
    Graph *graph = graph_create(3, 1, NULL, NULL);
    ASSERT(graph != NULL, "graph_create returned NULL");
    graph_add_edge(graph, 0, 2, NULL);
    AdjNode *adj = graph_get_adjacent(graph, 0);
    ASSERT(adj != NULL && adj->vertex == 2, "adjacency should contain vertex 2");
    ASSERT(graph_get_adjacent(graph, 99) == NULL, "invalid vertex should return NULL");
    graph_destroy(graph);
    PASS();
}

void test_graph_bfs_fifo(void) {
    TEST("graph_bfs_fifo");
    Graph *graph = graph_create(5, 1, NULL, NULL);
    ASSERT(graph != NULL, "graph_create returned NULL");
    graph_add_edge(graph, 0, 2, NULL);
    graph_add_edge(graph, 0, 1, NULL);
    graph_add_edge(graph, 1, 3, NULL);
    graph_add_edge(graph, 2, 4, NULL);

    reset_visits();
    graph_bfs(graph, 0, record_visit);

    ASSERT(visit_count == 5, "BFS should visit 5 vertices");
    ASSERT(visit_order[0] == 0, "BFS should start at 0");
    ASSERT(visit_order[1] == 1 && visit_order[2] == 2, "BFS should visit level one before deeper nodes");
    graph_destroy(graph);
    PASS();
}

void test_graph_dfs(void) {
    TEST("graph_dfs");
    Graph *graph = graph_create(4, 1, NULL, NULL);
    ASSERT(graph != NULL, "graph_create returned NULL");
    graph_add_edge(graph, 0, 1, NULL);
    graph_add_edge(graph, 1, 2, NULL);
    graph_add_edge(graph, 2, 3, NULL);

    reset_visits();
    graph_dfs(graph, 0, record_visit);

    ASSERT(visit_count == 4, "DFS should visit 4 vertices");
    ASSERT(visit_order[0] == 0, "DFS should start at 0");
    graph_destroy(graph);
    PASS();
}

void test_graph_destroy_weighted(void) {
    TEST("graph_destroy_weighted");
    Graph *graph = graph_create(3, 0, NULL, free);
    ASSERT(graph != NULL, "graph_create returned NULL");
    ASSERT(graph_add_edge(graph, 0, 1, make_weight(7)) == 1, "weighted add should succeed");
    ASSERT(graph_add_edge(graph, 1, 2, make_weight(9)) == 1, "weighted add should succeed");
    graph_destroy(graph);
    PASS();
}

void test_graph_null_safety(void) {
    TEST("graph_null_safety");
    graph_destroy(NULL);
    graph_print(NULL);
    graph_bfs(NULL, 0, record_visit);
    graph_dfs(NULL, 0, record_visit);
    ASSERT(graph_add_edge(NULL, 0, 1, NULL) == 0, "add on NULL should fail");
    ASSERT(graph_remove_edge(NULL, 0, 1) == 0, "remove on NULL should fail");
    ASSERT(graph_has_edge(NULL, 0, 1) == 0, "has edge on NULL should fail");
    ASSERT(graph_get_adjacent(NULL, 0) == NULL, "adjacent on NULL should return NULL");
    ASSERT(graph_num_vertices(NULL) == 0, "vertices on NULL should be 0");
    ASSERT(graph_num_edges(NULL) == 0, "edges on NULL should be 0");
    PASS();
}

int main(void) {
    printf("=== CDS-Bedrock Graph Tests ===\n\n");

    test_graph_create();
    test_graph_add_edge_directed();
    test_graph_add_edge_undirected();
    test_graph_remove_edge();
    test_graph_get_adjacent();
    test_graph_bfs_fifo();
    test_graph_dfs();
    test_graph_destroy_weighted();
    test_graph_null_safety();

    printf("\n=== Summary ===\n");
    printf("Passed : %d\n", pass_count);
    printf("Failed : %d\n", fail_count);

    return fail_count > 0 ? 1 : 0;
}
