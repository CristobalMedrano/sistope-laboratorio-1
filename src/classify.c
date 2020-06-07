#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <jpeglib.h>
#include <structs.h>
#include <constants.h>
#include <classify.h>

int classifyImage(Image image, int classificationThreshold){

    int len = image.height * image.width;
    int count = 0;

    for (int i = 0; i < len; i++){
        if(image.image_buffer[i] == 0){
            count++;
        }
    }

    float blacknessPercentage = (count*100)/len;

    if (blacknessPercentage >= classificationThreshold){
        return TRUE;
    }
    else{
        return FALSE;
    }
}

void showImageResultTitle(int numberImages, int flagShowResults){
    if (flagShowResults == TRUE){
        printf("\n");
        if (BORDER == TRUE) {
            if (numberImages >= 10){
                printf("|-----------------|---------------------|\n");
            }else {
                printf("|----------------|---------------------|\n");
            }
        }
        if (numberImages >= 10){
            printf("|      image      |     nearly black    |\n");
            printf("|-----------------|---------------------|\n");
        } else {
            printf("|      image     |     nearly black    |\n");
            printf("|----------------|---------------------|\n");
        }
    }
}
void showImageResultBody(int numberImages, int imageNumber, int flagShowResults, int isNearlyBlack) {
    if (flagShowResults == TRUE){
        if (imageNumber >= 10){
            if(isNearlyBlack == TRUE){
                printf("|     image_%d   |         yes         |", imageNumber);
            }
            else{
                printf("|     image_%d   |         no          |", imageNumber);
            }
        } else {
            if(isNearlyBlack == TRUE){
                printf("|     image_%d    |         yes         |", imageNumber);
            }
            else{
                printf("|     image_%d    |         no          |", imageNumber);
            }
        }
        if (BORDER == TRUE && numberImages == imageNumber){
            if (numberImages == 10){
                printf("\n|-----------------|---------------------|");
            } else {
                printf("\n|----------------|---------------------|");
            }
        }
        if (numberImages == imageNumber){
            printf("\n");
        }
        printf("\n");
    }
}
