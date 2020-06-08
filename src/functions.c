//Librerías incluídas
#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <jpeglib.h>
#include <inttypes.h>
#include <string.h>
#include <constants.h>
#include <structs.h>
#include <functions.h>

//Entrada:
//  Image image: Estructura con imagen almacenada.
//
//Funcionamiento: Procedimiento que permite visualizar por pantalla información asociada
//                a una imagen, como sus dimensiones, canales y pixeles.
//
//Salida: No tiene por ser procedimiento.
void printPixels(Image image){
    // Se muestra por pantalla el alto, ancho y los canales que posee la imagen de entrada.
    printf("width = %" PRIu32 "\n", image.width);
    printf("height = %" PRIu32 "\n", image.height);
    printf("channels = %" PRIu32 "\n", image.color_channel);
    // Se inicializan variables
    uint8_t num = 0;
    int loc = 0;
    // Se recorre la imagen e imprime por pantalla el valor de sus pixeles.
    for (int i = 0; i < image.height; i++)
    {
        for (int j = 0; j < image.width*image.color_channel; j++)
        {
            num = image.image_buffer[loc];
            printf("%" PRId8 " ", num);
            loc++;
        }
        printf("\n");
    }
}

//Entradas:
//  char* string: String
//  char* character: Caracter a eliminar en el string ingresado.
//
//Funcionamiento: Procedimiento que elimina un caracter de un string (Similar a strip de python).
//
//Salida: no tiene por ser procedimiento.
void strip(char* string, char* character){
    // Se inicializan las variables
    char* token = NULL;
    // Si el string es distinto de nulo
    if (string != NULL){
        // Se divide el string en el caracter ingresado.
        // Reemplaza el caracter del string por un espacio vacio.
        token = strtok(string, character);
        // Recorre todo el string para eliminar ese caracter y sus repeticiones.
        while( token != NULL ) {
            token = strtok(NULL, character);
        }
    }
}

//Entradas:
//  int n: Cantidad de filas
//  int m: Cantidad de columnas
//
//Funcionamiento: Función que crea una matriz de nxm inicializada en 0.
//
//Salida: Una matriz de nxm inicializada en 0, Nulo si no fue posible asignar la memoria de la matriz.
int** createMatrix(int n, int m){
    // Se inicializan las variables
    int i = 0;
    int** matrix = NULL;
    
    // Se solicita memoria
    matrix = (int **)calloc(n, sizeof(int *));
                    for (i = 0; i < n; i++)
                    matrix[i] = (int *)calloc(m, sizeof(int));
    // Si la asignacion de memoria es correcta
    if(NULL != matrix)
    {
        // Se retorna la matriz inicializada en 0.
        return matrix;
    }
    // Sino, se muestra por pantalla el error.
    printf("Memoria insuficiente: createMatrix()\n");
    printf("Error: functions.c\n");
    return NULL;
}

//Entradas:
//  int** matrix: Puntero doble que contiene una matriz de enteros
//  int posX: Posicion en X donde se insertara el dato
//  int posY: Posicion en Y donde se insertara el dato
//  int value: Valor a insertar en la matriz.
//
//Funcionamiento: Función que inserta un valor en la matriz
//
//Salida: Retorna la matriz actualizada con el valor agregado.
//        Retorna Nulo si la matriz no existe o la posicion a insertar es negativa.
int** appendMatrix(int** matrix, int posX, int posY, int value){
    // Si la posicion en X e Y son negativas o la matriz no existe
    if (matrix == NULL || posX < 0 || posY < 0){
        // Se retorna nulo.
        return NULL;
    // Sino
    } else {
        // Se inserta el valor en la matriz.
        matrix[posX][posY] = value;
        // Se retorna la matriz actualizada.
        return matrix;
    }
}

//Entradas:
//  int n: Cantidad de filas
//  int m: Cantidad de columnas
//  int** matrix: Matriz de NxM
//
//Funcionamiento: Procedimiento que muetra por pantalla una matriz de nxm
//
//Salida: No tiene por ser procedimiento.
void showMatrix(int n, int m, int** matrix){
    // Si la matriz existe y la cantidad de filas/columnas es positiva.
    if(NULL != matrix && n >= 0 && m >= 0)
    {
        int i = 0;
        // Se recorre cada fila
        for(i = 0; i < n; i++)
        {
            int j = 0;
            // Se recorre cada columna
            for(j = 0; j < m; j++)
            {
                // Se muestra por pantalla dependiendo del largo del valor la matriz.
                if (matrix[i][j] >= 100) 
                {
                    printf("%d  ", matrix[i][j]);
                }
                else if (matrix[i][j] >= 10) 
                {
                    printf("%d   ", matrix[i][j]);
                }
                else if (matrix[i][j] <= 9 && matrix[i][j] >= 0)
                {
                    printf("%d    ", matrix[i][j]);
                } 
                else if (matrix[i][j] < 0 && matrix[i][j] >= -9)
                {
                    printf("%d   ", matrix[i][j]);
                }
                else if (matrix[i][j] >= -10)
                {
                    printf("%d  ", matrix[i][j]);
                }
                else if (matrix[i][j] >= -100)
                {
                    printf("%d ", matrix[i][j]);
                } else {
                    printf("%d", matrix[i][j]);
                }
            }
            printf("\n");
        }
    }   
}

//Entradas:
//  int** matrix: Puntero doble con la informacion de la matriz
//  int row: Cantidad de filas de la matriz ingresada
//
//Funcionamiento: Procedimiento que libera la memoria asignada a la matriz ingresada.
//                Primero libera el arreglo de cada fila (columnas) y luego el arreglo de filas (filas).
//
//Salida: No tiene por ser procedimiento.
void freeMatrix(int** matrix, int row)
{
    // Si la matriz existe
    if (NULL != matrix) 
    {   
        int i = 0;
        // Para cada fila en la matriz
        for(i = 0; i < row; i++)
        {
            // Se liberan las columnas asignadas.
            free(matrix[i]);
        }
        // Finalmente se libera el puntero inicial de la matriz.
        free(matrix);
    }
}