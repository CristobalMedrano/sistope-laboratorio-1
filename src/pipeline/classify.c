//Librerías utilizadas
#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <jpeglib.h>

// Definicion de un booleano
#define FALSE 0
#define TRUE 1
// Opciones para mostrar un borde en los resultados de la bandera showResults.
#define BORDER 0

// Esta estructura contiene la informacion escencial de la imagen.
// Esta definida tal cual como lo indica la libreria jpeglib.
typedef struct Image
{
    // image_buffer es un puntero que contiene un arreglo con la informacion de la imagen, cada dato representa un pixel de valor 0 a 255.
    // En RGB se usan 3 valores consecutivos para definir un pixel (Es decir buffer[i], buffer[i+1], buffer[i+2] = 1 pixel).
    // En Escala de grises se usa solo un valor para definir un pixel. (Es decir buffer[i] = 1 pixel)
    JSAMPLE *image_buffer; /*Points to large array of R,G,B-order data*/
    // height es un entero sin signo de 32 bits que contiene la cantidad de filas de la imagen (alto de la imagen) 
    uint32_t height; /*Rows*/
    // width es un entero sin signo de 32 bits que contiene la cantidad de columnas de la imagen (ancho de la imagen) 
    uint32_t width; /*Columns*/
    // color_channel es un entero sin signo de 32 bits que contiene el numero de canales de color (para RGB son 3 canales y para escala de grises 1). 
    uint32_t color_channel; /*"component" values*/
} Image;

int classifyImage(Image image, int classificationThreshold);
void showImageResultTitle(int numberImages, int flagShowResults);
void showImageResultBody(int numberImages, int imageNumber, int flagShowResults, int isNearlyBlack);

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

int main(int argc, char const *argv[])
{
    int classificationThreshold = 0;
    Image binarizedImage = {};
    int isNearlyBlack = FALSE;
    isNearlyBlack = classifyImage(binarizedImage, classificationThreshold);
    printf("Es casi negra %d", isNearlyBlack);
    return 0;
}

