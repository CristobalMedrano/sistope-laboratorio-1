#include <stdlib.h>
#include <stdio.h>
#include <stdint.h>
#include <jpeglib.h>
#include <inttypes.h>
#include <getopt.h>
#include <string.h>
#include <ctype.h>
#include <constants.h>
#include <structs.h>
#include <functions.h>
#include <options.h>

void getOptionsArguments(int argc, char *argv[], int* numberImages, int* binarizationThreshold, int* classificationThreshold, char** maskFilename, int* flagShowResults) {
    int option = 0;
    char* endptr = NULL;
    int argument = -1;
    int isEqual = -1;

    if (argc < MIN_OPTS) {
        showCorrectUse(argv);
        exit(EXIT_FAILURE);
    } else {
        while ((option = getopt(argc, argv, "c:u:n:m:b")) != -1) {
            switch (option) {
            case 'c':
                argument = strtol(optarg, &endptr, ENDPTR_LEN);
                if (optarg != 0 && argument == 0) {
                    showCorrectUse(argv);
                    exit(EXIT_FAILURE);
                } else {
                    if (!isInteger(optarg, "la cantidad de imagenes")) exit(EXIT_FAILURE);
                    *numberImages = atoi(optarg);
                }
                //printf("Cantidad imagenes: %d\n", numberImages);
                break;
            case 'u':
                argument = strtol(optarg, &endptr, ENDPTR_LEN);
                if (optarg != 0 && argument == 0) {
                    showCorrectUse(argv);
                    exit(EXIT_FAILURE);
                } else {
                    if (!isInteger(optarg, "el umbral de binarizacion")) exit(EXIT_FAILURE);
                    *binarizationThreshold = atoi(optarg); //umbral binarizar la imagen.
                    //printf("Umbral para binarizar: %d\n", binarizationThreshold);
                }
                break;
            case 'n':
                argument = strtol(optarg, &endptr, ENDPTR_LEN);
                if (optarg != 0 && argument == 0) {
                    showCorrectUse(argv);
                    exit(EXIT_FAILURE);
                } else {
                    if (!isInteger(optarg, "el umbral de clasificacion")) exit(EXIT_FAILURE);
                    *classificationThreshold = atoi(optarg); //umbral binarizar la imagen.
                    //printf("Umbral de clasificacion: %d\n", classificationThreshold);
                }
                break;
            case 'm':
                isEqual = strcmp("mascara.txt", optarg);
                if (optarg != 0 && isEqual != 0) {
                    showCorrectUse(argv);
                    exit(EXIT_FAILURE);
                } else {
                    *maskFilename = (char * ) malloc(sizeof(char) * MASKFILENAME_LEN);
                    if (*maskFilename != NULL) *maskFilename = strcpy(*maskFilename, optarg); //máscara.

                }
                //printf("Mascara: %s\n", maskFilename);
                break;
            case 'b':
                *flagShowResults = TRUE; //bandera.
                //printf("Bandera: %d\n", flagShowResults);
                break;
            default:
                showCorrectUse(argv);
                exit(EXIT_FAILURE);
                break;
            }
        } 
    }
}

void showCorrectUse(char *argv[]){
    printf("ERROR. Se ingreso un numero incorrecto de opciones/argumentos\n");
    fprintf(stderr, "Uso: %s [-c cantidad de imagenes] [-u umbral binarizacion] [-n umbral clasificacion] [-m mascara] [-b]\n", argv[0]);
}

int isInteger(char* optarg, char* type)
{
    int i = 0;
    char dot = '.';
    char comma = ',';
    char dash = '-';
    if (optarg != NULL){
        for (i = 0; i < strlen(optarg); i++)
        {
            if (optarg[i] == dot || 
                optarg[i] == comma ||
                optarg[i] == dash ||
                isalpha(optarg[i])){
                printf("Error. El valor para %s debe ser un entero positivo.\n", type);
                return FALSE;
            }
        }
        return TRUE;
    } else {
        return FALSE;
    }
}

int validateArgs(int numberImages, int binarizationThreshold, int classificationThreshold, char* maskFilename){
    int validation1 = validateNumberImages(numberImages);
    int validation2 = validateBinarizationThreshold(binarizationThreshold);
    int validation3 = validateClassificationThreshold(classificationThreshold);
    int validation4 = validateMaskFile(maskFilename);
    if (validation1 == TRUE && validation2 == TRUE && validation3 == TRUE && validation4 == TRUE)
    {
        return TRUE;
    }else
    {
        return FALSE;
    }
}


int validateNumberImages(int numberImages){
    char filename[40] = "";
    int i = 0;
    int fileStatus = 0;
    if (numberImages <= 0){
        printf("ERROR. Cantidad de imagenes incorrecta, debe ingresar 1 o mas imagenes.\n");
        return FALSE;
	} else {
        for (i = 1; i <= numberImages; i++)
        {
            if (sprintf(filename, "imagen_%i.jpg", i) >= 0){
                fileStatus = isValidFile(filename);
                if (fileStatus == FALSE){
                    return FALSE;
                }
            }
            else{
                return FALSE;
            }
        }
        return TRUE;
    }
}

int isValidFile(char* filename){
    FILE* file = NULL;
    if(filename == NULL){
        printf("El nombre del archivo no es válido.");
        return FALSE;
    } else {
        file = fopen(filename, "rb");
        if (file != NULL){
            fclose(file);
            return TRUE;
        } else {
            printf("El archivo de imagen '%s', no existe o se encuentra en otro directorio.\n", filename);
            return FALSE;
        }
    }
}


int validateBinarizationThreshold(int binarizationThreshold){
    if (binarizationThreshold < 0){
        printf("ERROR. Umbral de binarizacion invalido (negativo).\n");
        return FALSE;
    }
    else if (binarizationThreshold > 255){
        printf("ERROR. Umbral de binarizacion invalido (sobre 255).\n");
        return FALSE;
    }
    else
    {
        return TRUE;
    }    
}

int validateClassificationThreshold(int classificationThreshold){
    if (classificationThreshold < 0){
        printf("ERROR. Umbral de clasificacion invalido (negativo).\n");
        return FALSE;
    }
    else if (classificationThreshold > 100){
        printf("ERROR. Umbral de clasificacion invalido (sobre 100).\n");
        return FALSE;
    }
    else{
        return TRUE;
    }
}

int validateMaskFile(char* maskFilename){
    FILE* maskFile = fopen(maskFilename, "rb");
    int isValid = FALSE;
    if (maskFile == NULL) {
        printf("ERROR. Archivo de mascara no existe.\n");
        return FALSE;
    } else {
        isValid = isValidMatrixMask(maskFile);
        if (isValid == FALSE){
            printf("ERROR. El archivo de mascara no es valido.\n");
            printf("Verifique que la matriz sea de 3x3.\n");
        }
        fclose(maskFile);
        return isValid;
    }
}

int isValidMatrixMask(FILE* maskFile){
    int row = 0;
    int valid = FALSE;
    if (maskFile == NULL){
        return FALSE;
    } else {
        row = getRowCount(maskFile);
        if (row == MASK_ROW){
            valid = isValidColumns(maskFile, row);
            return valid;
        } else {
            return FALSE;
        }
        return FALSE;
    }
}

int isValidColumns(FILE* maskFile, int rowCount){
    int colCount = 0;
    int i = 0;
    char string[256]  = "";
    int len = 0;

    rewind(maskFile);

    if (maskFile == NULL){
        return colCount;
    } else {
        for (i = 0; i < rowCount; i++){
            if( fgets (string, 256, maskFile) != NULL ){
                strip(string, "\n");
                strip(string, "\r");
                len = getColCount(string, " ");
            }
            if (len != MASK_COL){
                return FALSE;
            }
        }
        return TRUE;
    }
}

int getColCount(char* string, char* separator){
    char* token = NULL;
    int i = 0;
    
    if (string == NULL){
        return 0;
    } else {
        token = strtok(string, separator);
        while( token != NULL ) {
            token = strtok(NULL, separator);
            i++;
        }
        return i;
    }
}

int getRowCount(FILE* maskFile){
    int rowCount    = 0;
    char line[256]  = "";
    
    if(maskFile != NULL) {
        while(0 == feof(maskFile)){
            if( fgets (line, 256, maskFile) != NULL ){
                rowCount++;
            }
        }
        return rowCount;
    }
    return rowCount;
}