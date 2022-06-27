#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>
#include <time.h>
#include "list.h"
#include "soup.h"
#include "hashmap.h"

void mostrarMenu();
void crearSopaTematica();
void crearSopaPersonalizada();
void mostrarSopas();
void cargarSopa();
void exportarSopa();
void eliminarSopa(char *nombreSopa);
void mostrarSubmenuCrear(SopaLetras* sopa);
void mostrarSubmenuCargar(SopaLetras* sopa, char *nombre);


// Funciones auxiliares
char *get_field(char *linea, int indice);
int eliminarDeArchivo(char *nombreSopa, char *directorioSopa);
void llenarMapaTemas();
void llenarMapaSopas();
void cargarDatosSopa(SopaLetras *sopa, FILE *archivo);
void mostrarTemas();
void mostrarDificultades();
void obtenerDatosDificultad(int dificultad, int *cantidadPalabras, int *tamanioTablero);
void inicializarSopa(SopaLetras *sopa, FILE *archivoSopa);
void eliminarEspacios(char *linea);
void removerPalabra(char * texto, char * palabra);
List * obtenerPalabrasTema(char *tema);
List * obtenerPalabrasPersonalizada(int cantPalabras, int tamanio);
FILE * abrirArchivoTema(char *tema);
FILE * abrirArchivoSopa(char *nombre);
List * leerPalabrasArchivo(FILE *archivo);
List * obtenerPalabrasValidas(List *list, int largoMax);
List * obtenerPalabrasAleatorias(List *list, int numPalabras);

// Variables globales
HashMap * mapaTemas;
HashMap * mapaSopas;

int main()
{
    mapaTemas = createMap(10);
    mapaSopas = createMap(10);
    llenarMapaTemas();
    llenarMapaSopas();

    srand(time(NULL)); // Cambia la semilla de generación de números aleatorios

    int opcion = 0;
    while(opcion != 5)
    {
        mostrarMenu();
        
        printf("Ingrese una opcion: ");
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

/* Imprime el menú principal por pantalla */
void mostrarMenu()
{
    printf("1.- Crear sopa de letras tematica\n");
    printf("2.- Crear sopa de letras personalizada\n");
    printf("3.- Mostrar sopas de letras\n");
    printf("4.- Cargar sopa de letras\n");
    printf("5.- Salir\n");
    printf("\n");
}

/* Guarda el nombre de los temas en el mapa respectivo a partir del archivo temas.txt */
void llenarMapaTemas()
{
    FILE *archivo = fopen("temas.txt", "r");
    if(!archivo)
    {
        printf("Error: Archivo temas.txt no encontrado.\n");
        printf("Saliendo de la aplicacion...");
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

/* Imprime los temas por pantalla */
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

/* Lee y retorna la lista de palabras de un tema */
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

/* Abre el archivo de un tema en específico */
FILE * abrirArchivoTema(char *tema)
{
    char directorio[30];
    strcpy(directorio, "Temas/");
    strcat(directorio, tema);
    strcat(directorio, ".txt");

    FILE *archivo = fopen(directorio, "r");
    return archivo;
}

/* Lee y retorna las palabras ingresadas por el usuario para generar una sopa personalizada */
List * obtenerPalabrasPersonalizada(int cantPalabras, int largoMax)
{
    List* palabras = createList();
    char palabra[30];
    int cont = 0;

    while (cont < cantPalabras)
    {
        while(1)
        {
            printf("Ingrese palabras: ");
            fflush(stdin);
            scanf("%[^\n]", palabra);

            if((int)strlen(palabra) <= largoMax) break;
            printf("Error: Las palabras no pueden tener mas de %d letras.\n", largoMax);
        }

        char *dynchar = strdup(palabra);
        pushBack(palabras, dynchar);
        cont++;
    }

    return palabras;
}

/* Retorna la lista de palabras de un tema en específico. Si las palabras
 * no han sido cargadas previamente, lee el archivo del tema correspondiente
 * y guarda la lista de palabras en el mapa.
 */
List * obtenerPalabrasTema(char *tema)
{
    Pair *pair = searchMap(mapaTemas, tema);
    if(!pair)
    {
        printf("Error: El tema ingresado no existe.\n\n");
        return NULL;
    }

    if(pair->value) return (List *) pair->value; // Palabras previamente cargadas

    FILE *archivo = abrirArchivoTema(tema);
    if(!archivo)
    {
        printf("Error: Ocurrio un error al cargar la lista de palabras.\n\n");
        return NULL;
    }

    List *palabrasTema = leerPalabrasArchivo(archivo);
    pair->value = palabrasTema; // Se guarda la lista de palabras en el mapa
    fclose(archivo);
    
    return palabrasTema;
}

/* Retorna una lista con las palabras que no superen un largo determinado */
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

/* Retorna una cierta cantidad de palabras aleatorias de una lista.
 * Por ejemplo, si se quiere obtener la mitad de elementos de forma aleatoria,
 * por cada elemento se generan ceros y unos con la función rand, y se retornan
 * todos los elementos cuyo número sea cero.
 */
List * obtenerPalabrasAleatorias(List * list, int numPalabras)
{
    if(numPalabras >= getSize(list))
        return list;

    List *new = createList();
    int max_rand = getSize(list) / numPalabras; // Número máximo para generar de forma aleatoria
    if((getSize(list) % numPalabras) != 0)
        max_rand += 1; // Se suma 1 a max_rand cuando la división no es exacta

    int *posicionesInvalidas = (int *) calloc(getSize(list), sizeof(int)); // Elementos ya guardados

    while(getSize(new) < numPalabras) {
        void *data = firstList(list);
        int pos = 0; // Posición de los elementos en la lista

        while(data && (getSize(new) < numPalabras)) {
            int r = rand() % max_rand;
            if(r == 0 && posicionesInvalidas[pos] == 0)
            {
                pushBack(new, data); // Se agrega el elemento a la lista
                posicionesInvalidas[pos] = 1; // Se marca la posición del elemento como inválida
            }
            data = nextList(list);
            pos++;
        }
    }

    free(posicionesInvalidas);
    return new;
}

/* Imprime por pantalla la información de cada dificultad de la sopa de letras */
void mostrarDificultades()
{
    printf("\n");
    printf("%-15s %-25s %s\n", "Dificultad", "Cantidad de palabras", "Dimensiones del tablero");
    printf("%-15s %-25s %s\n", "1. Facil", "10", "12 x 12");
    printf("%-15s %-25s %s\n", "2. Media", "12", "15 x 15");
    printf("%-15s %-25s %s\n", "3. Dificil", "15", "18 x 18");
    printf("\n");
}

/* Retorna los datos necesarios para generar la sopa de letras según la dificultad */
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

/* Crea una sopa de letras a partir de un tema y muestra el submenú de la sopa generada */
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
        printf("Ingrese el numero de la dificultad: ");
        fflush(stdin);
        scanf("%d", &dificultad);
        obtenerDatosDificultad(dificultad, &cantidadPalabras, &tamanioTablero);
    }

    List *palabrasValidas = obtenerPalabrasValidas(palabrasTema, tamanioTablero);
    List *palabrasSopa = obtenerPalabrasAleatorias(palabrasValidas, cantidadPalabras);

    SopaLetras *sopa = crearSopaLetras(palabrasSopa, tamanioTablero);

    if(sopa == NULL)
        printf("Ocurrio un error al generar la sopa de letras. Intentalo de nuevo.\n\n");
    else
        mostrarSubmenuCrear(sopa);
}

/* Crea una sopa de letras personalizada y muestra el submenú de la sopa generada */
void crearSopaPersonalizada()
{
    int cantidadPalabras;
    int tamanioTablero;

    printf("\nIngrese la cantidad de palabras: ");
    fflush(stdin);
    scanf("%d", &cantidadPalabras);

    while(1)
    {
        printf("Ingrese el tamano del tablero: ");
        fflush(stdin);
        scanf("%d", &tamanioTablero);

        if(tamanioTablero >= 5 && tamanioTablero <= 20) break;
        printf("Error: El tamano del tablero debe estar entre 5 y 20.\n");
    }

    List *palabrasSopa = obtenerPalabrasPersonalizada(cantidadPalabras, tamanioTablero);
    if(!palabrasSopa) return;

    SopaLetras *sopa = crearSopaLetras(palabrasSopa, tamanioTablero);

    if(sopa == NULL)
        printf("Ocurrio un error al generar la sopa de letras. Intentalo de nuevo.\n\n");
    else
        mostrarSubmenuCrear(sopa);
}

/* Abre el archivo de una sopa previamente guardada por el usuario */
FILE * abrirArchivoSopa(char *nombre)
{
    char directorio[30];
    strcpy(directorio, "SopasPersonalizadas/");
    strcat(directorio, nombre);
    strcat(directorio, ".txt");

    FILE *archivo = fopen(directorio, "r");
    return archivo;
}

/* Elimina los espacios del tablero cuando se lee desde un archivo */
void eliminarEspacios(char *linea)
{
    for (int i = 0; i <= strlen(linea); i++)
    {
        if (linea[i] == ' ')
        {
            for (int j = i; j < strlen(linea); j++)
                linea[j] = linea[j+1];
        }
    }
}

/* Carga la información de las sopas previamente guardadas por el usuario */
void cargarDatosSopa(SopaLetras *sopa, FILE *archivo)
{
    char linea[101];

    fgets(linea, 100 * sizeof(char), archivo);
    fscanf(archivo, "%[^\n]", linea);
    fgetc(archivo);

    sopa->tamanio = atoi(linea + 8);

    fscanf(archivo, "%[^\n]", linea);
    fgetc(archivo);
    sopa->total_palabras = atoi(linea + 22);

    fgets(linea, 100 * sizeof(char), archivo);
    fgets(linea, 100 * sizeof(char), archivo);

    sopa->palabras = createList();

    for(int i = 0; i < sopa->total_palabras; i++)
    {
        fscanf(archivo, "%[^\n]", linea);
        fgetc(archivo);

        Palabra *palabra = (Palabra *) malloc(sizeof(Palabra));
        palabra->posicion = (Posicion *) malloc(sizeof(Posicion));

        palabra->palabra = get_field(linea, 0);
        palabra->posicion->x = atoi(get_field(linea, 1));
        palabra->posicion->y = atoi(get_field(linea, 2));
        palabra->orientacion = atoi(get_field(linea, 3));
        palabra->largo = strlen(palabra->palabra);

        pushBack(sopa->palabras, palabra);
    }

    fgets(linea, 100 * sizeof(char), archivo);
    fgets(linea, 100 * sizeof(char), archivo);

    sopa->tablero = inicializarTablero(sopa->tamanio);

    for (int i = 0; i < sopa->tamanio; i++)
    {
        fgets(linea, 100 * sizeof(char), archivo);
        eliminarEspacios(linea);
        strcpy(sopa->tablero[i], linea);
    }
}

/* Guarda la información de las sopas guardadas en el mapa respectivo */
void llenarMapaSopas()
{
    FILE *archivoNombres = fopen("sopas.txt", "r");
    if(!archivoNombres) return;

    char nombreSopa[30];
    while(fscanf(archivoNombres, "%[^\n]", nombreSopa) != EOF)
    {
        FILE *archivoSopa = abrirArchivoSopa(nombreSopa);
        if(!archivoSopa) continue;

        SopaLetras *sopa = (SopaLetras *) malloc(sizeof(SopaLetras));
        cargarDatosSopa(sopa, archivoSopa);
        insertMap(mapaSopas, nombreSopa, sopa);

        fgetc(archivoNombres);
        fclose(archivoSopa);
    }

    fclose(archivoNombres);
}

/* Muestra la información de las sopas previamente guardadas */
void mostrarSopas()
{
    Pair *pair = firstMap(mapaSopas);
    if(!pair) 
    {
        printf("No hay sopas guardadas.\n\n");
        return;
    }

    printf("%-15s %-25s %s\n", "Nombre", "Cantidad de palabras", "Tamano del tablero");

    while(pair)
    {
        SopaLetras *sopa = pair->value;
        printf("%-15s %-25d %d\n", pair->key, sopa->total_palabras, sopa->tamanio);
        pair = nextMap(mapaSopas);
    }
    printf("\n");
}

/* Lee campos separados por coma dentro de un archivo */
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

/* Carga una sopa previamente guardada y muestra el submenú correspondiente */
void cargarSopa()
{
    char nombreSopa[20];
    Pair *pair;
    SopaLetras *auxSopa;

    printf("Ingrese nombre de sopa a buscar: ");

    fflush(stdin);
    scanf("%s", nombreSopa);

    pair = searchMap(mapaSopas, nombreSopa);

    if(pair == NULL)
    {
        printf("No se encontraron sopas con el nombre ingresado.\n\n");
        return;
    }
    
    auxSopa = pair->value;

    mostrarSubmenuCargar(auxSopa, nombreSopa);
}

/* Guarda una sopa generada por el usuario */
void exportarSopa(SopaLetras* sopa)
{
    char directorio[50];
    char nombreSopa[30];

    printf("Ingrese el nombre: ");
    fflush(stdin); 
    scanf("%[^\n]", nombreSopa);

    FILE *archivoNombres = fopen("sopas.txt", "a");
    fseek(archivoNombres, 0, SEEK_END);
    if(ftell(archivoNombres) != 0)
        fprintf(archivoNombres, "\n");
    fprintf(archivoNombres, "%s", nombreSopa);
    fclose(archivoNombres);

    strcpy(directorio, "SopasPersonalizadas/");
    strcat(directorio, nombreSopa);
    strcat(directorio, ".txt");

    FILE* archivoSopa;
    archivoSopa = fopen(directorio, "wt");

    fprintf(archivoSopa, "Nombre/tema: %s\n", nombreSopa);
    fprintf(archivoSopa, "Tamano: %d\n", sopa->tamanio);
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

    insertMap(mapaSopas, nombreSopa, sopa);

    fclose(archivoSopa);

    printf("La sopa fue guardada en %s\n\n", directorio);
}

/* Elimina una sopa previamente guardada por el usuario */
void eliminarSopa(char *nombreSopa)
{
    char directorioSopa[50];

    strcpy(directorioSopa, "SopasPersonalizadas/");
    strcat(directorioSopa, nombreSopa);
    strcat(directorioSopa, ".txt");
    printf("%s\n", directorioSopa);
    if(remove(directorioSopa) != 0)
        printf("Error al borrar la sopa de letras.\n\n");
    else
    {
        eraseMap(mapaSopas, nombreSopa);
        eliminarDeArchivo(nombreSopa, directorioSopa);
        printf("La sopa de letras se elimino correctamente.\n\n");
    }
}

/* Remueve el nombre de la sopa eliminada del archivo sopas.txt */
int eliminarDeArchivo(char *nombreSopa, char *directorioSopa) { 
    char *nombreArchivo = (char *) malloc(sizeof(char) * 20); 
    char *texto = (char *) malloc(sizeof(char) * 200); 
    char *palabraEliminar = (char *) malloc(sizeof(char) * 20);

    strcpy(nombreArchivo, "sopas.txt"); 
    strcpy(palabraEliminar, nombreSopa); 

    FILE *fp = fopen(nombreArchivo, "r+");

    if (fp == NULL)
    { 
        printf("error al leer el archivo %s\n", nombreArchivo); 
        return -1; 
    }

    fscanf(fp, "%[^EOF]", texto);
    fclose(fp);

    fp = fopen(nombreArchivo, "w+");

    removerPalabra(texto, palabraEliminar); 

    fprintf(fp, texto); 
    fclose(fp);

    free(nombreArchivo);
    free(texto);
    free(palabraEliminar);

    return 0; 
}

/* Elimina una palabra de un string y lo retorna */
void removerPalabra(char *texto, char *palabra) { 

    int size = strlen(palabra); 

    char *ptr = strstr(texto, palabra); 

    if(ptr) { 
        int pos = (ptr - texto); 
    
        ptr = ptr + size;
        
        if((*ptr) == '\0') {
            if(size == strlen(texto)) {
                strcpy(texto, "");
            } else {
                texto[pos-1] = '\0'; 
            }
        }
        else {
            ptr++;
        
            int i; 

            for(i = 0; i < strlen(ptr); i++) { 
                texto[pos + i] = ptr[i]; 
            }
            texto[pos + i] = '\0';
        }
    }
}

/* Imprime por pantalla el submenú cuando se crea una sopa de letras */
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
        printf("Ingrese una opcion: ");
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
                return;    
            break;
        }
    }
}

/* Imprime por pantalla el submenú cuando se carga una sopa previamente guardada */
void mostrarSubmenuCargar(SopaLetras* sopa, char *nombreSopa)
{
    int opcion = 0;

    while(opcion != 4)
    {
        if(opcion != 2) mostrarTablero(sopa);
        printf("\n");

        printf("1.- Ver palabras ocultas\n");
        printf("2.- Mostrar soluciones\n");
        printf("3.- Eliminar\n");
        printf("4.- Salir\n\n");

        fflush(stdin);
        printf("Ingrese una opcion: ");
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
                eliminarSopa(nombreSopa);
                return;
        }
    }
}