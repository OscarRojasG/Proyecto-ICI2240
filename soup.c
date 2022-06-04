#include "soup.h"

SopaLetras * crearSopaLetras(List *listaPalabras, int cantidadPalabras, int tamanio)
{
    SopaLetras *sopa = (SopaLetras *) malloc(sizeof(SopaLetras));
    sopa->palabras = createList();
    sopa->tamanio = tamanio;
    sopa->total_palabras = cantidadPalabras;
    sopa->tablero = inicializarTablero(tamanio);

    List *posiciones = obtenerPosiciones(sopa);
    insertarPalabras(sopa, listaPalabras, posiciones);
    //llenarTablero(sopa);

    return sopa;
}

char ** inicializarTablero(int tamanio)
{
    char **tablero = (char **) calloc(tamanio, sizeof(char *));
    for(int i = 0; i < tamanio; i++)
        tablero[i] = (char *) calloc(tamanio, sizeof(char));
    return tablero;
}

List * obtenerPosiciones(SopaLetras *sopa)
{
    int cont = 0;
    List *listaPosiciones = createList();
    char **tmp = inicializarTablero(sopa->tamanio);

    while(cont < sopa->total_palabras)
    {
        Posicion *posicion = (Posicion *) malloc(sizeof(Posicion));
        posicion->x = rand() % sopa->tamanio;
        posicion->y = rand() % sopa->tamanio;

        if(tmp[posicion->x][posicion->y] == 'x')
            continue;

        tmp[posicion->x][posicion->y] = 'x';
        pushBack(listaPosiciones, posicion);
        cont++;
    }

    return listaPosiciones;
}

void insertarPalabras(SopaLetras *sopa, List *palabras, List *posiciones)
{
    GraphNode *inicial = createGraphNode(sopa, palabras, posiciones);
    GraphNode *final = DFS(inicial);
}

void llenarTablero(SopaLetras *sopa)
{
    for(int i = 0; i < sopa->tamanio; i++)
        for(int j = 0; j < sopa->tamanio; j++)
            if(sopa->tablero[i][j] == '\0')
            {
                int random = rand() % 26 + 97;
                char c = (char) random;
                sopa->tablero[i][j] = c;
            }
}

void mostrarTablero(SopaLetras *sopa)
{
    for(int i = 0; i < sopa->tamanio; i++)
    {
        for(int j = 0; j < sopa->tamanio; j++)
            printf("%c ", sopa->tablero[i][j]);

        printf("\n");
    }
}