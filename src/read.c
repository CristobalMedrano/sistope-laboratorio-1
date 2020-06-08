// Librerías utilizadas
#include <stdlib.h>
#include <stdio.h>
#include <jpeglib.h>
#include <inttypes.h>
#include <structs.h>
#include <constants.h>
#include <functions.h>
#include <read.h>

//Entrada:
//  int imageNumber: Entero que representa el número asociado a una imagen.
//
//Funcionamiento: Función encargada de realizar la primera etapa del pipeline, es decir, lee la imagen "i".
//
//Salida: Retorna la estructura de la imagen, útil para desarrollar las próximas etapas.
Image readImage(int imageNumber){
    // Se inicializa la estructura imagen
    Image image;
    // Se inicializa el nombre del archivo e imagen "i", según la entrada.
    char fileName[40] = "";
    char imageName[40] = "";
    sprintf(fileName, "./imagen_%i.jpg", imageNumber);
    sprintf(imageName, "image_%i.jpg", imageNumber);
    // Estructura para el manejador de errores JPEG
    struct jpeg_error_mgr jerr;
    // Se realiza la lectura y valida su funcionamiento
    if (!readJPG(fileName, &image, &jerr)){
        // Se indica por pantalla en caso de error
        printf("fallo readJPG");
    }
    // Se muestra por pantalla los pixeles (se omite para efectos finales)
    //printPixels(image);
    
    // Se retorna la estructura con imagen leida
    return image;
}

//Entradas: 
//  char* filename: Nombre del archivo de imagen
//  Image* image: puntero a estructura con imagen almacenada
//  struct jpeg_error_mgr* jerr: estructura que representa el manejador de errores de jpeglib
//
//Procedimiento: Función que presenta la rutina que permite la lectura o descompresión JPEG y ocupa elementos de jpeglib.
//
//Salidas: Devuelve un entero. Si retorna verdadero (TRUE 1), representa éxito en la lectura. 
//         Por el contrario, falso (FALSE 0) en caso de error.
int readJPG(char* filename, Image* image, struct jpeg_error_mgr* jerr){
    // Estructura que contiene los parámetros y punteros de descompresión JPEG 
    // al espacio de trabajo, definidos según jpeglib.h
    struct jpeg_decompress_struct cinfo;

    // Se configuran las rutinas de error JPEG normales
    cinfo.err = jpeg_std_error(jerr);

    // Se abre el archivo de entrada con "b" para leer archivos binarios.
    FILE *fp = fopen(filename, "rb");
    if (fp == NULL) {
        printf("Error al abrir el archivo %s\n", filename);
        return FALSE;
    }

    // Se inicializa el objeto de descompresión JPEG
    jpeg_create_decompress(&cinfo);
    // Se especifica la fuente u origen de los datos (archivo fp)
    jpeg_stdio_src(&cinfo, fp);
    // Se leen los parámetros del archivo
    jpeg_read_header(&cinfo, 1);
    // Se inicia el descompresor
    jpeg_start_decompress(&cinfo);

    // Como en este punto ya se tienen las dimensiones de la imagen, se guardan
    // y se crea el buffer necesario para su posterior uso.
    image->height = cinfo.image_height;
    image->width = cinfo.image_width;
    image->color_channel = cinfo.num_components;
    image->image_buffer = NULL;
    image->image_buffer = (JSAMPLE*) malloc(sizeof(int) *
                                       image->width  *
                                       image->height *
                                       image->color_channel);

    int row_stride = image->width * image->color_channel;
    JSAMPLE* buffer = (JSAMPLE*)image->image_buffer;
    
    for (int i = 0; i < image->height; i++)
    {
        // Se realiza el escaneo o lectura de lineas, en este caso tenemos solo un elemento de largo
        jpeg_read_scanlines(&cinfo, &buffer, 1);
        buffer += row_stride;
    }
    // Se finaliza la descompresión
    jpeg_finish_decompress(&cinfo);
    // Se libera la memoria del objeto de descompresión
    jpeg_destroy_decompress(&cinfo);
    // Se cierra el archivo de entrada
    fclose(fp);
    // En este punto se retorna 1 indicando éxito.
    return TRUE;
}