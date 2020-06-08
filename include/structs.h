#ifndef _STRUCTS_H
#define _STRUCTS_H

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

#endif