//Librerías utilizadas
#include <stdlib.h>
#include <stdio.h>
#include <jpeglib.h>
#include <inttypes.h>
#include <structs.h>
#include <grayscale.h>

//Entrada:
//  Image image: Estructura con la imagen proveniente de la primera etapa (lectura).
//
//Funcionamiento: Función que realiza la segunda etapa del pipeline, es decir, convierte una imagen a escala de grises
//                según la ecuación de luminiscencia Y = R*0.3 + G*0.59 + B*0.11
//
//Salida: Estructura imagen que representa o contiene la imagen convertida.
Image convertGrayScale(Image image){

    // Se inicializan los datos a utilizar
    int totalComponents = 0;
    int R = 0;    
    int G = 0;    
    int B = 0;
    int Y = 0;

    // Se calcula el total de componentes de la imagen
    totalComponents = image.height*image.width*image.color_channel;
    
    // Se inicializa la estructura para la imagen convertida
    Image convertedImage;
    convertedImage.height = image.height;
    convertedImage.width = image.width;
    convertedImage.color_channel = 1;
    convertedImage.image_buffer = NULL;
    convertedImage.image_buffer = (JSAMPLE*) malloc(sizeof(int) *
                                       convertedImage.width  *
                                       convertedImage.height *
                                       convertedImage.color_channel);
    // Se recorre la totalidad de los componentes de la imagen
    int pos = 0;
    for (int i = 0; i < totalComponents; i+=3){
        R = image.image_buffer[i]; //se obtiene valor R
        G = image.image_buffer[i+1]; // se obtiene valor G
        B = image.image_buffer[i+2]; // se obtiene valor B
        Y = R*0.3 + G*0.59 + B*0.11; // ecuación de luminiscencia
        convertedImage.image_buffer[pos] = Y;// se guarda el valor obtenido en ecuación
        pos++;
    }
    // Se retorna la estructura de la imagen convertida a escala de grises.
    return convertedImage;
}