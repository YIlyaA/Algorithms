#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>

#define MAX_VERTICES 100

struct Graph {
    bool visited[MAX_VERTICES];
    int adjacency_matrix[MAX_VERTICES][MAX_VERTICES];
    int num_vertices;
};

void addEdge(struct Graph *graph, int v, int w) {
    graph->adjacency_matrix[v][w] = 1;
    graph->adjacency_matrix[w][v] = -1;
}

void DFS(struct Graph *graph, int v) {
    graph->visited[v] = true;
    printf("%d ", v);

    for (int i = 0; i < graph->num_vertices; i++) {
        if (graph->adjacency_matrix[v][i] && !graph->visited[i]) {
            DFS(graph, i);
        }
    }
}

int main() {
    struct Graph graph;
    int num_edges, start_vertex;

    printf("Enter the number of vertices:");
    scanf("%d", &graph.num_vertices);

    printf("Enter the number of edges:");
    scanf("%d", &num_edges);

    for (int i = 0; i < MAX_VERTICES; i++) {
        graph.visited[i] = false;
        for (int j = 0; j < MAX_VERTICES; j++) {
            graph.adjacency_matrix[i][j] = 0;
        }
    }

    printf("Enter the edges (vertex1 vertex2):\n");
    for (int i = 0; i < num_edges; i++) {
        int vertex1, vertex2;
        scanf("%d %d", &vertex1, &vertex2);
        addEdge(&graph, vertex1, vertex2);
    }

    printf("Adjacency Matrix:\n");
    for (int i = 0; i < graph.num_vertices; i++) {
        for (int j = 0; j < graph.num_vertices; j++) {
            if (graph.adjacency_matrix[i][j] == -1)
                printf("%d  ", graph.adjacency_matrix[i][j]);
            else
                printf("%d   ", graph.adjacency_matrix[i][j]);
        }
        printf("\n");
    }


    printf("Enter the starting vertex:");
    scanf("%d", &start_vertex);
    printf("DFS Traversal:\n");

    DFS(&graph, start_vertex);
    for (int i = 0; i < graph.num_vertices; i++) {
        if (!graph.visited[i]) {
            DFS(&graph, i);
        }
    }
    printf("\n");
    printf("\n");

    return 0;
}
