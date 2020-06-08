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

//Entradas: argc (int): Numero de argumentos ingresados al momento de ejecutar el programa.
//          argv (char*): Arreglo con los argumentos del programa.
//          numberImages (int*): Puntero con la direccion de memoria donde se encuentra la cantidad de imagenes.
//          binarizationThreshold (int*): Puntero con la direccion de memoria donde se encuentra el umbral de binarizacion.
//          classificationThreshold (int*): Puntero con la direccion de memoria donde se encuentra el umbral de clasificacion.
//          maskFilename (char**): Puntero con la direccion de memoria donde se encuentra el nombre del archivo de mascara.
//          flagShowResults (int*): Puntero con la direccion de memoria donde se encuentra la bandera de resultados.
//
//Funcionamiento: Procedimiento que obtiene los datos ingresados por el usuario, de no existir alguna opcion
//                obligatoria, el programa termina su ejecucion indicando el error por pantalla.
//
//Salidas: Vacia, se retornan los datos por referencia.
void getOptionsArguments(int argc, char *argv[], int* numberImages, int* binarizationThreshold, int* classificationThreshold, char** maskFilename, int* flagShowResults) {
    // Se inicializan los datos
    int option = 0;
    char* endptr = NULL;
    int argument = -1;
    int isEqual = -1;

    // Si la cantidad de argumentos del programa no supera los argumentos minimos requeridos, el programa muestra
    // el error y termina la ejecucion de este.
    if (argc < MIN_OPTS) {
        showCorrectUse(argv);
        exit(EXIT_FAILURE);
    } else {
        // Se leen las opciones ingresadas por el usuario
        while ((option = getopt(argc, argv, "c:u:n:m:b")) != -1) {
            switch (option) {
            case 'c':
                argument = strtol(optarg, &endptr, ENDPTR_LEN);
                // Se verifica si la opcion tiene acompaniado un argumento, de no ser asi, se termina
                // la ejecucion del programa indicando previamente el error.
                if (optarg != 0 && argument == 0) {
                    showCorrectUse(argv);
                    exit(EXIT_FAILURE);
                } else {
                    // Se verifica que sea un entero el valor ingresado, sino, el programa termina su ejecucion.
                    if (!isInteger(optarg, "la cantidad de imagenes")) exit(EXIT_FAILURE);
                    // Se retorna por referencia el valor de imagenes a leer.
                    *numberImages = atoi(optarg);
                }
                break;
            case 'u':
                argument = strtol(optarg, &endptr, ENDPTR_LEN);
                // Se verifica si la opcion tiene acompaniado un argumento, de no ser asi, se termina
                // la ejecucion del programa indicando previamente el error.
                if (optarg != 0 && argument == 0) {
                    showCorrectUse(argv);
                    exit(EXIT_FAILURE);
                } else {
                    // Se verifica que sea un entero el valor ingresado, sino, el programa termina su ejecucion.
                    if (!isInteger(optarg, "el umbral de binarizacion")) exit(EXIT_FAILURE);
                    // Se retorna por referencia el valor para el umbral de binarizacion.
                    *binarizationThreshold = atoi(optarg);
                }
                break;
            case 'n':
                argument = strtol(optarg, &endptr, ENDPTR_LEN);
                // Se verifica si la opcion tiene acompaniado un argumento, de no ser asi, se termina
                // la ejecucion del programa indicando previamente el error.
                if (optarg != 0 && argument == 0) {
                    showCorrectUse(argv);
                    exit(EXIT_FAILURE);
                } else {
                    // Se verifica que sea un entero el valor ingresado, sino, el programa termina su ejecucion.
                    if (!isInteger(optarg, "el umbral de clasificacion")) exit(EXIT_FAILURE);
                    // Se retorna por referencia el valor para el umbral de clasificacion.
                    *classificationThreshold = atoi(optarg);
                }
                break;
            case 'm':
                // Se verifica que el nombre ingresado sea mascara.txt
                isEqual = strcmp("mascara.txt", optarg);
                // Si el nombre no corresponde o no se ingresa un argumento, el programa indica el error y 
                // termina la ejecucion de este.
                if (optarg != 0 && isEqual != 0) {
                    showCorrectUse(argv);
                    exit(EXIT_FAILURE);
                } else {
                    // Se solicita memoria para el nombre del archivo.
                    *maskFilename = (char * ) malloc(sizeof(char) * MASKFILENAME_LEN);
                    // De ser asignada correctamente la memoria se retorna por referencia la direccion de memoria
                    // donde se encuentra este nombre.
                    if (*maskFilename != NULL) *maskFilename = strcpy(*maskFilename, optarg);
                }
                break;
            case 'b':
                // Al ser un argumento opcional, se retorna directamente el valor, en caso de encontrarse en las opciones.
                *flagShowResults = TRUE;
                break;
            default:
                // Si algun argumento no es correcto, se muestra el error por pantalla y el programa finaliza su ejecucion
                showCorrectUse(argv);
                exit(EXIT_FAILURE);
                break;
            }
        } 
    }
}

//Entradas: argv (char*): Arreglo con los argumentos del programa.   
//
//Funcionamiento: Muestra por pantalla el error de argumentos.
//
//Salidas: No posee.
void showCorrectUse(char *argv[]){
    // Se muestra el error por pantalla.
    printf("ERROR. Se ingreso un numero incorrecto de opciones/argumentos\n");
    fprintf(stderr, "Uso: %s [-c cantidad de imagenes] [-u umbral binarizacion] [-n umbral clasificacion] [-m mascara] [-b]\n", argv[0]);
}

//Entradas: optarg (char*): String que contiene la opcion ingresada por el usuario.
//          type (char*): String que contiene un mensaje tipo caracteristico de cada funcion.   
//
//Funcionamiento: Verifica si el argumento ingresado (optarg) es un entero.
//
//Salidas: Retorna TRUE (1) si el argumento es un entero y FALSE (0), si no lo es.
int isInteger(char* optarg, char* type)
{
    // Se inicializan las variables
    int i = 0;
    char dot = '.';
    char comma = ',';
    char dash = '-';
    // Si el argumento es valido
    if (optarg != NULL){
        // Para cada letra en el string argumento, se verifica
        for (i = 0; i < strlen(optarg); i++)
        {
            // Si contiene un punto, coma o guion, ademas de verificar si es una letra
            if (optarg[i] == dot || 
                optarg[i] == comma ||
                optarg[i] == dash ||
                isalpha(optarg[i])){
                printf("Error. El valor para %s debe ser un entero positivo.\n", type);
                // Si contiene algun caracter, retorna falso.
                return FALSE;
            }
        }
        // En caso contrario, optarg es un entero y retorna verdadero.
        return TRUE;
    } else {
        // Si no existe un argumento, retorna inmediatamente falso
        return FALSE;
    }
}

//Entradas: numberImages (int): Entero con el numero de imagenes.
//          binarizationThreshold (int): Entero con el unmbral de binarizacion.
//          classificationThreshold (int): Entero con el umbral de clasificacion.
//          maskFilename (char*): Nombre del archivo de mascara.
//
//Funcionamiento: Verifica el estado de validacion de cada argumento.
//
//Salidas: Retorna TRUE (1) si todos los argumentos son validos o retorna FALSE (0) si algun argumento es incorrecto.
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

//Entradas: numberImages (int): Entero con el numero de imagenes.
//
//Funcionamiento: Valida si el numero de imagenes es correcto (Entero positivo mayor a 0) además de 
//                verificar si el archivo de imagen existe.
//
//Salidas: Retorna verdadero (TRUE 1) si el numero de imagenes es correto y falso (FALSE 0) si el numero es
//         incorrecto o no valido.
int validateNumberImages(int numberImages){
    // Se inicializan las variables
    char filename[40] = "";
    int i = 0;
    int fileStatus = 0;

    // Si el numero de imagenes es negativo o igual a 0.
    if (numberImages <= 0){
        // Se muestra el error por pantalla y se retorna falso.
        printf("ERROR. Cantidad de imagenes incorrecta, debe ingresar 1 o mas imagenes.\n");
        return FALSE;
    // Si no
	} else {
        // Se verifica que cada imagen exista
        for (i = 1; i <= numberImages; i++)
        {
            // Se verifica que la asignacion de nombre sea correcta
            if (sprintf(filename, "imagen_%i.jpg", i) >= 0){
                // Se valida si el archivo existe.
                fileStatus = isValidFile(filename);
                // Si no existe
                if (fileStatus == FALSE){
                    // Se retorna falso
                    return FALSE;
                }
            }
            else{
                // Si la asignacion de nombre falla, se retorna falso.
                return FALSE;
            }
        }
        // En caso de que todas las imagenes existan, se retorna verdadero.
        return TRUE;
    }
}

//Entradas: filename (char*): String que contiene el nombre del archivo.
//
//Funcionamiento: Verifica si un archivo existe en el directorio de programa.
//
//Salidas: Retorna falso si no existe y verdadero si existe. (FALSE 0, TRUE 1)
int isValidFile(char* filename){
    // Se inicializan las variables
    FILE* file = NULL;
    // Si el nombre del archivo no es valido.
    if(filename == NULL){
        // se indica por pantalla el error
        printf("El nombre del archivo no es válido.");
        // se retorna falso
        return FALSE;
    } else {
        // Se abre el archivo
        file = fopen(filename, "rb");
        // si fue posible abrir el archivo, entonces
        if (file != NULL){
            // se cierra el archivo
            fclose(file);
            // se retorna verdadero (esto indica de que si existe el archivo).
            return TRUE;
        } else {
            // si no fue posible abrir el archivo, se indica al usuario el error por pantalla.
            printf("El archivo de imagen '%s', no existe o se encuentra en otro directorio.\n", filename);
            // se retorna falso.
            return FALSE;
        }
    }
}

//Entradas: binarizationThreshold (int): Entero con el umbral de binarizacion.
//
//Funcionamiento: Valida si el umbral de binarizacion es correcto. (Entero positivo y menor a 255)
//
//Salidas: Retorna verdadero (TRUE 1) si el umbral es correto y falso (FALSE 0) si el umbral es incorrecto.
int validateBinarizationThreshold(int binarizationThreshold){
    // Si el umbral es negativo
    if (binarizationThreshold < 0){
        // Se indica que existe un error.
        printf("ERROR. Umbral de binarizacion invalido (negativo).\n");
        // Se retorna falso.
        return FALSE;
    }
    // Si no, se comprueba si es mayor a 255
    else if (binarizationThreshold > 255){
        // de ser asi, se muestra el error por pantalla.
        printf("ERROR. Umbral de binarizacion invalido (sobre 255).\n");
        // Se retorna falso
        return FALSE;
    }
    // Si no se cumplen las condiciones anteriores.
    else
    {   
        // el umbral es valido y se retorna verdadero.
        return TRUE;
    }    
}

//Entradas: classificationThreshold (int): Entero con el umbral de clasificacion.
//
//Funcionamiento: Valida si el umbral de clasificacion es correcto. (Entero positivo y menor a 100)
//
//Salidas: Retorna verdadero (TRUE 1) si el umbral es correto y falso (FALSE 0) si el umbral es incorrecto.
int validateClassificationThreshold(int classificationThreshold){
    // Se si a caso el umbral es negativo
    if (classificationThreshold < 0){
        // de ser asi se muestra el error por pantalla.
        printf("ERROR. Umbral de clasificacion invalido (negativo).\n");
        // se retorna falso.
        return FALSE;
    }
    // Sino, se verifica si el umbral es mayor a 100
    else if (classificationThreshold > 100){
        // se muestra el error por pantalla
        printf("ERROR. Umbral de clasificacion invalido (sobre 100).\n");
        // se retorna falso.
        return FALSE;
    }
    // sino, el umbral es correcto
    else{
        // se retorna verdadero
        return TRUE;
    }
}

//Entradas: maskFilename (char*): String que contiene el nombre del archivo de mascara.
//
//Funcionamiento: Valida si el archivo de mascara es correcto (matriz de 3x3 en su interior).
//
//Salidas: Retorna verdadero si la mascara contenida es valida, falso si no lo es (TRUE 1, FALSE 0).
int validateMaskFile(char* maskFilename){
    // Se inicializan las variables
    FILE* maskFile = NULL;
    int isValid = FALSE;

    // Se abre el archivo de mascara.
    maskFile = fopen(maskFilename, "rb");
    // Si no fue posible abrir el archivo
    if (maskFile == NULL) {
        // Se muestra el error por pantalla
        printf("ERROR. Archivo de mascara no existe.\n");
        // Se retorna falso
        return FALSE;
    // Si fue posible abrir el archivo
    } else {
        // Se verifica que sea una matriz valida.
        isValid = isValidMatrixMask(maskFile);
        // Si la matriz no es valida
        if (isValid == FALSE){
            // Se indica por pantalla el error
            printf("ERROR. El archivo de mascara no es valido.\n");
            printf("Verifique que la matriz sea de 3x3.\n");
            return FALSE;
        }
        // se cierra el archivo de mascara
        fclose(maskFile);
        // se retorna verdadero.
        return TRUE;
    }
}

//Entradas: maskFile (FILE*): Puntero de tipo FILE, que contiene los datos del archivo.
//
//Funcionamiento: Valida si una matriz de mascara es valida (matriz de 3x3).
//
//Salidas: Retorna verdadero si la mascara es correcta (3x3) y falso si no lo es.
int isValidMatrixMask(FILE* maskFile){
    // Se inicializan las variables
    int row = 0;
    int valid = FALSE;
    // Si el archivo de mascara es nulo
    if (maskFile == NULL){
        // Se retorna falso
        return FALSE;
    // Sino
    } else {
        // Se obtiene la cantidad de filas del archivo.
        row = getRowCount(maskFile);
        // Se verifica si la cantidad de filas es correcta (MASK_ROW 3)
        if (row == MASK_ROW){
            // Se verifica si la cantidad de columnas por fila es correcta.
            valid = isValidColumns(maskFile, row);
            // Si la cantidad de columnas es valida.
            if (valid == TRUE){
                // se retorna true.
                return TRUE;
            }
            // Si no, se retorna falso
            return TRUE;
        // Sino
        } else {
            // Se retorna falso
            return FALSE;
        }
    }
}

//Entradas: maskFile (FILE*): Puntero de tipo FILE, que contiene los datos del archivo.
//          rowCount (int): Numero de columnas en el archivo
//
//Funcionamiento: Verfica la cantidad de columnas de la matriz que se encuentra dentro del archivo es correcta.
//
//Salidas: Retorna verdadero si las columnas obtenidas son correctas, falso si no lo son. (TRUE 1, FALSE 0)
int isValidColumns(FILE* maskFile, int rowCount){
    // Se inicializan las variables
    int colCount = 0;
    int i = 0;
    char string[256]  = "";

    // Se reinicia el puntero del archivo (para no abrir el archivo dos veces).
    rewind(maskFile);

    // Si el archivo es nulo
    if (maskFile == NULL){
        // Se retorna falso
        return FALSE;
    // sino
    } else {
        // Para cada fila
        for (i = 0; i < rowCount; i++){
            // Se obtiene el valor string de la fila
            if( fgets (string, 256, maskFile) != NULL ){
                // Se le quita el salto de linea
                strip(string, "\n");
                // Se le quita el salto de linea unix
                strip(string, "\r");
                // Se obtiene la cantidad de columnas
                colCount = getColCount(string, " ");
            }
            // Si la cantidad de columnas de la fila es distinta a MASK_COL (3)
            if (colCount != MASK_COL){
                // se retorna falso
                return FALSE;
            }
        }
        // Si todas las columnas son del mismo largo, se retorna verdadero (TRUE 1)
        return TRUE;
    }
}

//Entradas: string (char*): string que contiene la linea a leer.
//          separator (char*): string que indica donde se debe separar la fila.
//
//Funcionamiento: A partir de una fila, se segmenta la fila en trozos dependiendo de el separador. 
//                esta separacion permite contar la cantidad de columnas que posee el archivo.
//
//Salidas: Retorna el numero de columnas o segmentos de un string.
int getColCount(char* string, char* separator){
    // Se inicializan las variables
    char* token = NULL;
    int i = 0;
    
    // Si el string es nulo
    if (string == NULL){
        // Se retorna 0
        return 0;
    // Sino
    } else {
        // Se realiza la segmentacion del string 
        token = strtok(string, separator);
        // Por cada segmento se cuenta la cantidad de columnas
        while( token != NULL ) {
            token = strtok(NULL, separator);
            i++;
        }
        // Se retorna el total de segmentos obtenidos.
        return i;
    }
}

//Entradas: maskFile (FILE*): Puntero de tipo FILE, que contiene los datos del archivo.
//
//Funcionamiento: Obtiene la cantidad de filas que posee un archivo.
//
//Salidas: Retorna la cantidad de filas en el archivo.
int getRowCount(FILE* maskFile){
    // Se inicializan las variables
    int rowCount    = 0;
    char line[256]  = "";
    
    // Si el archivo de mascara es distinto de nulo
    if(maskFile != NULL) {
        // Se lee hasta el final del archivo
        while(0 == feof(maskFile)){
            // Por cada fila encontrada
            if( fgets (line, 256, maskFile) != NULL ){
                // Se le suma 1 a el contador de filas
                rowCount++;
            }
        }
        // Se retorna el contador de filas
        return rowCount;
    // si el archivo es nulo.
    } else {
        // Se retorna 0
        return 0;
    }
}