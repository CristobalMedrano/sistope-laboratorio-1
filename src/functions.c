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
//  int numberImages: Entero con el número de imagenes.
//  int flagShowResults: Entero que representa la bandera de resultados
//                       que indica si se deben mostrar los resultados por pantalla.
//
//Funcionamiento: Procedimiento encargado de mostrar por pantalla el inicio de la tabulación
//                para la muestra de los resultados finales del programa.
//
//Salida: No tiene por tratarse de un procedimiento.
void showImageResultTitle(int numberImages, int flagShowResults){
    // En caso que se desee mostrar por pantalla los resultados o conclusión del programa
    // Se imprime por pantalla las cabeceras de las columnas de la tabla de resultados
    if (flagShowResults == TRUE){
        fprintf(stderr, "\n");
        if (BORDER == TRUE) {
            if (numberImages >= 10){
                fprintf(stderr, "|-----------------|---------------------|\n");
            }else {
                fprintf(stderr, "|----------------|---------------------|\n");
            }
        }
        // Si el numero de imagenes tiene dos o más dígitos, se agranda el ancho de la columna "image"
        // para una correcta alineación.
        if (numberImages >= 10){
            fprintf(stderr, "|      image      |     nearly black    |\n");
            fprintf(stderr, "|-----------------|---------------------|\n");
        } else {
            fprintf(stderr, "|      image     |     nearly black    |\n");
            fprintf(stderr, "|----------------|---------------------|\n");
        }
    }
}

