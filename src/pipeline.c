#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <pipeline.h>
#include <structs.h>
#include <inttypes.h>
#include <jpeglib.h>


void initPipeline(int numberImages, int binarizationThreshold, int classificationThreshold, char* maskFilename, int flagShowResults){
    printf("hola");
    for (int i = 1; i <= numberImages; i++)
    {
        //read()
        printf("vamos bien");
        readImage(i);
    }
}

int readJPG(char* filename, Image* image, struct jpeg_error_mgr* jerr){
    

    /*struct jpeg_decompress_struct cinfo;
    jpeg_create_decompress(&cinfo);
    cinfo.err = jpeg_std_error(jerr);

    FILE *fp = fopen(filename, "rb");
    if (fp == NULL) {
        printf("Error al abrir el archivo %s\n", filename);
        return 0;
    }

    jpeg_stdio_src(&cinfo, fp);

    jpeg_read_header(&cinfo, 1);

    jpeg_start_decompress(&cinfo);

    image->height = cinfo.image_height;
    image->width = cinfo.image_width;
    image->color_channel = cinfo.num_components;

    //image->image_buffer = NULL;
    //image->image_buffer = (int*)malloc(sizeof(int)*image->height*image->width*image->color_channel);
    image->image_buffer = (JSAMPLE*) malloc(sizeof(int)  *
                                       image->width  *
                                       image->height *
                                       image->color_channel);*/

    return 1;
}

Image readImage(int imageNumber){
    char fileName[40];
    char imageName[40];
    sprintf(fileName, "./imagen_%i.jpg", imageNumber);
    sprintf(imageName, "image_%i.jpg", imageNumber);
    printf("Filename: %s\n",fileName);
    printf("imageName: %s\n",imageName);
    Image image;
    struct jpeg_error_mgr jerr;

    if (!readJPG(fileName, &image, &jerr)){
        printf("fallo");
    }
    return image;
}