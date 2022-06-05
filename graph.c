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

    new->sopa = (SopaLetras *) malloc(sizeof(SopaLetras));
    new->sopa->palabras = copyList(node->sopa->palabras);
    new->sopa->total_palabras = node->sopa->total_palabras;
    new->sopa->tamanio = node->sopa->tamanio;

    new->sopa->tablero = inicializarTablero(node->sopa->tamanio);
    for(int i = 0; i < node->sopa->tamanio; i++)
        for(int j = 0; j < node->sopa->tamanio; j++)
            new->sopa->tablero[i][j] = node->sopa->tablero[i][j];

    new->palabrasRestantes = copyList(node->palabrasRestantes);
    new->posicionesRestantes = copyList(node->posicionesRestantes);
    new->intersecciones = node->intersecciones;

    for(int i = 0; i < 9; i++)
        new->contDir[i] = node->contDir[i];

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

void getIncrements(int *horizontal, int *vertical, int orientacion)
{
    *horizontal = 0, *vertical = 0;

    if(orientacion == DIR_RIGHT || orientacion == DIR_RIGHT_DOWN || orientacion == DIR_RIGHT_UP)
        *horizontal = 1;
    if(orientacion == DIR_LEFT || orientacion == DIR_LEFT_DOWN || orientacion == DIR_LEFT_UP)
        *horizontal = -1;
    if(orientacion == DIR_DOWN || orientacion == DIR_RIGHT_DOWN || orientacion == DIR_LEFT_DOWN)
        *vertical = 1;
    if(orientacion == DIR_UP || orientacion == DIR_RIGHT_UP || orientacion == DIR_LEFT_UP)
        *vertical = -1;
}

int fill_board(GraphNode* node, char *palabra, Posicion *posicion, int orientacion)
{
    if(!can_be_inserted(node, palabra, posicion, orientacion))
        return 0;

    if(node->contDir[orientacion] == ((node->sopa->total_palabras - 1) / 8 + 2))
        return 0;

    int n, m;
    getIncrements(&n, &m, orientacion);

    int word_size = strlen(palabra);
    for(int i = 0, j = 0, k = 0; k < word_size; i += n, j += m, k++)
    {
        char c = node->sopa->tablero[posicion->x + i][posicion->y + j];
        if(c != '\0' && c != palabra[k]) return 0;

        if(c == palabra[k])
            node->intersecciones++;

        node->sopa->tablero[posicion->x + i][posicion->y + j] = palabra[k];
    }
    node->contDir[orientacion]++;

    Palabra *solucion = createWord(palabra, word_size, posicion, orientacion);
    pushBack(node->sopa->palabras, solucion);

    return 1;
}

Palabra *createWord(char *palabra, int largo, Posicion *posicion, int orientacion)
{
    Palabra *solucion = (Palabra *) malloc(sizeof(Palabra));
    solucion->palabra = palabra;
    solucion->largo = largo;
    solucion->posicion = posicion;
    solucion->orientacion = orientacion;

    return solucion;
}

List* get_adj_nodes(GraphNode* node)
{
    List *adj_nodes = createList();
    char *palabra = popFront(node->palabrasRestantes);
    Posicion *posicion = popFront(node->posicionesRestantes);

    for(int k = 1; k <= 8; k++)
    {
        GraphNode *new = copy(node);
        int valid = fill_board(new, palabra, posicion, k);
        if(valid)
            pushBack(adj_nodes, new);
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

        if(is_final(node)) 
            return node;

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