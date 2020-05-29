#ifndef _FUNCTIONS_H
#define _FUNCTIONS_H

#define FALSE 0
#define TRUE 1

int validateArgs(int numberImages, int binarizationThreshold, int classificationThreshold, char* maskFilename);
int validateNumberImages(int numberImages);
int validateBinarizationThreshold(int binarizationThreshold);
int validateClassificationThreshold(int classificationThreshold);
int validateMaskFile(char* maskFilename);

#endif