#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>
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
char *get_field(char *linea, int indice);
void llenarMapaTemas();
void mostrarTemas();
void mostrarDificultades();
void obtenerDatosDificultad(int dificultad, int *cantidadPalabras, int *tamanioTablero);
void inicializarSopa(SopaLetras *sopa, FILE *archivoSopa);
List * obtenerPalabrasTema(char *tema);
List * obtenerPalabrasPersonalizada(int cantPalabras, int tamanio);
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

    char tema[30];
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

List * obtenerPalabrasPersonalizada(int cantPalabras, int largoMax)
{
    List* palabras = createList();
    char palabra [50];
    int cont = 0;

    while (cont < cantPalabras)
    {
        while(1)
        {
            printf("Ingrese palabras: ");
            fflush(stdin);
            scanf("%[^\n]", palabra);

            if((int)strlen(palabra) <= largoMax) break;
        }

        char *dynchar = strdup(palabra);
        pushBack(palabras, dynchar);
        cont++;
    }

    return palabras;
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

void mostrarDificultades()
{
    printf("\n");
    printf("%-15s %-25s %s\n", "Dificultad", "Cantidad de palabras", "Dimensiones del tablero");
    printf("%-15s %-25s %s\n", "1. Facil", "10", "12 x 12");
    printf("%-15s %-25s %s\n", "2. Media", "12", "15 x 15");
    printf("%-15s %-25s %s\n", "3. Dificil", "15", "18 x 18");
    printf("\n");
}

void obtenerDatosDificultad(int dificultad, int *cantidadPalabras, int *tamanioTablero)
{
    switch(dificultad)
    {
        case 1:
            *cantidadPalabras = 10;
            *tamanioTablero = 12;
            break;
        case 2:
            *cantidadPalabras = 12;
            *tamanioTablero = 15;
            break;
        case 3:
            *cantidadPalabras = 15;
            *tamanioTablero = 18;
            break;
    }
}

void inicializarSopa(SopaLetras *sopa, FILE *archivoSopa)
{

}

void crearSopaTematica()
{
    char tema[20];
    int dificultad;
    int cantidadPalabras = -1;
    int tamanioTablero = -1;

    mostrarTemas();

    printf("Ingrese un tema: ");
    fflush(stdin);
    scanf("%[^\n]", tema);

    List *palabrasTema = obtenerPalabrasTema(tema);
    if(!palabrasTema) return;

    while(cantidadPalabras == -1)
    {
        mostrarDificultades();
        printf("Ingrese el número de la dificultad: ");
        fflush(stdin);
        scanf("%d", &dificultad);
        obtenerDatosDificultad(dificultad, &cantidadPalabras, &tamanioTablero);
    }

    List *palabrasValidas = obtenerPalabrasValidas(palabrasTema, tamanioTablero);
    List *palabrasSopa = obtenerPalabrasAleatorias(palabrasValidas, cantidadPalabras);

    SopaLetras *sopa = crearSopaLetras(palabrasSopa, tamanioTablero);

    if(sopa == NULL)
        printf("Ocurrió un error al generar la sopa de letras. Inténtalo de nuevo.\n\n");
    else
        mostrarSubmenuCrear(sopa);
}

void crearSopaPersonalizada()
{
    char nombre[30];
    int cantidadPalabras;
    int tamanioTablero;

    printf("\nIngrese nombre de sopa: ");
    fflush(stdin);
    scanf("%[^\n]", nombre);

    printf("\nCantidad de palabras a ingresar: ");
    fflush(stdin);
    scanf("%d", &cantidadPalabras);

    while(1)
    {
        printf("\nTamaño de tablero: ");
        fflush(stdin);
        scanf("%d", &tamanioTablero);

        if(tamanioTablero <= 25)break;
    }

    List *palabrasSopa = obtenerPalabrasPersonalizada(cantidadPalabras, tamanioTablero);
    if(!palabrasSopa) return;

    SopaLetras *sopa = crearSopaLetras(palabrasSopa, tamanioTablero);

    if(sopa == NULL)
        printf("Ocurrió un error al generar la sopa de letras. Inténtalo de nuevo.\n\n");
    else
        mostrarSubmenuCrear(sopa);
}


void mostrarSopas()
{

}

char *get_field(char *linea, int indice)
{
    char *campo = (char *) malloc(100 * sizeof(char *)); // Guarda el string a retornar
    int i = 0; // Recorre la linea
    int k = 0; // Cuenta las comas
    int n = 0; // Recorre el campo
    bool comillas = false;

    while(linea[i] != '\0')
    {
        if(linea[i] == '\"')
        {
            comillas = !comillas;
        }

        if(k == indice)
        {
            if(linea[i] != '\"')
            {
                campo[n] = linea[i];
                n++;
            }
        }

        i++;

        if(linea[i] == ',' && !comillas)
        {
            k++;
            i++;
        }

        if(k > indice || linea[i] == '\0' || linea[i] == '\n')
        {
            campo[n] = '\0';
            return campo;
        }
    }

    return NULL;
}


void cargarSopa()
{
    char directorio[30];
    char nombreSopa[30];

    printf("Ingrese el nombre: ");
    fflush(stdin); scanf("%[^\n]", nombreSopa);

    strcpy(directorio, "SopasPersonalizadas/");
    strcat(directorio, nombreSopa);
    strcat(directorio, ".txt");

    FILE* archivoSopa;
    archivoSopa = fopen(directorio, "r");
    if(!archivoSopa) 
    {
        printf("ERROR: archivo %s no encontrado\n", nombreSopa);
        return;
    }

    SopaLetras *auxSopa = (SopaLetras *) malloc(sizeof(SopaLetras));

    char linea [101];

    fgets(linea, 100 * sizeof(char), archivoSopa);
    printf("\nNombre sopa: %s\n", linea + 13);

    fgets(linea, 100 * sizeof(char), archivoSopa);
    auxSopa->tamanio = atoi(linea + 8);
    fgets(linea, 100 * sizeof(char), archivoSopa);
    auxSopa->total_palabras = atoi(linea + 22);

    List *palabrasOcultas = createList();

    while (fgets(linea, 100 * sizeof(char), archivoSopa))
        if(strstr(linea, "Lista de palabras:")) break;

    while (fgets (linea, 100 * sizeof(char), archivoSopa)) 
    {
        if(strstr(linea, "Tablero:")) break;

        char *stringPalabra = get_field(linea, 0);
        int x = atoi(get_field(linea, 1));
        int y = atoi(get_field(linea, 2));  // Pasamos el string a entero
        int orientacion = atoi(get_field(linea, 3)); // Pasamos el string a entero

        Posicion *posicion = (Posicion *) malloc(sizeof(Posicion));
        posicion->x = x;
        posicion->y = y;

        Palabra *palabra = create_word(stringPalabra, ((int)strlen(stringPalabra)), posicion, orientacion);
        pushBack(palabrasOcultas, palabra);
    }

    popBack(palabrasOcultas);
    
    copiarTablero(auxSopa, archivoSopa);
    
}

void exportarSopa(SopaLetras* sopa)
{
    char directorio[30];
    char nombreSopa[30];

    printf("Ingrese el nombre: ");
    fflush(stdin); scanf("%[^\n]", nombreSopa);

    strcpy(directorio, "SopasPersonalizadas/");
    strcat(directorio, nombreSopa);
    strcat(directorio, ".txt");

    FILE* archivoSopa;
    archivoSopa = fopen(directorio, "wt");

    fprintf(archivoSopa, "Nombre/tema: %s\n", nombreSopa);
    fprintf(archivoSopa, "Tamaño: %d\n", sopa->tamanio);
    fprintf(archivoSopa, "Cantidad de palabras: %d\n", sopa->total_palabras);

    fprintf(archivoSopa, "\nLista de palabras:\n");

    Palabra *auxPalabra;
    auxPalabra = firstList(sopa->palabras);

    while(auxPalabra)
    {
        fprintf(archivoSopa, "%s,%d,%d,%d\n", auxPalabra->palabra, auxPalabra->posicion->x, auxPalabra->posicion->y, auxPalabra->orientacion);
        auxPalabra = nextList(sopa->palabras);
    }

    fprintf(archivoSopa, "\nTablero:\n");

    for (int i = 0; i < sopa->tamanio; i++)
    {
        for (int j = 0; j < sopa->tamanio; j++)
        {
            fprintf(archivoSopa, "%c ", sopa->tablero[i][j]);
        }
        fprintf(archivoSopa, "\n");
    }

    fclose(archivoSopa);

    printf("La sopa fue guardada en %s\n\n", directorio);

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