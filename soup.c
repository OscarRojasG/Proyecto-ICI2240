#include "soup.h"

SopaLetras *crearSopaLetras(List *listaPalabras, int tamanio)
{
    SopaLetras *sopa = (SopaLetras *) malloc(sizeof(SopaLetras));
    sopa->palabras = createList();
    sopa->tamanio = tamanio;
    sopa->total_palabras = 0;
    sopa->tablero = inicializarTablero(tamanio);

    char *palabra = firstList(listaPalabras);
    while(palabra)
    {
        insertarPalabra(sopa, palabra);
        palabra = nextList(listaPalabras);
    }
    llenarTablero(sopa);

    return sopa;
}

char **inicializarTablero(int tamanio)
{
    char **tablero = (char **) calloc(tamanio, sizeof(char *));
    for(int i = 0; i < tamanio; i++)
        tablero[i] = (char *) calloc(tamanio, sizeof(char));
    return tablero;
}

int posicionEsValida(SopaLetras *sopa, char *palabra)
{
    return 0;
}

void insertarPalabra(SopaLetras *sopa, char *palabra)
{

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