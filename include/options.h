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
int IsValidMatrixMask(FILE* maskFile);
int getRowCount(FILE* maskFile);
int getColCount(char* string, char* separator);
int isValidColumns(FILE* maskFile, int rowCount);
void strip(char* string, char* character);
int* splitToNumber(char* string, char* separator);
int* newArray (int initalLen);
int arrayLen(int* array);
int* arrayAppend(int* array, int pos, int number);
int isValidFile(char* filename);
#endif