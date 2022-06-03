#include "graph.h"

GraphNode* createGraphNode(char **tablero, int tamanio, List *palabras, List *posiciones)
{
    GraphNode *node = (GraphNode *) calloc(1, sizeof(GraphNode));
    node->tablero = tablero;
    node->tamanio = tamanio;
    node->intersecciones = 0;
    node->palabrasRestantes = palabras;
    node->posicionesRestantes = posiciones;
    return node;
}

GraphNode* copy(GraphNode* node)
{
    return NULL;
}

int is_valid(GraphNode* node)
{
    return 0;
}

List* get_adj_nodes(GraphNode* node)
{
    return NULL;
}


int is_final(GraphNode* node)
{
    return 0;
}

GraphNode* DFS(GraphNode* initial)
{

}