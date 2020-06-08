//Librerías utilizadas
#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <jpeglib.h>
#include <structs.h>
#include <binarize.h>

//Entradas:
//  Image image: Estructura con la imagen proveniente de la tercera etapa (filtro laplaciano).
//  int binarizationThreshold: Entero con el umbral de binarización
//
//Funcionamiento: Función que realiza la cuarta etapa del pipeline, es decir, binariza una imagen según un algoritmo definido
//                y un umbral de binarización entregado.
//
//Salida: Estructura con imagen binarizada.
Image binarizeImage(Image image, int binarizationThreshold){
    int len = image.height * image.width;
    // Se recorre buffer de la imagen
    for (int i = 0; i < len; i++){
        // En caso que un valor sea mayor al umbral entregado, se vuelve 255.
        if (image.image_buffer[i] > binarizationThreshold){
            image.image_buffer[i] = 255;
        }
        else{
            // En caso contrario, se vuelve 0.
            image.image_buffer[i] = 0;
        }
    }
    // Se retorna estructura de imagen binarizada resultante.
    return image;
}
