#ifndef GRAPH_H
#define GRAPH_H

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "list.h"
#include "soup.h"

typedef struct {
    SopaLetras *sopa;
    int contDir[9];
} GraphNode;


GraphNode * create_graph_node(SopaLetras *sopa);

GraphNode * copy_node(GraphNode *node);

char ** copy_board(char **tablero, int tamanio);

int is_balanced(GraphNode *node, int orientacion);

int can_be_inserted(SopaLetras *sopa, int largo_palabra, Posicion *posicion, int orientacion);

void get_increments(int *horizontal, int *vertical, int orientacion);

char ** fill_board(GraphNode* node, char *palabra, Posicion *posicion, int orientacion);

Palabra * create_word(char *palabra, int largo, Posicion *posicion, int orientacion);

List * get_adj_nodes(GraphNode* node, List *listaPalabras, List *listaPosiciones);

int is_final(GraphNode* node);

GraphNode * DFS(GraphNode* initial, List *palabras, List *posiciones);


#endif