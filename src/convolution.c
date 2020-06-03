#include <stdio.h>
#include <stdlib.h>
#include <jpeglib.h>
#include <inttypes.h>
#include <constants.h>
#include <string.h>
#include <structs.h>
#include <functions.h>
#include <convolution.h>

Image applyLaplacianFilter(Image image, char* maskFilename){
    Image convertedImage;
    int** laplacianFilter = NULL;
    int** imageMatrix = NULL;    
    int width = 0;
    int height = 0;
    JSAMPLE* buffer = NULL;
    int i = 0;
    int j = 0;
    int loc = 0;
    int pixel = 0;

    width = image.width + 2;
    height = image.height + 2;
    buffer = image.image_buffer;

    laplacianFilter = getLaplacianFilter(maskFilename);
    imageMatrix = getImageMatrix(width, height, buffer);

    convertedImage.height = image.height;
    convertedImage.width = image.width;
    convertedImage.color_channel = 1;
    convertedImage.image_buffer = NULL;
    convertedImage.image_buffer = (JSAMPLE*) malloc(sizeof(int) *
                                    convertedImage.width  *
                                    convertedImage.height *
                                    convertedImage.color_channel);
    
    for (i = 1; i < width - 1; i++)
    {
        for (j = 1; j < height - 1; j++)
        {
            pixel = laplacianFilter[0][0]*imageMatrix[i-1][j-1] +
                    laplacianFilter[0][1]*imageMatrix[i-1][j]   +
                    laplacianFilter[0][2]*imageMatrix[i-1][j+1] +
                    laplacianFilter[1][0]*imageMatrix[i][j-1]   +
                    laplacianFilter[1][1]*imageMatrix[i][j]     +
                    laplacianFilter[1][2]*imageMatrix[i][j+1]   +
                    laplacianFilter[2][0]*imageMatrix[i+1][j-1] +
                    laplacianFilter[2][1]*imageMatrix[i+1][j]   +
                    laplacianFilter[2][2]*imageMatrix[i+1][j+1] ;
            convertedImage.image_buffer[loc] = pixel;
            loc++;
        }
    }

    //showMatrix(MASK_ROW, MASK_COL, laplacianFilter);
    //showMatrix(width, height, imageMatrix);

    freeMatrix(imageMatrix, width);
    freeMatrix(laplacianFilter, MASK_ROW);
    return convertedImage;
}

int** getImageMatrix(int width, int height, JSAMPLE* buffer){
    int** imageMatrix = NULL;
    int i = 0;
    int j = 0;
    int loc = 0;

    imageMatrix = createMatrix(width, height);
    if (imageMatrix != NULL){
        for (i = 1; i < width-1; i++)
        {
            for (j = 1; j < height-1; j++)
            {
                imageMatrix = appendMatrix(imageMatrix, i, j, buffer[loc]);
                loc++;
            }
        }
        return imageMatrix;

    } else {
        return NULL;
    }
}

int** getLaplacianFilter(char* maskFilename){
    int** laplacianFilterMask = NULL;
    FILE* maskFile = NULL;
    
    if (maskFilename == NULL){
        return NULL;
    } else {
        maskFile = fopen(maskFilename, "rb");
        if (maskFile != NULL){
            laplacianFilterMask = getLaplacianFilterMask(maskFile);
            fclose(maskFile);
            if (laplacianFilterMask != NULL){
                return laplacianFilterMask;
            } else {
                return NULL;
            }
        } else {
            return NULL;
        }
        return NULL;
    }
}

int** getLaplacianFilterMask(FILE* maskFile){
    int** laplacianFilterMask = NULL;
    char* token = NULL;
    char string[256] = "";
    int rows = 3;
    int cols = 3;
    int i = 0;
    laplacianFilterMask = createMatrix(rows, cols);
    if (laplacianFilterMask != NULL){
        for ( i = 0; i < rows; i++)
        {
            if( fgets (string, 256, maskFile) != NULL ){
                strip(string, "\n");
                strip(string, "\r");
                token = strtok(string, " ");
                laplacianFilterMask = appendMatrix(laplacianFilterMask, i, 0, atoi(token));
                token = strtok(NULL, " ");
                laplacianFilterMask = appendMatrix(laplacianFilterMask, i, 1, atoi(token));
                token = strtok(NULL, " ");
                laplacianFilterMask = appendMatrix(laplacianFilterMask, i, 2, atoi(token));
            }
        }
        return laplacianFilterMask;
    } else {
        return NULL;
    }
}

int** createMatrix(int n, int m){
    int i = 0;
    int** matrix = NULL;
    
    matrix = (int **)calloc(n, sizeof(int *));
                    for (i = 0; i < n; i++)
                    matrix[i] = (int *)calloc(m, sizeof(int));
    if(NULL != matrix)
    {
        return matrix;
    }
    printf("Memoria insuficiente: createMatrix()\n");
    printf("Error: convolution.c\n");
    return NULL;
}

int** appendMatrix(int** matrix, int posX, int posY, int value){
    if (matrix == NULL || posX < 0 || posY < 0){
        return NULL;
    } else {
        matrix[posX][posY] = value;
        return matrix;
    }
}

void showMatrix(int n, int m, int** matrix){
    if(NULL != matrix && n >= 0 && m >= 0)
    {
        int i = 0;
        for(i = 0; i < n; i++)
        {
            int j = 0;
            for(j = 0; j < m; j++)
            {
                if (matrix[i][j] >= 100) 
                {
                    printf("%d  ", matrix[i][j]);
                }
                else if (matrix[i][j] >= 10) 
                {
                    printf("%d   ", matrix[i][j]);
                }
                else if (matrix[i][j] <= 9 && matrix[i][j] >= 0)
                {
                    printf("%d    ", matrix[i][j]);
                } 
                else if (matrix[i][j] < 0 && matrix[i][j] >= -9)
                {
                    printf("%d   ", matrix[i][j]);
                }
                else if (matrix[i][j] >= -10)
                {
                    printf("%d  ", matrix[i][j]);
                }
                else if (matrix[i][j] >= -100)
                {
                    printf("%d ", matrix[i][j]);
                } else {
                    printf("%d", matrix[i][j]);
                }
            }
            printf("\n");
        }
    }   
}

void freeMatrix(int** matrix, int row)
{
    if (NULL != matrix) 
    {   
        int i = 0;
        for(i = 0; i < row; i++)
        {
            free(matrix[i]);
        }
    }
    free(matrix);
}