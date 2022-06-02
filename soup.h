#ifndef SOUP_H
#define SOUP_H

#include <stdio.h>
#include <stdlib.h>
#include "list.h"

typedef struct {
    char *palabra;
    int largo;
    int orientacion;
    int pos_x;
    int pos_y;
} Palabra;

typedef struct {
    char **tablero;
    int tamanio;
    int total_palabras;
    List *palabras;
} SopaLetras;


SopaLetras *crearSopaLetras(List *listaPalabras, int tamanio);

char **inicializarTablero(int tamanio);

int posicionEsValida(SopaLetras *sopa, char *palabra);

void insertarPalabra(SopaLetras *sopa, char *palabra);

void llenarTablero(SopaLetras *sopa);

void mostrarTablero(SopaLetras *sopa);


#endif