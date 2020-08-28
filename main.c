// Bibliotecas utilizadas
#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <jpeglib.h>
#include <constants.h>
#include <options.h>
#include <functions.h>
#include <unistd.h>

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

    // Pipeline de 6 etapas.
    /*int i = 0;
    Image normalImage = {};
    Image grayScaleImage = {};
    Image laplacianFilterImage = {};
    Image binarizedImage = {};
    int isNearlyBlack = FALSE;*/

    // Obtencion de los argumentos ingresados por el usuario.
    getOptionsArguments(argc, argv, &numberImages, &binarizationThreshold, &classificationThreshold, &maskFilename, &flagShowResults);
    // Se validan los argumentos obtenidos de la funcion anterior
    isValidOpt = validateArgs(numberImages, binarizationThreshold, classificationThreshold, maskFilename);

    // Si los argumentos son correctos se inicia el pipeline
    if (isValidOpt == TRUE)
    {
        // Se realiza un EXEC para reemplazar este proceso con la primera etapa del pipeline
        showImageResultTitle(numberImages, flagShowResults);
        char *args[] = {"read.out", argv[2], argv[4], argv[6], argv[8], argv[9], NULL};
        execvp("src/pipeline/read.out", args);
       
        // Se retorna 0 indicando que la ejecucion fue correcta.*/
        free(maskFilename);
        return SUCCESS;
    } else {
        // Se verifica que el nombre del archivo fue creado y se libera.
        if (maskFilename != NULL) free(maskFilename);
        // Se termina la ejecucion del programa, indicando de que hubo un error retornando 0.
        return FAILURE;
    }
}

