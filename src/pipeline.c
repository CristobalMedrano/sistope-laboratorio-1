#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <string.h>
#include <constants.h>
#include <jpeglib.h>
#include <inttypes.h>
#include <structs.h>
#include <convolution.h>
#include <pipeline.h>

void initPipeline(int numberImages, int binarizationThreshold, int classificationThreshold, char* maskFilename, int flagShowResults){
    Image normalImage;
    Image grayScaleImage;
    Image laplacianFilterImage;
    Image binarizedImage;
    for (int i = 1; i <= numberImages; i++)
    {
        //read()
        normalImage = readImage(i);
        grayScaleImage = convertGrayScale(normalImage);
        laplacianFilterImage = applyLaplacianFilter(grayScaleImage, maskFilename);
        binarizedImage = binarizeImage(laplacianFilterImage, binarizationThreshold);

        writeImage(binarizedImage, i);

        free(normalImage.image_buffer);
        free(grayScaleImage.image_buffer);
        free(laplacianFilterImage.image_buffer);
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

    //printPixels(image);

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
        printf("fallo writeJPG");
    }else
    {
        printf("imagen guardada con exito.\n");
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

Image convertGrayScale(Image image){

    int totalComponents = image.height*image.width*image.color_channel;
    int R, G, B;
    int Y;

    Image convertedImage;
    convertedImage.height = image.height;
    convertedImage.width = image.width;
    convertedImage.color_channel = 1;
    convertedImage.image_buffer = NULL;
    convertedImage.image_buffer = (JSAMPLE*) malloc(sizeof(int) *
                                       convertedImage.width  *
                                       convertedImage.height *
                                       convertedImage.color_channel);
    
    int pos = 0;
    for (int i = 0; i < totalComponents; i+=3){
        R = image.image_buffer[i];
        G = image.image_buffer[i+1];
        B = image.image_buffer[i+2];
        Y = R*0.3 + G*0.59 + B*0.11; // ecuación de luminiscencia
        convertedImage.image_buffer[pos] = Y;
        pos++;
    }

    return convertedImage;
}

Image binarizeImage(Image image, int binarizationThreshold){
    int len = image.height * image.width;

    for (int i = 0; i < len; i++){
        if (image.image_buffer[i] > binarizationThreshold){
            image.image_buffer[i] = 255;
        }
        else{
            image.image_buffer[i] = 0;
        }
    }

    return image;
}