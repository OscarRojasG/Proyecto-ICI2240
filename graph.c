#include "graph.h"

#define DIR_RIGHT 1
#define DIR_LEFT 2
#define DIR_DOWN 3
#define DIR_UP 4
#define DIR_RIGHT_DOWN 5
#define DIR_RIGHT_UP 6
#define DIR_LEFT_DOWN 7
#define DIR_LEFT_UP 8


GraphNode* createGraphNode(SopaLetras *sopa, List *palabras, List *posiciones)
{
    GraphNode *node = (GraphNode *) calloc(1, sizeof(GraphNode));
    node->sopa = sopa;
    node->intersecciones = 0;
    node->palabrasRestantes = palabras;
    node->posicionesRestantes = posiciones;
    return node;
}

GraphNode* copy(GraphNode* node)
{
    GraphNode *new = (GraphNode *) malloc(sizeof(GraphNode));
    *new = *node;
    new->palabrasRestantes = copyList(node->palabrasRestantes);
    new->posicionesRestantes = copyList(node->posicionesRestantes);
    return new;
}

int can_be_inserted(GraphNode* node, char *palabra, Posicion *posicion, int orientacion)
{
    int word_size = strlen(palabra);

    if(orientacion == DIR_RIGHT || orientacion == DIR_RIGHT_DOWN || orientacion == DIR_RIGHT_UP)
        if((posicion->x + word_size) > node->sopa->tamanio)
            return 0;
    if(orientacion == DIR_LEFT || orientacion == DIR_LEFT_DOWN || orientacion == DIR_LEFT_UP)
        if((posicion->x - word_size + 1) < 0)
            return 0;
    if(orientacion == DIR_DOWN || orientacion == DIR_RIGHT_DOWN || orientacion == DIR_LEFT_DOWN)
        if((posicion->y + word_size) > node->sopa->tamanio)
            return 0;
    if(orientacion == DIR_UP || orientacion == DIR_RIGHT_UP || orientacion == DIR_LEFT_UP)
        if((posicion->y - word_size + 1) < 0)
            return 0;

    return 1;
}

int fill_board(GraphNode* node, char *palabra, Posicion *posicion, int orientacion)
{
    //if(!can_be_inserted(node, palabra, posicion, orientacion))
    //    return 0;
    return 1;
}

List* get_adj_nodes(GraphNode* node)
{
    List *adj_nodes = createList();
    char *palabra = popFront(node->palabrasRestantes);

    for(int i = 0; i < getSize(node->posicionesRestantes); i++)
    {
        GraphNode *new = copy(node);
        Posicion *posicion = firstList(new->posicionesRestantes);
        for(int n = 0; n < getSize(new->posicionesRestantes); n++)
        {
            if(n == i) break;
            posicion = nextList(new->posicionesRestantes);
        }

        popCurrent(new->posicionesRestantes);

        for(int k = 1; k <= 8; k++)
        {
            GraphNode *new2 = copy(new);
            int valid = fill_board(new2, palabra, posicion, k);
            if(valid)
                pushBack(adj_nodes, new2);
        }
    }

    return adj_nodes;
}

int is_final(GraphNode* node)
{
    if(firstList(node->palabrasRestantes))
        return 0;
    return 1;
}

GraphNode* DFS(GraphNode* initial)
{
    Stack *stack = createStack();
    push(stack, initial);

    while(top(stack))
    {
        GraphNode *node = top(stack);
        pop(stack);

        if(is_final(node)) continue;

        List *adj_nodes = get_adj_nodes(node);
        GraphNode *aux = firstList(adj_nodes);

        while(aux)
        {
            push(stack, aux);
            aux = nextList(adj_nodes);
        }

        free(node);
    }

    return NULL;
}