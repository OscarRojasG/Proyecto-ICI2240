#include <stdio.h>
#include <stdlib.h>

void mostrarMenu();
void crearSopaTematica();
void crearSopaPersonalizada();
void mostrarSopas();
void cargarSopa();

int main()
{
    int opcion = 0;
    while(opcion != 5)
    {
        mostrarMenu();
        
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

void crearSopaTematica()
{

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