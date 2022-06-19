#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "list.h"
#include "soup.h"
#include "hashmap.h"

void mostrarMenu();
void crearSopaTematica();
void crearSopaPersonalizada();
void mostrarSopas();
void cargarSopa();
void exportarSopa();
void mostrarSubmenuCrear(SopaLetras* sopa);
void mostrarSubmenuCargar(SopaLetras* sopa);


// Funciones auxiliares
void llenarMapaTemas();
void mostrarTemas();
List * obtenerPalabrasTema(char *tema);
FILE * abrirArchivoTema(char *tema);
List * leerPalabrasArchivo(FILE *archivo);
List * obtenerPalabrasValidas(List *list, int largoMax);
List * obtenerPalabrasAleatorias(List *list, int numPalabras);

// Variables globales
HashMap * mapaTemas;

int main()
{
    mapaTemas = createMap(10);
    llenarMapaTemas();

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

void llenarMapaTemas()
{
    FILE *archivo = fopen("temas.txt", "r");
    if(!archivo)
    {
        printf("Error: Archivo temas.txt no encontrado.\n");
        printf("Saliendo de la aplicación...");
        exit(EXIT_FAILURE);
    }

    char tema[20];
    while(fscanf(archivo, "%[^\n]", tema) != EOF)
    {
        char *dynchar = strdup(tema);
        insertMap(mapaTemas, dynchar, NULL);
        fgetc(archivo);
    }
}

void mostrarTemas()
{
    Pair *pair = firstMap(mapaTemas);
    char *tema = NULL;
    int num = 1;

    while(pair)
    {
        tema = pair->key;
        printf("%d.- %s\n", num, tema);
        pair = nextMap(mapaTemas);
        num++;
    }
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
    if(!pair)
    {
        printf("Error: El tema ingresado no existe.\n\n");
        return NULL;
    }

    if(pair->value) return (List *) pair->value;

    FILE *archivo = abrirArchivoTema(tema);
    if(!archivo)
    {
        printf("Error: Ocurrió un error al cargar la lista de palabras.\n\n");
        return NULL;
    }

    List *palabrasTema = leerPalabrasArchivo(archivo);
    pair->value = palabrasTema;
    fclose(archivo);
    
    return palabrasTema;
}

List * obtenerPalabrasValidas(List *list, int largoMax)
{
    List *new = createList();
    char *palabra = (char *) firstList(list);
    while(palabra)
    {
        if(strlen(palabra) <= largoMax)
            pushBack(new, palabra);
        palabra = nextList(list);
    }

    return new;
}

List * obtenerPalabrasAleatorias(List * list, int numPalabras)
{
    if(numPalabras >= getSize(list))
        return list;

    List *new = createList();
    int max_rand = getSize(list) / numPalabras;
    if((getSize(list) % numPalabras) == 0)
        max_rand -= 1;

    int *posicionesInvalidas = (int *) calloc(getSize(list), sizeof(int));

    while(getSize(new) < numPalabras) {
        void *data = firstList(list);
        int pos = 0;

        while(data && (getSize(new) < numPalabras)) {
            int r = rand() % max_rand;
            if(r == 0 && posicionesInvalidas[pos] == 0)
            {
                pushBack(new, data);
                posicionesInvalidas[pos] = 1;
            }
            data = nextList(list);
            pos++;
        }
    }

    free(posicionesInvalidas);
    return new;
}

void crearSopaTematica()
{
    char tema[20];
    int cantidadPalabras = 18;
    int tamanioTablero = 18;

    mostrarTemas();

    printf("Ingrese un tema: ");
    fflush(stdin);
    scanf("%[^\n]", tema);

    List *palabrasTema = obtenerPalabrasTema(tema);
    if(!palabrasTema) return;

    List *palabrasValidas = obtenerPalabrasValidas(palabrasTema, tamanioTablero);
    List *palabrasSopa = obtenerPalabrasAleatorias(palabrasValidas, cantidadPalabras);

    SopaLetras *sopa = crearSopaLetras(palabrasSopa, tamanioTablero);

    if(sopa == NULL)
        printf("Ocurrió un error al generar la sopa de letras. Inténtalo de nuevo.\n");
    else
        mostrarSubmenuCrear(sopa);
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

void exportarSopa()
{

}

void eliminarSopa()
{

}

void mostrarSubmenuCrear(SopaLetras* sopa)
{
    int opcion = 0;
    printf("\n");
    
    while(opcion != 4)
    {
        if(opcion != 2) mostrarTablero(sopa);
        printf("\n");

        printf("1.- Ver palabras ocultas\n");
        printf("2.- Mostrar soluciones\n");
        printf("3.- Exportar\n");
        printf("4.- Salir\n\n");

        fflush(stdin);
        printf("Ingrese una opción: ");
        scanf("%d", &opcion);
        printf("\n");

        switch (opcion)
        {
            case 1:
                mostrarPalabras(sopa);
                break;

            case 2:
                mostrarSoluciones(sopa);
                break;

            case 3:
                exportarSopa(sopa);
                break;
        }
    }
}

void mostrarSubmenuCargar(SopaLetras* sopa)
{
   int opcion = 0;

    while(opcion != 4)
    {
        printf("1.- Ver palabras ocultas\n");
        printf("2.- Mostrar soluciones\n");
        printf("3.- Eliminar\n");
        printf("4.- Salir\n\n");

        fflush(stdin);
        scanf("%d", &opcion);

        switch (opcion)
        {
            case 1:
                mostrarPalabras(sopa);
            break;

            case 2:
                mostrarSoluciones(sopa);
            break;

            case 3:
                eliminarSopa(sopa);
            break;
    
            default:
            break;
        }
    }
}