#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <string.h>
#include <constants.h>
#include <jpeglib.h>
#include <inttypes.h>
#include <structs.h>
#include <pipeline.h>

void initPipeline(int numberImages, int binarizationThreshold, int classificationThreshold, char* maskFilename, int flagShowResults){
    for (int i = 1; i <= numberImages; i++)
    {
        //read()
        Image image = readImage(i);
        writeImage(image, i);
    }
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

    return image;
}

void writeImage(Image image, int imageNumber){
    char filename[50] = "";
    char number[100];
    sprintf(number, "%d", imageNumber);
    strcat(filename, "salida_imagen_");
    strcat(filename, number);
    strcat(filename, ".jpg");

    struct jpeg_error_mgr jerr;

    if (!writeJPG(&image, imageNumber, filename, &jerr)){
        printf("fallo readJPG");
    }else
    {
        printf("imagen guardada con exito.\n");
        if(image.image_buffer != NULL){
            free(image.image_buffer);
            image.image_buffer = NULL;
        }
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
    cinfo.in_color_space = JCS_RGB;

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