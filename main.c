#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <jpeglib.h>
#include <constants.h>
#include <options.h>

//Entradas: argc: Numero de argumentos ingresados al momento de ejecutar el programa.
//          argv: Arreglo con los argumentos del programa.
//
//Funcionamiento: Funcion principal del programa, en esta funcion se lleva a cabo el pipeline de 6 etapas.
//
//Salidas: Retorna 0 si la ejecución fue correcta y 1 si la ejecución fue incorrecta.
int main(int argc, char *argv[]) {
    // Opciones ingresadas por el usuario.
    int numberImages = 0;
    int binarizationThreshold = 0;
    int classificationThreshold = 0;
    char* maskFilename = NULL;
    int flagShowResults = FALSE;
    int isValidOpt = FALSE;

    // Obtencion de los argumentos ingresados por el usuario.
    getOptionsArguments(argc, argv, &numberImages, &binarizationThreshold, &classificationThreshold, &maskFilename, &flagShowResults);
    // Se validan los argumentos obtenidos de la funcion anterior
    isValidOpt = validateArgs(numberImages, binarizationThreshold, classificationThreshold, maskFilename);

    // Si los argumentos son correctos se inicia el pipeline
    if (isValidOpt == TRUE)
    {
        /*
        // Se muestra el titulo de salida (Si es que el usuario asi lo ingreso). 
        showImageResultTitle(numberImages, flagShowResults);
        // Para cada imagen se le realiza el pipeline.
        for (i = 1; i <= numberImages; i++)
        {
            // Se obtiene la imagen desde el archivo.
            normalImage = readImage(i);
            // Se convierte la imagen a escala de grises.
            grayScaleImage = convertGrayScale(normalImage);
            // Se aplica el filtro laplaciano a la imagen en escala de grises.
            laplacianFilterImage = applyLaplacianFilter(grayScaleImage, maskFilename);
            // Se binariza la imagen segun el umbral ingresado por el usuario.
            binarizedImage = binarizeImage(laplacianFilterImage, binarizationThreshold);
            // Se analiza la propiedad segun el umbral ingresado por el usuario.
            isNearlyBlack = classifyImage(binarizedImage, classificationThreshold);

            // Si el usuario lo indica, se muestran los resultados por pantalla.
            showImageResultBody(numberImages, i, flagShowResults, isNearlyBlack);
            // Se escribe la imagen resultante en disco.
            writeImage(binarizedImage, i);

            // Se libera la memoria utilizada, principalmente el buffer de cada imagen.
            free(normalImage.image_buffer);
            free(grayScaleImage.image_buffer);
            free(laplacianFilterImage.image_buffer);
        }
        // Se libera la memoria utilizada para obtener el nombre de la mascara.
        free(maskFilename);
        // Se retorna 0 indicando que la ejecucion fue correcta. */
        free(maskFilename);
        return SUCCESS;
    } else {
        // Se verifica que el nombre del archivo fue creado y se libera.
        if (maskFilename != NULL) free(maskFilename);
        // Se termina la ejecucion del programa, indicando de que hubo un error retornando 0.
        return FAILURE;
    }
}

