#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <jpeglib.h>
#include <structs.h>
#include <pipeline.h>

void menu(int numberImages, int binarizationThreshold, int classificationThreshold, char* maskFilename, int flagShowResults){

    int option;

    do
    {
        printf( "\n   + ----------------------------------------- +");
        printf( "\n     Laboratorio 1 - Sistemas Operativos 1-2020 \n");
        printf( "\n   1. Iniciar programa");
        printf( "\n   2. Creditos" );
        printf( "\n   3. Salir" );
        printf( "\n   + ----------------------------------------- +");
        printf( "\n\n   Introduzca una opcion (1-3): ");

        scanf("%d", &option);

        switch (option)
        {
        case 1:
            initPipeline(numberImages, binarizationThreshold, classificationThreshold, maskFilename, flagShowResults);
            break;

        case 2: 
            printf( "\n   + ----------------------------------------- +");
            printf("\n * Autores: Cristobal Medrano y Mauricio Soto \n");
            printf(" * Universidad de Santiago de Chile \n");
            break;

        default:
                if(option != 3)
                    printf("\n Error. Esta opcion no esta permitida.\n");
                break;
        }
        
    } while (option != 3);    
}