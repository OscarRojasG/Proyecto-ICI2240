#ifndef GRAPH_H
#define GRAPH_H

#include <stdio.h>
#include <stdlib.h>
#include "list.h"

typedef struct {
    char **tablero;
    int tamanio;
    int intersecciones;
    int contDir[8];
    List *palabrasRestantes;
    List *posicionesRestantes;
} GraphNode;


GraphNode* createGraphNode(char **tablero, int tamanio, List *palabras, List *posiciones);

GraphNode* copy(GraphNode* node);

int is_valid(GraphNode* node);

List* get_adj_nodes(GraphNode* node);

int is_final(GraphNode* node);

GraphNode* DFS(GraphNode* initial);


#endif