#pragma once

typedef struct edges edges;
struct edges {
    int from;
    int to;
};

typedef int *Matr;

typedef struct Graph Graph;
struct Graph {
    int V;
    int E;
    Matr *Adj;
};

typedef struct distance distance;
struct distance {
    unsigned int length;
    int from;
};


void prim(char *in, char *out);

Graph *create_graph(unsigned int n_vertices, unsigned int n_edges);

void free_graph(Graph *G);

void errors(int err);

void print_help(void);
