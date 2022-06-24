#include "graph.h"

/* Se definen las orientaciones como enteros para facilitar su manejo */
#define DIR_RIGHT 1
#define DIR_LEFT 2
#define DIR_DOWN 3
#define DIR_UP 4
#define DIR_RIGHT_DOWN 5
#define DIR_RIGHT_UP 6
#define DIR_LEFT_DOWN 7
#define DIR_LEFT_UP 8

/* Crea el nodo inicial a partir de una sopa de letras vacía */
GraphNode* create_graph_node(SopaLetras *sopa)
{
    GraphNode *node = (GraphNode *) calloc(1, sizeof(GraphNode));
    node->sopa = sopa;
    return node;
}

/* Realiza una copia profunda de un nodo sin considerar el tablero */
GraphNode* copy_node(GraphNode *node)
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

/* Realiza una copia del tablero */
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

/* 
 * Verifica si la orientación de la palabra a insertar desbalancea la sopa de letras.
 * Que esté balanceada significa que todas sus palabras deben estar en diferentes 
 * direcciones de manera equitativa.
 */
int is_balanced(GraphNode *node, int orientacion)
{
    if(node->contDir[orientacion] == ((node->sopa->total_palabras - 1) / 8 + 2))
        return 0;
    return 1;
}

/* Verifica si la palabra cabe dentro del tablero considerando su orientación */
int can_be_inserted(SopaLetras *sopa, int largo_palabra, Posicion *posicion, int orientacion)
{
    if(orientacion == DIR_RIGHT || orientacion == DIR_RIGHT_DOWN || orientacion == DIR_RIGHT_UP)
        if((posicion->x + largo_palabra) > sopa->tamanio)
            return 0;
    if(orientacion == DIR_LEFT || orientacion == DIR_LEFT_DOWN || orientacion == DIR_LEFT_UP)
        if((posicion->x - largo_palabra + 1) < 0)
            return 0;
    if(orientacion == DIR_DOWN || orientacion == DIR_RIGHT_DOWN || orientacion == DIR_LEFT_DOWN)
        if((posicion->y + largo_palabra) > sopa->tamanio)
            return 0;
    if(orientacion == DIR_UP || orientacion == DIR_RIGHT_UP || orientacion == DIR_LEFT_UP)
        if((posicion->y - largo_palabra + 1) < 0)
            return 0;

    return 1;
}

/* Retorna los incrementos cuando se quiere recorrer el tablero en una determinada dirección */
void get_increments(int *horizontal, int *vertical, int orientacion)
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

/* 
 * Inserta la palabra en el tablero según la posición y orientación indicadas.
 * Retorna una copia del tablero con la palabra insertada. Si en el proceso
 * de inserción ocurre un choque con otra palabra, retorna NULL.
 */
char ** fill_board(GraphNode *node, char *palabra, Posicion *posicion, int orientacion)
{
    int n, m;
    get_increments(&n, &m, orientacion); // Se obtienen los incrementos para recorrer el tablero
    char **aux = copy_board(node->sopa->tablero, node->sopa->tamanio);

    for(int i = 0, j = 0, k = 0; palabra[k] != '\0'; i += n, j += m, k++)
    {
        char c = aux[posicion->x + i][posicion->y + j];
        if(c != '\0' && c != palabra[k]) // Choque entre palabras donde los caracteres no coinciden
        {
            for(int i = 0; i < node->sopa->tamanio; i++)
                free(aux[i]); // Se libera la memoria del tablero copiado
            free(aux);

            return NULL;
        }

        aux[posicion->x + i][posicion->y + j] = palabra[k];
    }

    return aux;
}

/* Crea y retorna una palabra a partir de sus atributos */ 
Palabra *create_word(char *palabra, int largo, Posicion *posicion, int orientacion)
{
    Palabra *solucion = (Palabra *) malloc(sizeof(Palabra));
    solucion->palabra = palabra;
    solucion->largo = largo;
    solucion->posicion = posicion;
    solucion->orientacion = orientacion;

    return solucion;
}

/* 
 * Obtiene los nodos adyacentes. Cada acción sobre un nodo corresponde a insertar
 * una nueva palabra en la posición correspondiente pero en distintas direcciones.
 * Para encontrar las palabras que aún faltan por insertar, se busca en la lista
 * de palabras la palabra siguiente a la última insertada.
 */
List* get_adj_nodes(GraphNode *node, List *listaPalabras, List *listaPosiciones)
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
            if(palabra == ultima_palabra) // Se encuentra la última palabra insertada
                flag = 0; // Se usa para salir del ciclo al terminar la iteración actual
            
            palabra = nextList(listaPalabras); // Avanza a la siguiente palabra
            posicion = nextList(listaPosiciones); // Avanza a la siguiente posición
        }
    }

    if(palabra == NULL) return adj_nodes; // No quedan más palabras para insertar
    int largo = strlen(palabra);

    /* Se intentará insertar la palabra en todas las orientaciones posibles */
    for(int orientacion = 1; orientacion <= 8; orientacion++)
    {
        /* Se verifica si la palabra puede insertarse en el tablero en esa orientación */
        if(!is_balanced(node, orientacion)) continue;
        if(!can_be_inserted(node->sopa, largo, posicion, orientacion)) continue;

        char **tablero = fill_board(node, palabra, posicion, orientacion);
    
        if(tablero) // Tablero válido
        {
            GraphNode *new = copy_node(node); // Se crea un nuevo nodo

            /* Se añade la palabra insertada a la lista de palabras en la sopa */
            Palabra *solucion = create_word(palabra, largo, posicion, orientacion);
            pushBack(new->sopa->palabras, solucion);

            new->sopa->tablero = tablero; // Se copia el tablero al nuevo nodo
            new->contDir[orientacion]++; // Se aumenta el contador de direcciones 

            pushBack(adj_nodes, new); // Se inserta el nodo adyacente a la lista
        }
    }

    return adj_nodes;
}

/*
 * Retorna 1 si el nodo corresponde a un estado final. Retorna 0 en caso contrario.
 * Un estado final es aquel donde se han insertado todas las palabras en el tablero.
 */
int is_final(GraphNode *node)
{
    if(getSize(node->sopa->palabras) == node->sopa->total_palabras)
        return 1;
    return 0;
}

/* 
 * Realiza una búsqueda profunda entre los nodos del grafo hasta encontrar una sopa válida.
 * Retorna NULL en caso de no encontrar solución.
 */
GraphNode* DFS(GraphNode *initial, List *palabras, List *posiciones)
{
    Stack *stack = createStack(); // Los nodos se guardan en una pila
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
            push(stack, aux); // Se insertan los nodos adyacentes en la pila
            aux = nextList(adj_nodes);
        }

        free(node); // Se libera memoria del nodo actual
    }

    return NULL;
}