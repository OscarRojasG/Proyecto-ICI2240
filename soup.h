#ifndef SOUP_H
#define SOUP_H

#include <stdio.h>
#include <stdlib.h>
#include "list.h"
#include "graph.h"

typedef struct {
    int x;
    int y;
} Posicion;

typedef struct {
    char *palabra;
    int largo;
    int orientacion;
    Posicion *posicion;
} Palabra;

typedef struct {
    char **tablero;
    int tamanio;
    int total_palabras;
    List *palabras;
} SopaLetras;


SopaLetras * crearSopaLetras(List *listaPalabras, int cantidadPalabras, int tamanio);

char ** inicializarTablero(int tamanio);

List * obtenerPosiciones(SopaLetras *sopa);

void insertarPalabras(SopaLetras *sopa, List *listaPalabras, List *posiciones);

void llenarTablero(SopaLetras *sopa);

void mostrarTablero(SopaLetras *sopa);


#endif