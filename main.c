#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <jpeglib.h>
#include <getopt.h>
#include <string.h>
#include <constants.h>
#include <structs.h>
#include <functions.h>
#include <menu.h>
#include <pipeline.h>

int main(int argc, char *argv[]) {
    int numberImages = 0;
    char* maskFilename = NULL;
    int classificationThreshold = 0;
    int binarizationThreshold = 0;
    int flagShowResults = 0;
    int opt = 0;
    while ((opt = getopt (argc, argv, "c:u:n:m:b")) != -1)
    {
      switch (opt){
        case 'c':
            numberImages = atoi(optarg);		  //cantidad de imágenes.
            printf("Cantidad imagenes: %d\n", numberImages);
            break;
        case 'u':
            binarizationThreshold = atoi(optarg);	      //umbral binarizar la imagen.
            printf("Umbral para binarizar: %d\n", binarizationThreshold);
            break;
        case 'n':
            classificationThreshold = atoi(optarg);	      //umbral clasificacion.
            printf("Umbral de clasificacion: %d\n", classificationThreshold);
            break;
        case 'm':
            maskFilename = (char*)malloc(sizeof(char)*40);
            maskFilename = strcpy(maskFilename, optarg);		      //máscara.
            printf("Mascara: %s\n", maskFilename);
            break;
        case 'b':
            flagShowResults = 1;				     //bandera.
            printf("Bandera: %d\n", flagShowResults);
            break;
        default:
            break;
      }
    }

    // sigue el programa (aqui valida la salida del getopt)
    int isValidOpt = validateArgs(numberImages, binarizationThreshold, classificationThreshold, maskFilename);

    if (isValidOpt == TRUE)
    {
        initPipeline(numberImages, binarizationThreshold, classificationThreshold, maskFilename, flagShowResults);
        /*showMenu();
        int option;
        do
        {
            option = NO_INPUT;
            menu(&option, 1, 3);
    
            switch(option)
            {
                case RUN:
                    initPipeline(numberImages, binarizationThreshold, classificationThreshold, maskFilename, flagShowResults);
                    pressToContinue();
                    break;
    
                case ABOUT:
                    about();
                    break;
    
                case EXIT:
                    printf("Programa finalizado.\n");
                    break;
            }

        }while(option != EXIT);*/
        free(maskFilename);
        return 0;
    } else {
        free(maskFilename);
        return 0;
    }
}
