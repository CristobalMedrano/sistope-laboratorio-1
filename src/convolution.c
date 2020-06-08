#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <jpeglib.h>
#include <constants.h>
#include <string.h>
#include <structs.h>
#include <functions.h>
#include <convolution.h>

//Entradas: image (Image): Estructura con la imagen proveniente de la segunda etapa (escala de grises).
//          maskFilename (char*): String que contiene el nombre de la mascara de filtro laplaciano.
//
//Funcionamiento: Se aplica el filtro laplaciano obtenido desde el archivo de mascara a la imagen en escala
//                de grises, el resultado es una imagen (image) con la convolucion aplicada.
//
//Salidas: Retorna una estructura con la imagen luego de aplicar el filtro laplaciano.
Image applyLaplacianFilter(Image image, char* maskFilename){
    // Se inicializan las variables a utilizar
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

    // Se calcula el nuevo ancho y alto temporal de la imagen
    // Esto busca principalmente agregar un borde completo a la imagen
    // de solo 0.
    width = image.width + 2;
    height = image.height + 2;
    buffer = image.image_buffer;

    // Se obtiene la matriz del filtro laplaciano que se encuentra en el archivo.
    laplacianFilter = getLaplacianFilter(maskFilename);
    // Se genera una matriz de la imagen que esta en escala de grises con un borde de 0.
    imageMatrix = getImageMatrix(height, width, buffer);

    // Se inicia una nueva imagen para guardar los resultados del filtro laplaciano.
    convertedImage.height = image.height;
    convertedImage.width = image.width;
    convertedImage.color_channel = 1;
    convertedImage.image_buffer = NULL;
    convertedImage.image_buffer = (JSAMPLE*) malloc(sizeof(int) *
                                    convertedImage.width  *
                                    convertedImage.height *
                                    convertedImage.color_channel);
    
    // Se recorre la matriz de la imagen en escala de grises
    // Las filas y columnas comienzan de 1 para asi ignorar el borde de 0.
    for (i = 1; i < height - 1; i++)
    {
        for (j = 1; j < width - 1; j++)
        {
            // Se aplica la convolucion.
            pixel = laplacianFilter[0][0]*imageMatrix[i-1][j-1] +
                    laplacianFilter[0][1]*imageMatrix[i-1][j]   +
                    laplacianFilter[0][2]*imageMatrix[i-1][j+1] +
                    laplacianFilter[1][0]*imageMatrix[i][j-1]   +
                    laplacianFilter[1][1]*imageMatrix[i][j]     +
                    laplacianFilter[1][2]*imageMatrix[i][j+1]   +
                    laplacianFilter[2][0]*imageMatrix[i+1][j-1] +
                    laplacianFilter[2][1]*imageMatrix[i+1][j]   +
                    laplacianFilter[2][2]*imageMatrix[i+1][j+1] ;
            // El pixel resultante se guarda en el buffer de la imagen resultante.
            convertedImage.image_buffer[loc] = pixel;
            // Se mueve el buffer en 1.
            loc++;
        }
    }

    //showMatrix(MASK_ROW, MASK_COL, laplacianFilter);
    //showMatrix(width, height, imageMatrix);

    // Se libera la memoria utilizada para generar la matriz de la imagen (en escala de grises) 
    // con borde de 0.
    freeMatrix(imageMatrix, height);
    // Se libera la memoria utilizada para generar la matriz del filtro laplaciano.
    freeMatrix(laplacianFilter, MASK_ROW);
    // Se retorna la imagen con el filtro laplaciano aplicado.
    return convertedImage;
}

//Entradas: height (int): Entero que representa el alto de la imagen
//          width (int): Entero que representa el ancho de la imagen
//          buffer (JSAMPLE*): Arreglo que contiene la informacion de cada pixel (este valor va desde 0 a 255)
//                             JSAMPLE es un formato propio de la liberia de jpeglib.
//
//Funcionamiento: Genera una matriz de alto x ancho (height x width) con un borde de 0 
//                a partir del buffer de la imagen en escala de grises.
//
//Salidas: Retorna una matriz de alto x ancho (height x width) (int**) con la imagen en escala de grises
//         si la ejecucion fue correcta y NULL si no se pudo asignar memoria.
int** getImageMatrix(int height, int width, JSAMPLE* buffer){
    //Se inicializan los valores
    int** imageMatrix = NULL;
    int i = 0;
    int j = 0;
    int loc = 0;

    // Se crea una matriz de altoXancho iniciada en 0. Se asume que el ancho y alto ya vienen
    // modificados.
    imageMatrix = createMatrix(height, width);
    // Si se creo correctamente, la matriz se recorre.
    if (imageMatrix != NULL){
        // Para cada fila
        for (i = 1; i < height-1; i++)
        {
            // Para cada columna
            for (j = 1; j < width-1; j++)
            {
                // Se agrega a la matriz creada el buffer de la imagen en escala de grises.
                imageMatrix = appendMatrix(imageMatrix, i, j, buffer[loc]);
                loc++;
            }
        }
        // Se retorna la matriz que contiene la imagen en escala de grises con un borde de 0.
        return imageMatrix;
    // Sino
    } else {
        // se retorna NULO.
        return NULL;
    }
}

//Entradas: maskFilename (char*): String con el nombre del archivo de mascara.
//
//Funcionamiento: Obtiene a partir de un archivo de mascara, la matriz que contiene el filtro
//                laplaciano de 3x3.
//
//Salidas: Retorna una matriz de 3x3 con el filtro laplaciano del archivo.
int** getLaplacianFilter(char* maskFilename){
    // Se inicializan las variables
    int** laplacianFilterMask = NULL;
    FILE* maskFile = NULL;
    
    // Si el nombre de archivo es invalido
    if (maskFilename == NULL){
        // Retorna nulo
        return NULL;
    // Sino
    } else {
        // Se lee el archivo de mascara.
        maskFile = fopen(maskFilename, "rb");
        // Si se pudo abrir correctamente el archivo.
        if (maskFile != NULL){
            // Se obtiene la mascara de filtro laplaciano (matriz de 3x3)
            laplacianFilterMask = getLaplacianFilterMask(maskFile);
            // Se cierra el archivo
            fclose(maskFile);
            // Si la mascara obtenida no es nula
            if (laplacianFilterMask != NULL){
                // Se retorna la matriz con la mascara.
                return laplacianFilterMask;
            // Sino
            } else {
                // Se retorna nulo
                return NULL;
            }
        // Sino
        } else {
            // Se retorna nulo.
            return NULL;
        }
    }
}

//Entrada: maskFile (FILE*): Puntero que contiene la informacion del archivo de mascara.
//
//Funcionamiento: A partir de la informacion contenida en el archivo de mascara, se genera una
//                matriz de 3x3 con el filtro laplaciano leido.
//
//Salida: Matriz de 3x3 que contiene el filtro laplaciano (es un doble puntero).
int** getLaplacianFilterMask(FILE* maskFile){
    // Se inicializan las variables
    int** laplacianFilterMask = NULL;
    char* token = NULL;
    char string[256] = "";
    int rows = 3;
    int cols = 3;
    int i = 0;

    // Se crea una matriz de nxm iniciada en 0.
    laplacianFilterMask = createMatrix(rows, cols);
    // Si la matriz fue creada con exito
    if (laplacianFilterMask != NULL){
        // para cada fila en el archivo
        for ( i = 0; i < rows; i++)
        {
            // Se genera un string con toda la informacion de la fila.
            if( fgets (string, 256, maskFile) != NULL ){
                // Se le quita el salto de linea
                strip(string, "\n");
                // Se le quita el salto de linea unix
                strip(string, "\r");
                // Se separa la fila en segmentos definidos por el espacio.
                token = strtok(string, " ");
                // Se guarda el valor (token) de la primera columna de la fila en la matriz
                laplacianFilterMask = appendMatrix(laplacianFilterMask, i, 0, atoi(token));
                // Se mueve el separador al segundo segmento
                token = strtok(NULL, " ");
                // Se guarda el valor (token) de la segunda columna de la fila en la matriz
                laplacianFilterMask = appendMatrix(laplacianFilterMask, i, 1, atoi(token));
                // Se mueve el separador al tercer segmento
                token = strtok(NULL, " ");
                // Se guarda el valor (token) de la tercera columna de la fila en la matriz
                laplacianFilterMask = appendMatrix(laplacianFilterMask, i, 2, atoi(token));
            }
            // Se repite este proceso para cada fila en el archivo.
        }
        // Se retorna la matriz laplaciana
        return laplacianFilterMask;
    // Sino
    } else {
        // Se retorna nulo.
        return NULL;
    }
}