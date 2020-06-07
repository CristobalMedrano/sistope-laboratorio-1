#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <jpeglib.h>
#include <structs.h>
#include <constants.h>
#include <functions.h>
#include <options.h>
#include <read.h>
#include <grayscale.h>
#include <convolution.h>
#include <binarize.h>
#include <classify.h>
#include <write.h>

int main(int argc, char *argv[]) {
    // Options
    int numberImages = 0;
    int binarizationThreshold = 0;
    int classificationThreshold = 0;
    char* maskFilename = NULL;
    int flagShowResults = FALSE;
    int isValidOpt = FALSE;

    // Pipeline
    int i = 0;
    Image normalImage = {};
    Image grayScaleImage = {};
    Image laplacianFilterImage = {};
    Image binarizedImage = {};
    int isNearlyBlack = FALSE;

    getOptionsArguments(argc, argv, &numberImages, &binarizationThreshold, &classificationThreshold, &maskFilename, &flagShowResults);
    isValidOpt = validateArgs(numberImages, binarizationThreshold, classificationThreshold, maskFilename);

    if (isValidOpt == TRUE)
    {
        showImageResultTitle(numberImages, flagShowResults);
        for (i = 1; i <= numberImages; i++)
        {
            normalImage = readImage(i);
            grayScaleImage = convertGrayScale(normalImage);
            laplacianFilterImage = applyLaplacianFilter(grayScaleImage, maskFilename);
            binarizedImage = binarizeImage(laplacianFilterImage, binarizationThreshold);
            isNearlyBlack = classifyImage(binarizedImage, classificationThreshold);

            showImageResultBody(numberImages, i, flagShowResults, isNearlyBlack);
            writeImage(binarizedImage, i);

            free(normalImage.image_buffer);
            free(grayScaleImage.image_buffer);
            free(laplacianFilterImage.image_buffer);
        }    
        if (maskFilename != NULL) free(maskFilename);
        return SUCCESS;
    } else {
        if (maskFilename != NULL) free(maskFilename);
        return FAILURE;
    }
}

