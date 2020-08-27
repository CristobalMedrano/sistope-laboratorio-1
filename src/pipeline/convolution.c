//Bibliotecas utilizadas
#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <jpeglib.h>
#include <inttypes.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <unistd.h>

// Se definen constantes para el pipe
#define READ 0
#define WRITE 1

// Constantes para definir la cantidad de filas y columnas de la mascara
#define MASK_ROW 3
#define MASK_COL 3

// Esta estructura contiene la informacion escencial de la imagen.
// Esta definida tal cual como lo indica la libreria jpeglib.
typedef struct Image
{
    // image_buffer es un puntero que contiene un arreglo con la informacion de la imagen, cada dato representa un pixel de valor 0 a 255.
    // En RGB se usan 3 valores consecutivos para definir un pixel (Es decir buffer[i], buffer[i+1], buffer[i+2] = 1 pixel).
    // En Escala de grises se usa solo un valor para definir un pixel. (Es decir buffer[i] = 1 pixel)
    JSAMPLE *image_buffer; /*Points to large array of R,G,B-order data*/
    // height es un entero sin signo de 32 bits que contiene la cantidad de filas de la imagen (alto de la imagen) 
    uint32_t height; /*Rows*/
    // width es un entero sin signo de 32 bits que contiene la cantidad de columnas de la imagen (ancho de la imagen) 
    uint32_t width; /*Columns*/
    // color_channel es un entero sin signo de 32 bits que contiene el numero de canales de color (para RGB son 3 canales y para escala de grises 1). 
    uint32_t color_channel; /*"component" values*/
} Image;

Image applyLaplacianFilter(Image image, char* maskFilename);
int** getImageMatrix(int height, int width, JSAMPLE* buffer);
int** getLaplacianFilter(char* maskFilename);
int** getLaplacianFilterMask(FILE* maskFile);
void freeMatrix(int** matrix, int row);
void showMatrix(int n, int m, int** matrix);
int** appendMatrix(int** matrix, int posX, int posY, int value);
int** createMatrix(int n, int m);
void strip(char* string, char* character);

//Entradas: image (Image): Estructura con la imagen proveniente de la segunda etapa (escala de grises).
//          maskFilename (char*): String que contiene el nombre de la mascara de filtro laplaciano.
//
//Funcionamiento: Se aplica el filtro laplaciano obtenido desde el archivo de mascara a la imagen en escala
//                de grises, el resultado es una imagen (image) con la convolucion aplicada.
//
//Salidas: Retorna una estructura con la imagen luego de aplicar el filtro laplaciano.
Image applyLaplacianFilter(Image image, char* maskFilename){
    // Se inicializan las variables a utilizar
    Image convertedImage = {};
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

//Entradas:
//  int** matrix: Puntero doble con la informacion de la matriz
//  int row: Cantidad de filas de la matriz ingresada
//
//Funcionamiento: Procedimiento que libera la memoria asignada a la matriz ingresada.
//                Primero libera el arreglo de cada fila (columnas) y luego el arreglo de filas (filas).
//
//Salida: No tiene por ser procedimiento.
void freeMatrix(int** matrix, int row)
{
    // Si la matriz existe
    if (NULL != matrix) 
    {   
        int i = 0;
        // Para cada fila en la matriz
        for(i = 0; i < row; i++)
        {
            // Se liberan las columnas asignadas.
            free(matrix[i]);
        }
        // Finalmente se libera el puntero inicial de la matriz.
        free(matrix);
    }
}

//Entradas:
//  int n: Cantidad de filas
//  int m: Cantidad de columnas
//  int** matrix: Matriz de NxM
//
//Funcionamiento: Procedimiento que muetra por pantalla una matriz de nxm
//
//Salida: No tiene por ser procedimiento.
void showMatrix(int n, int m, int** matrix){
    // Si la matriz existe y la cantidad de filas/columnas es positiva.
    if(NULL != matrix && n >= 0 && m >= 0)
    {
        int i = 0;
        // Se recorre cada fila
        for(i = 0; i < n; i++)
        {
            int j = 0;
            // Se recorre cada columna
            for(j = 0; j < m; j++)
            {
                // Se muestra por pantalla dependiendo del largo del valor la matriz.
                if (matrix[i][j] >= 100) 
                {
                    printf("%d  ", matrix[i][j]);
                }
                else if (matrix[i][j] >= 10) 
                {
                    printf("%d   ", matrix[i][j]);
                }
                else if (matrix[i][j] <= 9 && matrix[i][j] >= 0)
                {
                    printf("%d    ", matrix[i][j]);
                } 
                else if (matrix[i][j] < 0 && matrix[i][j] >= -9)
                {
                    printf("%d   ", matrix[i][j]);
                }
                else if (matrix[i][j] >= -10)
                {
                    printf("%d  ", matrix[i][j]);
                }
                else if (matrix[i][j] >= -100)
                {
                    printf("%d ", matrix[i][j]);
                } else {
                    printf("%d", matrix[i][j]);
                }
            }
            printf("\n");
        }
    }   
}

//Entradas:
//  int** matrix: Puntero doble que contiene una matriz de enteros
//  int posX: Posicion en X donde se insertara el dato
//  int posY: Posicion en Y donde se insertara el dato
//  int value: Valor a insertar en la matriz.
//
//Funcionamiento: Función que inserta un valor en la matriz
//
//Salida: Retorna la matriz actualizada con el valor agregado.
//        Retorna Nulo si la matriz no existe o la posicion a insertar es negativa.
int** appendMatrix(int** matrix, int posX, int posY, int value){
    // Si la posicion en X e Y son negativas o la matriz no existe
    if (matrix == NULL || posX < 0 || posY < 0){
        // Se retorna nulo.
        return NULL;
    // Sino
    } else {
        // Se inserta el valor en la matriz.
        matrix[posX][posY] = value;
        // Se retorna la matriz actualizada.
        return matrix;
    }
}

//Entradas:
//  int n: Cantidad de filas
//  int m: Cantidad de columnas
//
//Funcionamiento: Función que crea una matriz de nxm inicializada en 0.
//
//Salida: Una matriz de nxm inicializada en 0, Nulo si no fue posible asignar la memoria de la matriz.
int** createMatrix(int n, int m){
    // Se inicializan las variables
    int i = 0;
    int** matrix = NULL;
    
    // Se solicita memoria
    matrix = (int **)calloc(n, sizeof(int *));
                    for (i = 0; i < n; i++)
                    matrix[i] = (int *)calloc(m, sizeof(int));
    // Si la asignacion de memoria es correcta
    if(NULL != matrix)
    {
        // Se retorna la matriz inicializada en 0.
        return matrix;
    }
    // Sino, se muestra por pantalla el error.
    printf("Memoria insuficiente: createMatrix()\n");
    printf("Error: functions.c\n");
    return NULL;
}

//Entradas:
//  char* string: String
//  char* character: Caracter a eliminar en el string ingresado.
//
//Funcionamiento: Procedimiento que elimina un caracter de un string (Similar a strip de python).
//
//Salida: no tiene por ser procedimiento.
void strip(char* string, char* character){
    // Se inicializan las variables
    char* token = NULL;
    // Si el string es distinto de nulo
    if (string != NULL){
        // Se divide el string en el caracter ingresado.
        // Reemplaza el caracter del string por un espacio vacio.
        token = strtok(string, character);
        // Recorre todo el string para eliminar ese caracter y sus repeticiones.
        while( token != NULL ) {
            token = strtok(NULL, character);
        }
    }
}

//Entrada:
//  Image image: Estructura con imagen almacenada.
//
//Funcionamiento: Procedimiento que permite visualizar por pantalla información asociada
//                a una imagen, como sus dimensiones, canales y pixeles.
//
//Salida: No tiene por ser procedimiento.
void printPixels(Image image){
    // Se muestra por pantalla el alto, ancho y los canales que posee la imagen de entrada.
    fprintf(stderr, "width = %" PRIu32 "\n", image.width);
    fprintf(stderr, "height = %" PRIu32 "\n", image.height);
    fprintf(stderr, "channels = %" PRIu32 "\n", image.color_channel);
    // Se inicializan variables
    /*uint8_t num = 0;
    int loc = 0;
    // Se recorre la imagen e imprime por pantalla el valor de sus pixeles.
    for (int i = 0; i < image.height; i++)
    {
        for (int j = 0; j < image.width*image.color_channel; j++)
        {
            num = image.image_buffer[loc];
            fprintf(stderr, "%" PRId8 " ", num);
            loc++;
        }
        fprintf(stderr, "\n");
    }*/
}

int main(int argc, char *argv[])
{
    /*
        argv[1] -> Cantidad de imagenes
        argv[2] -> Umbral de binarizacion
        argv[3] -> Umbral de clasificacion
        argv[4] -> Nombre de la mascara
        argv[5] -> Mostrar resultados
    */
   
    int numberImages = atoi(argv[1]);
    char* maskFilename = argv[4];

    // Se crea el pipe de convolution->binarize 
    // (aplicacion del filtro laplaciano - convolucion a binarizar)
    int pipedes[2];
    pipe(pipedes);

    // Se crea el hijo
    pid_t pid = fork();
    if (pid < 0){ 
        // No se creo el hijo
        perror("Existe un error en el Fork de 'grayscale.c'\n");
        exit(1);
        
    } else if (pid == 0){ 
        // Soy el hijo

        // Conexion entre la lectura del pipe y la entrada del hijo.
        dup2(pipedes[READ], STDIN_FILENO);
        close(pipedes[READ]);
        close(pipedes[WRITE]);
        
        // Se realiza un EXECVP para reemplazar este proceso con la tercera etapa 
        // (Aplicación de un filtro laplaciano - Convolucion) del pipeline.
        char *args[] = {"binarize.out", argv[1], argv[2], argv[3], argv[4], argv[5], NULL};
        execvp("src/pipeline/binarize.out", args);

    } else { 
        // Soy el padre       
        // Conexion entre la escritura del pipe y la salida del padre.
        dup2(pipedes[WRITE], STDOUT_FILENO);
        close(pipedes[READ]);
        close(pipedes[WRITE]);

        int i = 0;
        for (i = 1; i <= numberImages; i++)
        {
            // Se inicializa en 0 la memoria de la imagen en escala de grises
            Image grayScaleImage = {};

            // Se lee la imagen en escala de grises desde el pipe
            read(STDIN_FILENO, &grayScaleImage, sizeof(Image));

            grayScaleImage.image_buffer = (JSAMPLE*) malloc(sizeof(int) *
                                        grayScaleImage.width  *
                                        grayScaleImage.height *
                                        grayScaleImage.color_channel);
            
            int imageBufferLengthRead = grayScaleImage.height * grayScaleImage.width * grayScaleImage.color_channel * sizeof(int);            
            read(STDIN_FILENO, grayScaleImage.image_buffer, imageBufferLengthRead);
            

            // Se inicializa en 0 la memoria de la imagen al aplicar el filtro laplaciano
            Image laplacianFilterImage = {};
            laplacianFilterImage.image_buffer = NULL;
            
            // Se aplica el filtro laplaciano a la imagen en escala de grises.            
            laplacianFilterImage = applyLaplacianFilter(grayScaleImage, maskFilename);
            int imageBufferLengthWrite = laplacianFilterImage.height * laplacianFilterImage.width * laplacianFilterImage.color_channel * sizeof(int);            

            // Se libera la memoria usada en la imagen en escala de grises.
            // Se escribe la imagen con el filtro laplaciano en la salida del padre.
            write(STDOUT_FILENO, &laplacianFilterImage, sizeof(Image));
            write(STDOUT_FILENO, laplacianFilterImage.image_buffer, imageBufferLengthWrite);
        }
        wait(NULL);
    }
    return 0;
}