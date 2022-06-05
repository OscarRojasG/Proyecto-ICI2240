#ifndef GRAPH_H
#define GRAPH_H

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "list.h"
#include "soup.h"

typedef struct {
    SopaLetras *sopa;
    int intersecciones;
    int contDir[9];
    List *palabrasRestantes;
    List *posicionesRestantes;
} GraphNode;


GraphNode* createGraphNode(SopaLetras *sopa, List *palabras, List *posiciones);

GraphNode* copy(GraphNode* node);

int fill_board(GraphNode* node, char *palabra, Posicion *posicion, int orientacion);

Palabra *createWord(char *palabra, int largo, Posicion *posicion, int orientacion);

int can_be_inserted(GraphNode* node, char *palabra, Posicion *posicion, int orientacion);

List* get_adj_nodes(GraphNode* node);

void getIncrements(int *horizontal, int *vertical, int orientacion);

int is_final(GraphNode* node);

GraphNode* DFS(GraphNode* initial);


#endif