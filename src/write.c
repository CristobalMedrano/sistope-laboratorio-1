#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <string.h>
#include <jpeglib.h>
#include <structs.h>
#include <constants.h>
#include <write.h>

void writeImage(Image image, int imageNumber){
    char filename[50] = "";
    char number[100];
    sprintf(number, "%d", imageNumber);
    strcat(filename, "salida_imagen_");
    strcat(filename, number);
    strcat(filename, ".jpg");

    struct jpeg_error_mgr jerr;

    if (!writeJPG(&image, imageNumber, filename, &jerr)){
        printf("fallo writeJPG");
    }
}

int writeJPG(Image* image, int imageNumber, char* filename, struct jpeg_error_mgr* jerr){
    struct jpeg_compress_struct cinfo;
    
    FILE *fp = fopen(filename, "wb");
    if (fp == NULL) {
        printf("Error al escribir el archivo %s\n", filename);
        return FALSE;
    }

    jpeg_create_compress(&cinfo);
    jpeg_stdio_dest(&cinfo, fp);

    cinfo.image_width = image->width;
    cinfo.image_height = image->height;
    cinfo.input_components = image->color_channel;
    //cinfo.in_color_space = JCS_GRAYSCALE;

    cinfo.err = jpeg_std_error(jerr); 
    jpeg_set_defaults(&cinfo);
    jpeg_start_compress(&cinfo, TRUE);
    
    int row_stride = image->width * image->color_channel;
    JSAMPLE* buffer = (JSAMPLE*)image->image_buffer;
    int i = 0;
    for (i=0; i< image->height; i++) {
        jpeg_write_scanlines(&cinfo, &buffer, 1);
        buffer += row_stride;
    }

    jpeg_finish_compress(&cinfo);
    jpeg_destroy_compress(&cinfo);
    fclose(fp);
    
    return TRUE;
}