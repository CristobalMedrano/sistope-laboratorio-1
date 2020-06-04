#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <constants.h>
#include <functions.h>

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
    if (numberImages <= 0 ||numberImages > 6){
        printf("ERROR. Cantidad de imagenes incorrecta\n");
        return FALSE;
	}
    return TRUE;
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
        isValid = IsValidMatrixMask(maskFile);
        invalidMaskFile(isValid);
        fclose(maskFile);
        return isValid;
    }
}

void invalidMaskFile(int isValid){
    if (isValid == FALSE){
        printf("ERROR. El archivo de mascara no es valido.\n");
        printf("Verifique que la matriz sea de 3x3.\n");
    }
}

int IsValidMatrixMask(FILE* maskFile){
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

int* splitToNumber(char* string, char* separator){
    int* array = NULL;
    char* token = NULL;
    int i = 0;
    
    if (string == NULL){
        return NULL;
    } else {
        array = newArray(1);
        if (array != NULL) {
            token = strtok(string, separator);
            while( token != NULL ) {
                array = arrayAppend(array, i, atoi(token));
                token = strtok(NULL, separator);
                i++;
            }
            return array;
        }
        return array;
    }
}

int* newArray (int initalLen){
    int* array = NULL;
    if (initalLen <= 0){
        return NULL;
    } else {
        array = (int*)malloc(sizeof(int)*initalLen);
        return array;
    }
}

int* arrayAppend(int* array, int pos, int number){
    if (array == NULL){
        return NULL;
    } else {
        array = realloc(array, sizeof(array)*2);
        if (array != NULL){
            array[pos] = number;
            return array;
        } else {
            return array;
        }
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