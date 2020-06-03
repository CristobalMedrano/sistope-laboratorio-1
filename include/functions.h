#ifndef _FUNCTIONS_H
#define _FUNCTIONS_H

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
void invalidMaskFile(int isValid);

#endif