#ifndef _CONVOLUTION_H
#define _CONVOLUTION_H

Image applyLaplacianFilter(Image image, char* maskFilename);
int** getLaplacianFilter(char* maskFilename);
int** getLaplacianFilterMask(FILE* maskFile);
int** getImageMatrix(int width, int height, JSAMPLE* buffer);
int** createMatrix(int n, int m);
int** appendMatrix(int** matrix, int posX, int posY, int value);
void showMatrix(int n, int m, int** matrix);
void freeMatrix(int** matrix, int row);

#endif