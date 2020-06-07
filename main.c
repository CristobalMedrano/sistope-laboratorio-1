#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <ctype.h>
#include <jpeglib.h>
#include <getopt.h>
#include <string.h>
#include <constants.h>
#include <structs.h>
#include <functions.h>
#include <menu.h>
#include <pipeline.h>
#include <options.h>

int main(int argc, char *argv[]) {
    // Options
    int numberImages = 0;
    int binarizationThreshold = 0;
    int classificationThreshold = 0;
    char* maskFilename = NULL;
    int flagShowResults = FALSE;
    int isValidOpt = FALSE;

    getOptionsArguments(argc, argv, &numberImages, &binarizationThreshold, &classificationThreshold, &maskFilename, &flagShowResults);
    isValidOpt = validateArgs(numberImages, binarizationThreshold, classificationThreshold, maskFilename);

    if (isValidOpt == TRUE)
    {
        initPipeline(numberImages, binarizationThreshold, classificationThreshold, maskFilename, flagShowResults);
        free(maskFilename);
        return 0;
    } else {
        free(maskFilename);
        return 0;
    }
}
