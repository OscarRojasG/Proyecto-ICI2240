#include "soup.h"

SopaLetras *crearSopaLetras(List *listaPalabras, int tamanio)
{
    SopaLetras *sopa = (SopaLetras *) malloc(sizeof(SopaLetras));
    sopa->tamanio = tamanio;
    sopa->total_palabras = 0;
    sopa->tablero = inicializarTablero(tamanio);

    return sopa;
}

char **inicializarTablero(int tamanio)
{
    char **tablero = (char **) malloc(sizeof(char *));
    for(int i = 0; i < tamanio; i++)
        tablero[i] = (char *) malloc(sizeof(char));
    return tablero;
}

int posicionEsValida(SopaLetras *sopa, Palabra *palabra)
{
    return 0;
}

void insertarPalabra(SopaLetras *sopa, Palabra *palabra)
{

}

void llenarTablero(SopaLetras *sopa)
{

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