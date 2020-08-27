//Librerías incluídas
#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <jpeglib.h>
#include <inttypes.h>
#include <string.h>
#include <constants.h>
#include <functions.h>

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

