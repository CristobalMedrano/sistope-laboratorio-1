#ifndef _CONVOLUTION_H
#define _CONVOLUTION_H

Image applyLaplacianFilter(Image image, char* maskFilename);
int** getLaplacianFilter(char* maskFilename);
int** getLaplacianFilterMask(FILE* maskFile);
int** getImageMatrix(int height, int width, JSAMPLE* buffer);

#endif