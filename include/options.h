#ifndef _OPTIONS_H
#define _OPTIONS_H

void getOptionsArguments(int argc, char *argv[], int* numberImages, int* binarizationThreshold, int* classificationThreshold, char** maskFilename, int* flagShowResults);
void showCorrectUse(char *argv[]);
int isInteger(char* optarg, char* type);
int validateArgs(int numberImages, int binarizationThreshold, int classificationThreshold, char* maskFilename);
int validateNumberImages(int numberImages);
int validateBinarizationThreshold(int binarizationThreshold);
int validateClassificationThreshold(int classificationThreshold);
int validateMaskFile(char* maskFilename);
int isValidFile(char* filename);
int isValidMatrixMask(FILE* maskFile);
int isValidColumns(FILE* maskFile, int rowCount);
int getRowCount(FILE* maskFile);
int getColCount(char* string, char* separator);
void strip(char* string, char* character);
#endif