#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "list.h"
#include "soup.h"
#include "util.h"
#include "hashmap.h"

void mostrarMenu();
void crearSopaTematica();
void crearSopaPersonalizada();
void mostrarSopas();
void cargarSopa();

// Funciones auxiliares
List * obtenerPalabrasTema(char *tema);
FILE * abrirArchivoTema(char *tema);
List * leerPalabrasArchivo(FILE *archivo);

// Variables globales
HashMap * mapaTemas;

int main()
{
    mapaTemas = createMap(10);

    int opcion = 0;
    while(opcion != 5)
    {
        mostrarMenu();
        
        printf("Ingrese una opción: ");
        fflush(stdin);
        scanf("%d", &opcion);

        switch(opcion)
        {
            case 1:
                crearSopaTematica();
                break;
            case 2:
                crearSopaPersonalizada();
                break;
            case 3:
                mostrarSopas();
                break;
            case 4:
                cargarSopa();
                break;
            case 5:
                exit(EXIT_SUCCESS);
                break;
        }
        printf("\n");
    }

    return 0;
}

void mostrarMenu()
{
    printf("1.- Crear sopa de letras temática\n");
    printf("2.- Crear sopa de letras personalizada\n");
    printf("3.- Mostrar sopas de letras\n");
    printf("4.- Cargar sopa de letras\n");
    printf("5.- Salir\n");
    printf("\n");
}

List * leerPalabrasArchivo(FILE *archivo)
{
    List *list = createList();
    char palabra[20];
    while(fscanf(archivo, "%[^\n]", palabra) != EOF)
    {
        char *dynchar = strdup(palabra);
        pushBack(list, dynchar);
        fgetc(archivo);
    }
    return list;
}

FILE * abrirArchivoTema(char *tema)
{
    char directorio[30];
    strcpy(directorio, "Temas/");
    strcat(directorio, tema);
    strcat(directorio, ".txt");

    FILE *archivo = fopen(directorio, "r");
    return archivo;
}

List * obtenerPalabrasTema(char *tema)
{
    Pair *pair = searchMap(mapaTemas, tema);
    if(pair) return (List *) pair->value;

    FILE *archivo = abrirArchivoTema(tema);
    if(!archivo)
    {
        printf("Error: El tema ingresado no existe.\n");
        return NULL;
    }

    List *palabrasTema = leerPalabrasArchivo(archivo);
    insertMap(mapaTemas, tema, palabrasTema);
    fclose(archivo);
    
    return palabrasTema;
}

void crearSopaTematica()
{
    char tema[20];

    printf("Ingrese un tema: ");
    fflush(stdin);
    scanf("%[^\n]", tema);

    List *palabrasTema = obtenerPalabrasTema(tema);
    if(!palabrasTema) return;

    List *palabrasSopa = getRandomElements(palabrasTema, 15);

    SopaLetras *sopa = crearSopaLetras(palabrasSopa, 15);

    if(sopa == NULL)
        printf("Ocurrió un error al generar la sopa de letras. Inténtalo de nuevo.\n");
    else
        mostrarSoluciones(sopa);
}



void crearSopaPersonalizada()
{

}

void mostrarSopas()
{

}

void cargarSopa()
{

}