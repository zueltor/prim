#include "header.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <limits.h>

void prim(char *in, char *out) {
    int n_vertices,
            n_edges;
    unsigned int i;

    FILE *f1 = fopen(in, "rb");
    FILE *f2 = fopen(out, "wb");
    if (f1 == NULL) {
        errors(1);
        return;
    }
    if (f2 == NULL) {
        errors(1);
        return;
    }
    fseek(f1, 0, SEEK_END);
    int size = ftell(f1);
    if (size == 0) {
        errors(2);
        return;
    }
    fseek(f1, 0, SEEK_SET);

    if (fscanf(f1, "%d", &n_vertices)) {
        if (n_vertices < 0 || n_vertices > 5000) {
            errors(2);
            return;
        }
    } else {
        errors(6);
        return;
    }

    if (fscanf(f1, "%d", &n_edges) != 1) {
        errors(6);
        return;
    }
    if (n_edges < 0 || n_edges > n_vertices * (n_vertices - 1) / 2) {
        errors(3);
        return;
    }

    Graph *G = create_graph(n_vertices, n_edges);

    int v1, v2, length;

    for (i = 0; i < G->E; i++) {
        if (fscanf(f1, "%d %d %d", &v1, &v2, &length) == 3) {
            if (v1 < 1 || v1 > G->V || v2 < 1 || v2 > G->V) {
                free_graph(G);
                errors(4);
                return;
            } else if (length < 0 || length > INT_MAX) {
                free_graph(G);
                errors(5);
                return;
            }
        } else {
            free_graph(G);
            errors(6);
            return;
        }
        if (v1 < v2)
            G->Adj[v2][v1] = length;
        else G->Adj[v1][v2] = length;
    }

    edges *MST = (edges *) malloc((G->V) * sizeof(edges));
    int *visited_vertices = (int *) malloc((G->V + 1) * sizeof(int));
    memset(visited_vertices, 0, (G->V + 1) * sizeof(int));
    distance *dist = (distance *) malloc((G->V + 1) * sizeof(distance));

    unsigned int j = 0,
            k = 0;

    for (i = 1; i <= G->V; i++) {
        dist[i].length = UINT_MAX;
        dist[i].from = i;
    }
    unsigned int min_length,
            v;

    dist[1].length = 0;

    for (i = 1; i <= G->V; i++) {
        min_length = UINT_MAX;
        for (j = 1; j <= G->V; j++) {
            if (visited_vertices[j] == 0 && dist[j].length < min_length) {
                min_length = dist[j].length;
                v = j;
            }
        }
        visited_vertices[v] = 1;
        MST[k].from = v;
        MST[k].to = dist[v].from;
        k++;
        for (j = 1; j <= G->V; j++) {
            if (j < v) {
                v2 = j;
                v1 = v;
            } else {
                v2 = v;
                v1 = j;
            }
            if (dist[j].length > G->Adj[v1][v2] && G->Adj[v1][v2] > 0) {
                dist[j].length = G->Adj[v1][v2];
                dist[j].from = v;
            }
        }
    }

    if ((n_vertices == 0) || (n_vertices > 1 && n_edges == 0)) {
        errors(7);
        free(visited_vertices);
        free(dist);
        free(MST);
        free_graph(G);
        return;

    }
    for (i = 1; i <= n_vertices; i++)
        if (dist[i].length == UINT_MAX) {
            free_graph(G);
            free(visited_vertices);
            free(dist);
            free(MST);
            errors(7);
            return;
        }
    for (i = 1; i < k; i++)
        fprintf(f2, "%d %d\n", MST[i].from, MST[i].to);

    free(visited_vertices);
    free(dist);
    free(MST);
    free_graph(G);
}

Graph *create_graph(unsigned int n_vertices, unsigned int n_edges) {
    unsigned int i;
    Graph *G = (Graph *) malloc(sizeof(Graph));
    G->V = n_vertices;
    G->E = n_edges;

    G->Adj = (Matr *) malloc((G->V + 1) * sizeof(Matr));
    for (i = 1; i <= G->V; i++) {
        G->Adj[i] = (int *) malloc((i + 1) * sizeof(int));
        memset(G->Adj[i], -1, (i + 1) * sizeof(int));
    }
    return G;
}

void free_graph(Graph *G) {
    unsigned int i;
    for (i = 1; i <= G->V; i++)
        free(G->Adj[i]);
    free(G->Adj);
    free(G);
}


void errors(int err) {
    switch (err) {
        case 1:
            printf("file cannot be opened");
            break;
        case 2:
            printf("bad number of vertices");
            break;
        case 3:
            printf("bad number of edges");
            break;
        case 4:
            printf("bad vertex");
            break;
        case 5:
            printf("bad length");
            break;
        case 6:
            printf("bad number of lines");
            break;
        case 7:
            printf("no spanning tree");
            break;
        default:;
    }
}

void print_help(void) {
    printf("Topsort: <inputfile> <outputfile>");
}
