#include <functions.h>
#include <stdio.h>
int validateArgs(int numberImages, int binarizationThreshold, int classificationThreshold, char* maskFilename){
    int val1 = validateNumberImages(numberImages);
    int val2 = validateBinarizationThreshold(binarizationThreshold);
    int val3 = validateClassificationThreshold(classificationThreshold);
    int val4 = validateMaskFile(maskFilename);
    if (val1 == TRUE && val2 == TRUE && val3 == TRUE && val4 == TRUE)
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
    else if (binarizationThreshold > 100){
        printf("ERROR. Umbral de binarizacion invalido (sobre 100).\n");
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
    else
    {
        return TRUE;
    }
}

int validateMaskFile(char* maskFilename){
    FILE* maskFile = fopen(maskFilename, "rb");
    if (maskFile == NULL){
        printf("ERROR. Archivo de mascara no existe.\n");
        return FALSE;
    } else
    {
        fclose(maskFile);
        return TRUE;
    }
}
