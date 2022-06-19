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
    return node;
}

GraphNode* copy(GraphNode* node)
{
    GraphNode *new = (GraphNode *) malloc(sizeof(GraphNode));

    new->sopa = (SopaLetras *) malloc(sizeof(SopaLetras));
    new->sopa->palabras = copyList(node->sopa->palabras);
    new->sopa->total_palabras = node->sopa->total_palabras;
    new->sopa->tamanio = node->sopa->tamanio;

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

char ** copy_board(char **tablero, int tamanio)
{
    char **new = (char **) malloc(tamanio * sizeof(char *));

    for(int i = 0; i < tamanio; i++)
    {
        new[i] = (char *) malloc(tamanio * sizeof(char));

        for(int j = 0; j < tamanio; j++)
            new[i][j] = tablero[i][j];
    }

    return new;
}

char ** fill_board(GraphNode *node, char *palabra, Posicion *posicion, int orientacion)
{
    if(!can_be_inserted(node, palabra, posicion, orientacion))
        return NULL;

    if(node->contDir[orientacion] == ((node->sopa->total_palabras - 1) / 8 + 2))
        return NULL;

    int n, m;
    getIncrements(&n, &m, orientacion);
    char **aux = copy_board(node->sopa->tablero, node->sopa->tamanio);

    for(int i = 0, j = 0, k = 0; palabra[k] != '\0'; i += n, j += m, k++)
    {
        char c = aux[posicion->x + i][posicion->y + j];
        if(c != '\0' && c != palabra[k])
        {
            for(int i = 0; i < node->sopa->tamanio; i++)
                free(aux[i]);
            free(aux);

            return NULL;
        }

        aux[posicion->x + i][posicion->y + j] = palabra[k];
    }

    return aux;
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

List* get_adj_nodes(GraphNode* node, List *listaPalabras, List *listaPosiciones)
{
    List *adj_nodes = createList();

    Palabra *tipoPalabra = lastList(node->sopa->palabras); // Última palabra insertada
    char *ultima_palabra;

    char *palabra = firstList(listaPalabras);
    Posicion *posicion = firstList(listaPosiciones);

    if(tipoPalabra)
    {
        int flag = 1;
        ultima_palabra = tipoPalabra->palabra;

        while(palabra && flag)
        {
            if(palabra == ultima_palabra) 
                flag = 0;
            
            palabra = nextList(listaPalabras);
            posicion = nextList(listaPosiciones);
        }
    }

    if(palabra == NULL) return adj_nodes;

    int largo = strlen(palabra);

    for(int k = 1; k <= 8; k++)
    {
        char **tablero = fill_board(node, palabra, posicion, k);
    
        if(tablero)
        {
            GraphNode *new = copy(node);

            Palabra *solucion = createWord(palabra, largo, posicion, k);
            pushBack(new->sopa->palabras, solucion);
            new->sopa->tablero = tablero;
            new->contDir[k]++;

            pushBack(adj_nodes, new);
        }
    }

    return adj_nodes;
}

int is_final(GraphNode* node)
{
    if(getSize(node->sopa->palabras) == node->sopa->total_palabras)
        return 1;
    return 0;
}

GraphNode* DFS(GraphNode* initial, List *palabras, List *posiciones)
{
    Stack *stack = createStack();
    push(stack, initial);

    while(top(stack))
    {
        GraphNode *node = top(stack);
        pop(stack);
    
        if(is_final(node)) 
            return node;

        List *adj_nodes = get_adj_nodes(node, palabras, posiciones);

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