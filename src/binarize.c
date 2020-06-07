#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <jpeglib.h>
#include <structs.h>
#include <binarize.h>

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
