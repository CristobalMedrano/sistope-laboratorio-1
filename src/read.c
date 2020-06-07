#include <stdlib.h>
#include <stdio.h>
#include <jpeglib.h>
#include <inttypes.h>
#include <structs.h>
#include <constants.h>
#include <functions.h>
#include <read.h>

Image readImage(int imageNumber){
    Image image;
    char fileName[40] = "";
    char imageName[40] = "";
    sprintf(fileName, "./imagen_%i.jpg", imageNumber);
    sprintf(imageName, "image_%i.jpg", imageNumber);

    struct jpeg_error_mgr jerr;

    if (!readJPG(fileName, &image, &jerr)){
        printf("fallo readJPG");
    }
    //printPixels(image);
    return image;
}

int readJPG(char* filename, Image* image, struct jpeg_error_mgr* jerr){
    struct jpeg_decompress_struct cinfo;
    cinfo.err = jpeg_std_error(jerr);

    FILE *fp = fopen(filename, "rb");
    if (fp == NULL) {
        printf("Error al abrir el archivo %s\n", filename);
        return FALSE;
    }

    jpeg_create_decompress(&cinfo);
    jpeg_stdio_src(&cinfo, fp);
    jpeg_read_header(&cinfo, 1);
    jpeg_start_decompress(&cinfo);

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
        jpeg_read_scanlines(&cinfo, &buffer, 1);
        buffer += row_stride;
    }

    jpeg_finish_decompress(&cinfo);
    jpeg_destroy_decompress(&cinfo);
    fclose(fp);
    
    return TRUE;
}

