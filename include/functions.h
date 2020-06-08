#ifndef _FUNCTIONS_H
#define _FUNCTIONS_H

void printPixels(Image image);
void strip(char* string, char* character);
int** createMatrix(int n, int m);
int** appendMatrix(int** matrix, int posX, int posY, int value);
void showMatrix(int n, int m, int** matrix);
void freeMatrix(int** matrix, int row);

#endif