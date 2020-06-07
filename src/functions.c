#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <jpeglib.h>
#include <inttypes.h>
#include <string.h>
#include <constants.h>
#include <structs.h>
#include <functions.h>

void printPixels(Image image){
    
    printf("width = %" PRIu32 "\n", image.width);
    printf("height = %" PRIu32 "\n", image.height);
    printf("channels = %" PRIu32 "\n", image.color_channel);
    
    uint8_t num = 0;
    int loc = 0;

    for (int i = 0; i < image.height; i++)
    {
        for (int j = 0; j < image.width*image.color_channel; j++)
        {
            num = image.image_buffer[loc];
            printf("%" PRId8 " ", num);
            loc++;
        }
        printf("\n");
    }
}

void strip(char* string, char* character){
    char* token = NULL;
    if (string != NULL){
        token = strtok(string, character);
        while( token != NULL ) {
            token = strtok(NULL, character);
        }
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