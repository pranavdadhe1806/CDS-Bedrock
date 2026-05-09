#include "../include/bedrock.h"
#include <stdio.h>
#include <stdlib.h>

static int pass_count = 0;
static int fail_count = 0;
static int visit_order[32];
static int visit_count = 0;

#define TEST(name) printf("  %-34s", name)
#define PASS() do { printf("PASS\n"); pass_count++; } while (0)
#define FAIL(msg) do { printf("FAIL - %s\n", msg); fail_count++; return; } while (0)
#define ASSERT(cond, msg) do { if (!(cond)) { FAIL(msg); } } while (0)

static void record_visit(int vertex) {
    if (visit_count < 32) {
        visit_order[visit_count++] = vertex;
    }
}

static void reset_visits(void) {
    visit_count = 0;
    for (int i = 0; i < 32; i++) {
        visit_order[i] = -1;
    }
}

static int visited_contains(int vertex) {
    for (int i = 0; i < visit_count; i++) {
        if (visit_order[i] == vertex) return 1;
    }
    return 0;
}

static int visited_count_of(int vertex) {
    int count = 0;
    for (int i = 0; i < visit_count; i++) {
        if (visit_order[i] == vertex) count++;
    }
    return count;
}

static int *make_weight(int value) {
    int *weight = malloc(sizeof(int));
    if (weight != NULL) *weight = value;
    return weight;
}

static void print_int_weight(const void *weight) {
    if (weight != NULL) {
        printf("%d", *(const int *)weight);
    }
}

void test_add_edge_directed(void) {
    TEST("add_edge_directed");
    Graph *graph = graph_new(3, 1);
    ASSERT(graph != NULL, "graph_new returned NULL");
    ASSERT(graph_edge(graph, 0, 1) == 1, "add edge should succeed");
    ASSERT(graph_connected(graph, 0, 1) == 1, "0->1 should exist");
    ASSERT(graph_connected(graph, 1, 0) == 0, "1->0 should not exist");
    graph_destroy(graph);
    PASS();
}

void test_add_edge_undirected(void) {
    TEST("add_edge_undirected");
    Graph *graph = graph_new(3, 0);
    ASSERT(graph != NULL, "graph_new returned NULL");
    ASSERT(graph_edge(graph, 0, 1) == 1, "add edge should succeed");
    ASSERT(graph_connected(graph, 0, 1) == 1, "0-1 should exist");
    ASSERT(graph_connected(graph, 1, 0) == 1, "1-0 should exist");
    graph_destroy(graph);
    PASS();
}

void test_remove_edge_directed(void) {
    TEST("remove_edge_directed");
    Graph *graph = graph_new(3, 1);
    ASSERT(graph != NULL, "graph_new returned NULL");
    graph_edge(graph, 0, 1);
    ASSERT(graph_unedge(graph, 0, 1) == 1, "remove should succeed");
    ASSERT(graph_connected(graph, 0, 1) == 0, "0->1 should be gone");
    graph_destroy(graph);
    PASS();
}

void test_remove_edge_undirected(void) {
    TEST("remove_edge_undirected");
    Graph *graph = graph_new(3, 0);
    ASSERT(graph != NULL, "graph_new returned NULL");
    graph_edge(graph, 0, 1);
    ASSERT(graph_unedge(graph, 0, 1) == 1, "remove should succeed");
    ASSERT(graph_connected(graph, 0, 1) == 0, "0-1 should be gone");
    ASSERT(graph_connected(graph, 1, 0) == 0, "1-0 should be gone");
    graph_destroy(graph);
    PASS();
}

void test_remove_nonexistent_edge(void) {
    TEST("remove_nonexistent_edge");
    Graph *graph = graph_new(3, 1);
    ASSERT(graph != NULL, "graph_new returned NULL");
    ASSERT(graph_unedge(graph, 0, 1) == 0, "missing remove should return 0");
    graph_destroy(graph);
    PASS();
}

void test_has_edge_false(void) {
    TEST("has_edge_false");
    Graph *graph = graph_new(3, 1);
    ASSERT(graph != NULL, "graph_new returned NULL");
    ASSERT(graph_connected(graph, 0, 2) == 0, "unconnected vertices should return 0");
    graph_destroy(graph);
    PASS();
}

void test_num_edges_directed(void) {
    TEST("num_edges_directed");
    Graph *graph = graph_new(4, 1);
    ASSERT(graph != NULL, "graph_new returned NULL");
    graph_edge(graph, 0, 1);
    graph_edge(graph, 0, 2);
    graph_edge(graph, 2, 3);
    ASSERT(graph_edges(graph) == 3, "directed edge count should be 3");
    graph_destroy(graph);
    PASS();
}

void test_num_edges_undirected(void) {
    TEST("num_edges_undirected");
    Graph *graph = graph_new(4, 0);
    ASSERT(graph != NULL, "graph_new returned NULL");
    graph_edge(graph, 0, 1);
    graph_edge(graph, 0, 2);
    graph_edge(graph, 2, 3);
    ASSERT(graph_edges(graph) == 3, "undirected edge count should be 3");
    graph_destroy(graph);
    PASS();
}

void test_bfs_order(void) {
    TEST("bfs_order");
    Graph *graph = graph_new(4, 1);
    ASSERT(graph != NULL, "graph_new returned NULL");
    graph_edge(graph, 0, 2);
    graph_edge(graph, 0, 1);
    graph_edge(graph, 1, 3);
    graph_edge(graph, 2, 3);

    reset_visits();
    graph_bfs(graph, 0, record_visit);

    ASSERT(visit_count == 4, "BFS should visit 4 vertices");
    ASSERT(visit_order[0] == 0, "BFS should start at 0");
    ASSERT((visit_order[1] == 1 || visit_order[1] == 2), "second visit should be layer 1");
    ASSERT((visit_order[2] == 1 || visit_order[2] == 2), "third visit should be layer 1");
    ASSERT(visit_order[3] == 3, "vertex 3 should be layer 2");
    ASSERT(visited_count_of(3) == 1, "vertex 3 should be visited once");
    graph_destroy(graph);
    PASS();
}

void test_dfs_visits_all_reachable(void) {
    TEST("dfs_visits_all_reachable");
    Graph *graph = graph_new(4, 1);
    ASSERT(graph != NULL, "graph_new returned NULL");
    graph_edge(graph, 0, 1);
    graph_edge(graph, 1, 2);
    graph_edge(graph, 2, 3);

    reset_visits();
    graph_dfs(graph, 0, record_visit);

    ASSERT(visit_count == 4, "DFS should visit all reachable vertices");
    ASSERT(visited_contains(0) && visited_contains(1) && visited_contains(2) && visited_contains(3),
           "DFS should visit 0,1,2,3");
    graph_destroy(graph);
    PASS();
}

void test_bfs_disconnected(void) {
    TEST("bfs_disconnected");
    Graph *graph = graph_new(5, 1);
    ASSERT(graph != NULL, "graph_new returned NULL");
    graph_edge(graph, 0, 1);
    graph_edge(graph, 1, 2);

    reset_visits();
    graph_bfs(graph, 0, record_visit);

    ASSERT(visit_count == 3, "BFS should only visit connected component");
    ASSERT(!visited_contains(3) && !visited_contains(4), "isolated vertices should not be visited");
    graph_destroy(graph);
    PASS();
}

void test_dfs_disconnected(void) {
    TEST("dfs_disconnected");
    Graph *graph = graph_new(5, 1);
    ASSERT(graph != NULL, "graph_new returned NULL");
    graph_edge(graph, 0, 1);
    graph_edge(graph, 1, 2);

    reset_visits();
    graph_dfs(graph, 0, record_visit);

    ASSERT(visit_count == 3, "DFS should only visit connected component");
    ASSERT(!visited_contains(3) && !visited_contains(4), "isolated vertices should not be visited");
    graph_destroy(graph);
    PASS();
}

void test_bfs_with_cycle(void) {
    TEST("bfs_with_cycle");
    Graph *graph = graph_new(3, 1);
    ASSERT(graph != NULL, "graph_new returned NULL");
    graph_edge(graph, 0, 1);
    graph_edge(graph, 1, 2);
    graph_edge(graph, 2, 0);

    reset_visits();
    graph_bfs(graph, 0, record_visit);

    ASSERT(visit_count == 3, "BFS cycle should terminate");
    ASSERT(visited_count_of(0) == 1 && visited_count_of(1) == 1 && visited_count_of(2) == 1,
           "BFS should visit each cycle vertex once");
    graph_destroy(graph);
    PASS();
}

void test_dfs_with_cycle(void) {
    TEST("dfs_with_cycle");
    Graph *graph = graph_new(3, 1);
    ASSERT(graph != NULL, "graph_new returned NULL");
    graph_edge(graph, 0, 1);
    graph_edge(graph, 1, 2);
    graph_edge(graph, 2, 0);

    reset_visits();
    graph_dfs(graph, 0, record_visit);

    ASSERT(visit_count == 3, "DFS cycle should terminate");
    ASSERT(visited_count_of(0) == 1 && visited_count_of(1) == 1 && visited_count_of(2) == 1,
           "DFS should visit each cycle vertex once");
    graph_destroy(graph);
    PASS();
}

void test_invalid_vertex(void) {
    TEST("invalid_vertex");
    Graph *graph = graph_new(3, 1);
    ASSERT(graph != NULL, "graph_new returned NULL");
    ASSERT(graph_edge(graph, -1, 0) == 0, "negative src should fail");
    ASSERT(graph_edge(graph, 0, 3) == 0, "out-of-range dest should fail");
    ASSERT(graph_connected(graph, 0, 99) == 0, "invalid has_edge should fail");
    ASSERT(graph_get_adjacent(graph, -1) == NULL, "invalid adjacent should return NULL");
    graph_destroy(graph);
    PASS();
}

void test_graph_print(void) {
    TEST("graph_print");
    Graph *empty = graph_new(2, 1);
    Graph *weighted = graph_new_weighted(2, 1, print_int_weight, free);
    ASSERT(empty != NULL && weighted != NULL, "graph allocation failed");

    graph_print(empty);
    graph_weighted_edge(weighted, 0, 1, make_weight(7));
    graph_print(weighted);
    graph_print(NULL);

    graph_destroy(empty);
    graph_destroy(weighted);
    PASS();
}

void test_destroy_valgrind(void) {
    TEST("destroy_valgrind");
    Graph *graph = graph_new_weighted(6, 1, NULL, free);
    ASSERT(graph != NULL, "graph_new_weighted returned NULL");

    for (int i = 0; i < 10; i++) {
        int src = i % 6;
        int dest = (i + 1) % 6;
        ASSERT(graph_weighted_edge(graph, src, dest, make_weight(i)) == 1,
               "weighted edge should be inserted");
    }

    graph_destroy(graph);
    PASS();
}

int main(void) {
    printf("=== CDS-Bedrock Phase 8: Graph Tests ===\n\n");

    test_add_edge_directed();
    test_add_edge_undirected();
    test_remove_edge_directed();
    test_remove_edge_undirected();
    test_remove_nonexistent_edge();
    test_has_edge_false();
    test_num_edges_directed();
    test_num_edges_undirected();
    test_bfs_order();
    test_dfs_visits_all_reachable();
    test_bfs_disconnected();
    test_dfs_disconnected();
    test_bfs_with_cycle();
    test_dfs_with_cycle();
    test_invalid_vertex();
    test_graph_print();
    test_destroy_valgrind();

    printf("\n=== Summary ===\n");
    printf("Passed : %d\n", pass_count);
    printf("Failed : %d\n", fail_count);

    return fail_count > 0 ? 1 : 0;
}
