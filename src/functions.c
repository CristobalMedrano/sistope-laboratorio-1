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




