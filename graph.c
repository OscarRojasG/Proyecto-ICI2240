#include "graph.h"

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
    return new;
}

int fill_board(GraphNode* node, char *palabra, Posicion *posicion, int orientacion)
{
    return 1;
}

List* get_adj_nodes(GraphNode* node)
{
    List *adj_nodes = createList();

    for(int i = 0; i < node->sopa->total_palabras; i++)
    {
        GraphNode *new = copy(node);
        char *palabra = firstList(new->palabrasRestantes);
        for(int n = 0; n == i; n++)
            palabra = nextList(new->palabrasRestantes);

        popCurrent(new->palabrasRestantes);

        for(int j = 0; j < node->sopa->total_palabras; j++)
        {
            GraphNode *new2 = copy(new);
            Posicion *posicion = firstList(new2->posicionesRestantes);
            for(int n = 0; n == j; n++)
                posicion = nextList(new2->posicionesRestantes);

            popCurrent(new2->posicionesRestantes);

            for(int k = 1; k <= 8; k++)
            {
                GraphNode *new3 = copy(new2);
                int valid = fill_board(new3, palabra, posicion, k);
                if(valid)
                    pushBack(adj_nodes, new3);
            }
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

        if(is_final(node)) return node;

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