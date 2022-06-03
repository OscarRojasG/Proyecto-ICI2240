#ifndef GRAPH_H
#define GRAPH_H

#include <stdio.h>
#include <stdlib.h>
#include "list.h"
#include "soup.h"

typedef struct {
    SopaLetras *sopa;
    int intersecciones;
    int contDir[8];
    List *palabrasRestantes;
    List *posicionesRestantes;
} GraphNode;


GraphNode* createGraphNode(SopaLetras *sopa, List *palabras, List *posiciones);

GraphNode* copy(GraphNode* node);

int is_valid(GraphNode* node);

List* get_adj_nodes(GraphNode* node);

int is_final(GraphNode* node);

GraphNode* DFS(GraphNode* initial);


#endif