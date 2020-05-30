#ifndef _FUNCTIONS_H
#define _FUNCTIONS_H

int validateArgs(int numberImages, int binarizationThreshold, int classificationThreshold, char* maskFilename);
int validateNumberImages(int numberImages);
int validateBinarizationThreshold(int binarizationThreshold);
int validateClassificationThreshold(int classificationThreshold);
int validateMaskFile(char* maskFilename);

#endif