#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <jpeglib.h>
#include <inttypes.h>
#include <structs.h>
#include <pipeline.h>

void initPipeline(int numberImages, int binarizationThreshold, int classificationThreshold, char* maskFilename, int flagShowResults){
    printf("hola");
    for (int i = 1; i <= numberImages; i++)
    {
        //read()
        readImage(i);
    }
}

int readJPG(char* filename, Image* image, struct jpeg_error_mgr* jerr){
    

    struct jpeg_decompress_struct cinfo;
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
    
    return 1;
}

void printPixels(Image image){
    
    printf("width = %" PRIu32 "\n", image.width);
    printf("height = %" PRIu32 "\n", image.height);
    printf("channels = %" PRIu32 "\n", image.color_channel);

    
    uint8_t num = 0;
    int loc = 0;

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
        printf("fallo readJPG");
    }

    printPixels(image);

    if(image.image_buffer != NULL){
        free(image.image_buffer);
        image.image_buffer = NULL;
    }
    return image;
}