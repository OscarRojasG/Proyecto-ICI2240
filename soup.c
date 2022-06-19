#include "soup.h"

#define COLOR_GREEN "\x1B[32m"
#define COLOR_RESET "\x1B[0m"

SopaLetras * crearSopaLetras(List *listaPalabras, int tamanio)
{
    SopaLetras *sopa = (SopaLetras *) malloc(sizeof(SopaLetras));
    sopa->palabras = createList();
    sopa->tamanio = tamanio;
    sopa->total_palabras = getSize(listaPalabras);
    sopa->tablero = inicializarTablero(tamanio);

    int success = insertarPalabras(sopa, listaPalabras);
    if(success)
    {
        llenarTablero(sopa);
        return sopa;
    }

    return NULL;
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

int insertarPalabras(SopaLetras *sopa, List *palabras)
{
    GraphNode *inicial, *final;
    for(int i = 0; i < 5; i++)
    {
        List *posiciones = obtenerPosiciones(sopa);
        inicial = createGraphNode(sopa, palabras, posiciones);
        final = DFS(inicial, palabras, posiciones);

        if(final) 
        {
            *sopa = *(final->sopa);
            return 1;
        }
    }

    return 0;
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

void mostrarSoluciones(SopaLetras *sopa)
{
    char **tmp = inicializarTablero(sopa->tamanio);

    Palabra *palabra = firstList(sopa->palabras);
    while(palabra)
    {
        int n, m;
        getIncrements(&n, &m, palabra->orientacion);

        for(int i = 0, j = 0, k = 0; k < palabra->largo; i += n, j += m, k++)
            tmp[palabra->posicion->x + i][palabra->posicion->y + j] = 'x';

        palabra = nextList(sopa->palabras);
    }

    int colored = 0;
    for(int i = 0; i < sopa->tamanio; i++)
    {
        for(int j = 0; j < sopa->tamanio; j++)
        {
            if(tmp[i][j] == 'x' && !colored)
            {
                printf("%s", COLOR_GREEN);
                colored = 1;
            }
            if(tmp[i][j] != 'x' && colored)
            {
                printf("%s", COLOR_RESET);
                colored = 0;
            }

            printf("%c ", sopa->tablero[i][j]);
        }

        printf("\n");
    }
    
    if(colored)
        printf("%s", COLOR_RESET);
}

void mostrarPalabras(SopaLetras* sopa)
{
    Palabra* palabra = firstList(sopa->palabras);

    while (palabra)
    {
        printf("%s\n", palabra->palabra);

        palabra = nextList(sopa->palabras);
    }
}