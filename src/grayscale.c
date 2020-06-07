#include <stdlib.h>
#include <stdio.h>
#include <jpeglib.h>
#include <inttypes.h>
#include <structs.h>
#include <grayscale.h>

Image convertGrayScale(Image image){

    int totalComponents = 0;
    int R = 0;    
    int G = 0;    
    int B = 0;
    int Y = 0;

    totalComponents = image.height*image.width*image.color_channel;
    
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