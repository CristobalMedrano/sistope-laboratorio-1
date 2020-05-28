#include <stdio.h>
#include <stdlib.h>
#include <getopt.h>
#include <string.h>
#include <functions.h>

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
    int validOpt = validateArgs(numberImages, binarizationThreshold, classificationThreshold, maskFilename);
    printf("opciones validas: %d\n", validOpt);

    free(maskFilename);
    /*
    for (int i = 1; i <= numberImages; i++){
        char imageName[40];
        char dir[40] = "imagen_";
        char imageNumber[100];
        int rows, columns;
        sprintf(imageNumber, "%d", i);
        strcpy(imageName, strcat(strcat(dir, imageNumber), ".jpg"));
        FILE * fp;
        fp = fopen(imageName, "rb");

        if (! fp) {
            printf("Error. Archivo no encontrado / invalido.");
            abort();
        }
        fclose(fp);
    }*/
    return 0;
    //menu(numberImages, binarizationThreshold, classificationThreshold, maskFilename, flagShowResults);
}
