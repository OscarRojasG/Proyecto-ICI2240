#include "soup.h"

#define COLOR_GREEN "\x1B[32m"
#define COLOR_RESET "\x1B[0m"

/* Crea y retorna una sopa de letras a partir de la lista de palabras y el tamaño del tablero */
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

/* Inicializa un tablero vacío con caracteres nulos */
char ** inicializarTablero(int tamanio)
{
    char **tablero = (char **) calloc(tamanio, sizeof(char *));
    for(int i = 0; i < tamanio; i++)
        tablero[i] = (char *) calloc(tamanio, sizeof(char));
    return tablero;
}

/* Obtiene las posiciones de cada palabra en el tablero, de modo que cada palabra quepa
 * en al menos una dirección. Por ejemplo, si el largo de la palabra coincide con el
 * tamaño del tablero, la palabra solo puede ser insertada en los bordes.
 */
List * obtenerPosiciones(SopaLetras *sopa, List *listaPalabras)
{
    List *listaPosiciones = createList();
    char *palabra = firstList(listaPalabras);
    char **tmp = inicializarTablero(sopa->tamanio); // Usado para guardar las posiciones ocupadas

    while(palabra)
    {
        Posicion *posicion = (Posicion *) malloc(sizeof(Posicion));
        int largo = strlen(palabra);
        int eje = rand() % 2; // Eje de orientación de la palabra (horizontal - vertical)
        int intervalo = rand() % 2; // Indica si la posición estará al inicio o al final del tablero
        int posValida = rand() % (sopa->tamanio - largo + 1); // Posición válida al inicio del tablero

        if(intervalo == 1)
            posValida += sopa->tamanio - posValida - 1; // Posición válida al final del tablero

        if(eje == 0)
        {
            posicion->x = posValida; // Posición válida en el eje x
            posicion->y = rand() % sopa->tamanio; // Posición aleatoria en el eje y
        }
        else
        {
            posicion->y = posValida; // Posición válida en el eje y
            posicion->x = rand() % sopa->tamanio; // Posición aleatoria en el eje x
        }

        if(tmp[posicion->x][posicion->y] == 'x') // Posición ocupada
            continue;

        tmp[posicion->x][posicion->y] = 'x'; // Se marca la posición como ocupada
        pushBack(listaPosiciones, posicion);

        palabra = nextList(listaPalabras);
    }

    return listaPosiciones;
}

/* Ordena las palabras de mayor a menor según el número de caracteres. 
 * Esto disminuye la cantidad de nodos adyacentes generados en el grafo
 * debido a que se comienza insertando las palabras más largas, que son
 * aquellas con menos combinaciones posibles en el tablero. 
 */
void ordenarPalabrasPorLargo(List *listaPalabras)
{
    int tamanio = getSize(listaPalabras);
    char **tmp = (char **) malloc(tamanio * sizeof(char *)); // Arreglo auxiliar
    char *palabra;

    int i = 0;
    while((palabra = popFront(listaPalabras)) != NULL)
    {
        tmp[i] = palabra;
        i++;
    }

    qsort(tmp, tamanio, sizeof(char *), cmp);

    for(int i = 0; i < tamanio; i++)
        pushBack(listaPalabras, tmp[i]);

    free(tmp);
}

/* Función para comparar elementos utilizada por la función qsort */
int cmp(const void * a, const void * b)
{
    char *p1 = *((char **) a);
    char *p2 = *((char **) b);
    int size_p1 = strlen(p1);
    int size_p2 = strlen(p2);
    if(size_p1 > size_p2) return -1;
    if(size_p1 < size_p2) return 1;
    return 0;
}

/* Inserta las palabras en la sopa de letras haciendo uso del grafo.
 * Este tendrá hasta 1000 intentos en caso de no encontrar un nodo válido.
 */
int insertarPalabras(SopaLetras *sopa, List *palabras)
{
    GraphNode *inicial, *final;
    ordenarPalabrasPorLargo(palabras);

    for(int i = 0; i < 1000; i++)
    {
        List *posiciones = obtenerPosiciones(sopa, palabras);
        inicial = create_graph_node(sopa);
        final = DFS(inicial, palabras, posiciones);

        if(final) 
        {
            *sopa = *(final->sopa);
            return 1;
        }
    }

    return 0;
}

/* Llena el tablero con letras aleatorias en las posiciones vacías */
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

/* Imprime el tablero por pantalla */
void mostrarTablero(SopaLetras *sopa)
{
    for(int i = 0; i < sopa->tamanio; i++)
    {
        for(int j = 0; j < sopa->tamanio; j++)
            printf("%c ", sopa->tablero[i][j]);

        printf("\n");
    }
}

/* Imprime el tablero mostrando las palabras ocultas con color */
void mostrarSoluciones(SopaLetras *sopa)
{
    char **tmp = inicializarTablero(sopa->tamanio); // Tablero auxiliar

    Palabra *palabra = firstList(sopa->palabras);
    while(palabra)
    {
        int n, m;
        get_increments(&n, &m, palabra->orientacion); // Incrementos para recorrer la palabra en el tablero

        for(int i = 0, j = 0, k = 0; k < palabra->largo; i += n, j += m, k++)
            tmp[palabra->posicion->x + i][palabra->posicion->y + j] = 'x'; // Se marca la posición

        palabra = nextList(sopa->palabras);
    }

    int colored = 0;
    for(int i = 0; i < sopa->tamanio; i++)
    {
        for(int j = 0; j < sopa->tamanio; j++)
        {
            if(tmp[i][j] == 'x' && !colored) // Se imprimen las posiciones marcadas en color
            {
                printf("%s", COLOR_GREEN);
                colored = 1;
            }
            if(tmp[i][j] != 'x' && colored) // Se resetea el color
            {
                printf("%s", COLOR_RESET);
                colored = 0;
            }

            printf("%c ", sopa->tablero[i][j]);
        }

        printf("\n");
    }
    
    if(colored)
        printf("%s", COLOR_RESET); // Se resetea el color
}

/* Muestra la lista de palabras ocultas en el tablero */
void mostrarPalabras(SopaLetras* sopa)
{
    Palabra* palabra = firstList(sopa->palabras);

    while (palabra)
    {
        printf("%s\n", palabra->palabra);
        palabra = nextList(sopa->palabras);
    }
    
    printf("\n");
}