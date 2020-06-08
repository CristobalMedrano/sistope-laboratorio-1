//Librerías utilizadas
#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <jpeglib.h>
#include <structs.h>
#include <constants.h>
#include <classify.h>

//Entradas:
//  Image image: Estructura con la imagen proveniente de la cuarta etapa (binarización).
//  int classificationThreshold: Entero con el umbral de clasificación.
//
//Funcionamiento: Función que representa una primera parte de la quinta etapa del pipeline y realiza la clasificación
//                de una imagen, según un umbral (ambos suministrados), determinando si una imagen es o no "nearly black".
//
//Salida: Entero que representa si una imagen es casi negra/nearly black (TRUE 1) o no (FALSE 0). 
int classifyImage(Image image, int classificationThreshold){
    // Se obtiene largo e inicializa contador
    int len = image.height * image.width;
    int count = 0;
    // Se recorre buffer en busca de pixeles negros y aumenta contador
    for (int i = 0; i < len; i++){
        if(image.image_buffer[i] == 0){
            count++;
        }
    }
    // Se obtiene porcentaje de negrura
    float blacknessPercentage = (count*100)/len;
    // Se clasifica según el siguiente algoritmo
    if (blacknessPercentage >= classificationThreshold){
        // Si el porcentaje es mayor al umbral entregado, se retorna TRUE 1
        return TRUE;
    }
    else{
        // En caso contrario, se retorna FALSE 0
        return FALSE;
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
        printf("\n");
        if (BORDER == TRUE) {
            if (numberImages >= 10){
                printf("|-----------------|---------------------|\n");
            }else {
                printf("|----------------|---------------------|\n");
            }
        }
        // Si el numero de imagenes tiene dos o más dígitos, se agranda el ancho de la columna "image"
        // para una correcta alineación.
        if (numberImages >= 10){
            printf("|      image      |     nearly black    |\n");
            printf("|-----------------|---------------------|\n");
        } else {
            printf("|      image     |     nearly black    |\n");
            printf("|----------------|---------------------|\n");
        }
    }
}

//Entradas:
//  int numberImages: Entero con la cantidad total de imagenes
//  int imageNumber: Entero con el número asociado a una imagen
//  int flagShowResults: Entero que representa la bandera de resultados
//                       que indica si se deben mostrar los resultados por pantalla. 
//  int isNearlyBlack: Entero que representa si una imagen es nearlyblack o no (1 y 0, respectivamente).
//
//Funcionamiento: Procedimiento que muestra el cuerpo de la tabla de resultados, es decir, la representación
//                visual por pantalla del resultado de la clasificación de cada imagen.
//
//Salida: No tiene por ser un procedimiento.
void showImageResultBody(int numberImages, int imageNumber, int flagShowResults, int isNearlyBlack) {
    // En caso que se desee mostrar por pantalla los resultados o conclusión del programa,
    // se muestra cada clasificación por imagen (cada fila representa el resultado asociado a una imagen)
    if (flagShowResults == TRUE){
        // Se maneja la alineación de las columnas en caso de tener más de 10 imagenes.
        if (imageNumber >= 10){
            if(isNearlyBlack == TRUE){
                // Caso positivo
                printf("|     image_%d   |         yes         |", imageNumber);
            }
            else{
                // Caso negativo
                printf("|     image_%d   |         no          |", imageNumber);
            }
        } else {// En caso de tener menos de 10 imagenes
            if(isNearlyBlack == TRUE){
                // Caso positivo
                printf("|     image_%d    |         yes         |", imageNumber);
            }
            else{
                // Caso negativo
                printf("|     image_%d    |         no          |", imageNumber);
            }
        } // Se manejan los bordes posterior a mostrar cada clasificación
        if (BORDER == TRUE && numberImages == imageNumber){
            if (numberImages == 10){
                printf("\n|-----------------|---------------------|");
            } else {
                printf("\n|----------------|---------------------|");
            }
        }// Saltos de linea respectivos para mejor visualización en consola
        if (numberImages == imageNumber){
            printf("\n");
        }
        printf("\n");
    }
}