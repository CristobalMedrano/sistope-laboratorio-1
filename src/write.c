//Librerías utilizadas
#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <string.h>
#include <jpeglib.h>
#include <structs.h>
#include <constants.h>
#include <write.h>

//Entradas:
//  Image image: Estructura con la imagen a escribir.
//  int imageNumber: Entero que representa el número asociado a la imagen.
//
//Funcionamiento: Procedimiento que realiza la segunda parte de la quinta etapa del pipeline, es decir, escribir en disco
//                la imagen resultante en formato jpg.
//Salida: No tiene por ser un procedimiento.
void writeImage(Image image, int imageNumber){
    // Se procede a generar y dar formato al nombre del archivo de salida
    char filename[50] = "";
    char number[100];
    sprintf(number, "%d", imageNumber);
    strcat(filename, "salida_imagen_");
    strcat(filename, number);
    strcat(filename, ".jpg");
    // Estructura para el manejador de errores JPEG
    struct jpeg_error_mgr jerr;
    // Se realiza la escritura y valida su funcionamiento
    if (!writeJPG(&image, imageNumber, filename, &jerr)){
        // En caso de error, se muestra mensaje por pantalla.
        printf("fallo writeJPG");
    }
}

//Entradas:
// Image* image: Puntero a estructura imagen.
// int imageNumber: Entero que representa el número de una imagen.
// char* filename: String que contiene el nombre del archivo a escribir.
// struct jpeg_error_mgr* jerr: estructura que representa el manejador de errores de jpeglib.
//
//Funcionamiento: Función que presenta la rutina que permite la escritura o compresión JPEG y ocupa elementos de jpeglib.
//
//Salida: Devuelve un entero. Si retorna verdadero (TRUE 1), representa éxito en la lectura. 
//         Por el contrario, falso (FALSE 0) en caso de error.
int writeJPG(Image* image, int imageNumber, char* filename, struct jpeg_error_mgr* jerr){
    // Estructura que contiene los parámetros y punteros de compresión JPEG 
    // al espacio de trabajo, definidos según jpeglib.h
    struct jpeg_compress_struct cinfo;
    
    // Se abre el archivo de entrada modo escritura con "b" para leer archivos binarios.
    FILE *fp = fopen(filename, "wb");
    if (fp == NULL) {
        printf("Error al escribir el archivo %s\n", filename);
        return FALSE;
    }

    // Se configuran las rutinas de error JPEG normales.
    cinfo.err = jpeg_std_error(jerr);

    // Se inicializa el objeto de compresión JPEG.
    jpeg_create_compress(&cinfo);
    jpeg_stdio_dest(&cinfo, fp);

    // Se establecen los parámetros para la compresión
    cinfo.image_width = image->width;
    cinfo.image_height = image->height;
    cinfo.input_components = image->color_channel;
    //cinfo.in_color_space = JCS_GRAYSCALE;

    // También se establecen aquellos parámetros de compresión predeterminados.
    jpeg_set_defaults(&cinfo);

    // Se inicia la compresión
    jpeg_start_compress(&cinfo, TRUE);
    
    int row_stride = image->width * image->color_channel;
    JSAMPLE* buffer = (JSAMPLE*)image->image_buffer;
    int i = 0;
    // Se realiza la escritura escaneando líneas, teniendo un único elemento de largo
    for (i=0; i< image->height; i++) {
        jpeg_write_scanlines(&cinfo, &buffer, 1);
        buffer += row_stride;
    }

    // Se finaliza la compresión
    jpeg_finish_compress(&cinfo);
    // Se libera memoria del objeto de compresión
    jpeg_destroy_compress(&cinfo);
    // Se cierra el archivo
    fclose(fp);
    // En este punto, se retorna (TRUE 1) indicando éxito.
    return TRUE;
}