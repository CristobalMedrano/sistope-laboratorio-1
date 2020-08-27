//Bibliotecas utilizadas
#include <stdlib.h>
#include <stdio.h>
#include <jpeglib.h>
#include <inttypes.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <unistd.h>

// Se definen constantes para el pipe
#define READ 0
#define WRITE 1

// Definicion de un booleano
#define FALSE 0
#define TRUE 1

// Opciones para mostrar un borde en los resultados de la bandera showResults.
#define BORDER 0

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

int classifyImage(Image image, int classificationThreshold);
void showImageResultTitle(int numberImages, int flagShowResults);
void showImageResultBody(int numberImages, int imageNumber, int flagShowResults, int isNearlyBlack);

//Entradas:
//  Image image: Estructura con la imagen proveniente de la cuarta etapa (binarización).
//  int classificationThreshold: Entero con el umbral de clasificación.
//
//Funcionamiento: Función que representa una primera parte de la quinta etapa del pipeline y realiza la clasificación
//                de una imagen, según un umbral (ambos suministrados), determinando si una imagen es o no "nearly black".
//
//Salida: Entero que representa si una imagen es casi negra/nearly black (TRUE 1) o no (FALSE 0). 
int classifyImage(Image image, int classificationThreshold){
    // Se obtiene largo e inicializa contador
    int len = image.height * image.width;
    int count = 0;
    // Se recorre buffer en busca de pixeles negros y aumenta contador
    for (int i = 0; i < len; i++){
        if(image.image_buffer[i] == 0){
            count++;
        }
    }
    // Se obtiene porcentaje de negrura
    float blacknessPercentage = (count*100)/len;
    // Se clasifica según el siguiente algoritmo
    if (blacknessPercentage >= classificationThreshold){
        // Si el porcentaje es mayor al umbral entregado, se retorna TRUE 1
        return TRUE;
    }
    else{
        // En caso contrario, se retorna FALSE 0
        return FALSE;
    }
}

Image classifiedImage(Image image){
    int totalComponents = 0;

    Image currentBinarizedImage = {};
    currentBinarizedImage.width = image.width;
    currentBinarizedImage.height = image.height;
    currentBinarizedImage.color_channel = image.color_channel;
    currentBinarizedImage.image_buffer = NULL;
    currentBinarizedImage.image_buffer = (JSAMPLE*) malloc(sizeof(int) *
                                currentBinarizedImage.width  *
                                currentBinarizedImage.height *
                                currentBinarizedImage.color_channel);
    // Se calcula el total de componentes de la imagen
    totalComponents = image.height*image.width*image.color_channel;
    int pos = 0;
    int i = 0;
    for (i = 0; i < totalComponents; i++)
    {
        currentBinarizedImage.image_buffer[pos] = image.image_buffer[pos];
        pos++;
    }
    return currentBinarizedImage;
    
}

//Entradas:
//  int numberImages: Entero con la cantidad total de imagenes
//  int imageNumber: Entero con el número asociado a una imagen
//  int flagShowResults: Entero que representa la bandera de resultados
//                       que indica si se deben mostrar los resultados por pantalla. 
//  int isNearlyBlack: Entero que representa si una imagen es nearlyblack o no (1 y 0, respectivamente).
//
//Funcionamiento: Procedimiento que muestra el cuerpo de la tabla de resultados, es decir, la representación
//                visual por pantalla del resultado de la clasificación de cada imagen.
//
//Salida: No tiene por ser un procedimiento.
void showImageResultBody(int numberImages, int imageNumber, int flagShowResults, int isNearlyBlack) {
    // En caso que se desee mostrar por pantalla los resultados o conclusión del programa,
    // se muestra cada clasificación por imagen (cada fila representa el resultado asociado a una imagen)
    if (flagShowResults == TRUE){
        // Se maneja la alineación de las columnas en caso de tener más de 10 imagenes.
        if (imageNumber >= 10){
            if(isNearlyBlack == TRUE){
                // Caso positivo
                fprintf(stderr, "|     image_%d   |         yes         |", imageNumber);
            }
            else{
                // Caso negativo
                fprintf(stderr, "|     image_%d   |         no          |", imageNumber);
            }
        } else {// En caso de tener menos de 10 imagenes
            if(isNearlyBlack == TRUE){
                // Caso positivo
                fprintf(stderr, "|     image_%d    |         yes         |", imageNumber);
            }
            else{
                // Caso negativo
                fprintf(stderr, "|     image_%d    |         no          |", imageNumber);
            }
        } // Se manejan los bordes posterior a mostrar cada clasificación
        if (BORDER == TRUE && numberImages == imageNumber){
            if (numberImages == 10){
                fprintf(stderr, "\n|-----------------|---------------------|");
            } else {
                fprintf(stderr, "\n|----------------|---------------------|");
            }
        }// Saltos de linea respectivos para mejor visualización en consola
        if (numberImages == imageNumber){
            fprintf(stderr, "\n");
        }
        fprintf(stderr, "\n");
    }
}

void printPixels(Image image){
    // Se muestra por pantalla el alto, ancho y los canales que posee la imagen de entrada.
    fprintf(stderr, "width = %" PRIu32 "\n", image.width);
    fprintf(stderr, "height = %" PRIu32 "\n", image.height);
    fprintf(stderr, "channels = %" PRIu32 "\n", image.color_channel);
    // Se inicializan variables
    uint8_t num = 0;
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
    }
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
    int classificationThreshold = atoi(argv[3]);
    int flagShowResults = FALSE;
    
    if(argv[5] != NULL){
        flagShowResults = TRUE;
    }
    
    // Se crea el pipe de classify->write
    // (clasificar a escribir)
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
        
        // Se realiza un EXECVP para reemplazar este proceso con la quinta etapa 
        // (Clasificacion de la imagen) del pipeline.
        char *args[] = {"write.out", argv[1], argv[2], argv[3], argv[4], argv[5], NULL};
        execvp("src/pipeline/write.out", args);

    } else { 
        // Soy el padre
        
        // Conexion entre la escritura del pipe y la salida del padre.
        dup2(pipedes[WRITE], STDOUT_FILENO);
        close(pipedes[READ]);
        close(pipedes[WRITE]);

        int i = 0;
        for (i = 1; i <= numberImages; i++)
        {
            // Se inicializa en 0 la memoria de la imagen binarizada
            Image binarizedImage = {};

            // Se lee la imagen binarizada desde el pipe
            read(STDIN_FILENO, &binarizedImage, sizeof(Image));
            
            binarizedImage.image_buffer = (JSAMPLE*) malloc(sizeof(int) *
                                        binarizedImage.width  *
                                        binarizedImage.height *
                                        binarizedImage.color_channel);

            int imageBufferLengthRead = binarizedImage.height * binarizedImage.width * binarizedImage.color_channel * sizeof(int);            
            read(STDIN_FILENO, binarizedImage.image_buffer, imageBufferLengthRead);
            
            // Por defecto, toda imagen es clasificada como no cercana al negro.
            int isNearlyBlack = FALSE;
            
            // Se analiza la propiedad segun el umbral ingresado por el usuario.
            isNearlyBlack = classifyImage(binarizedImage, classificationThreshold);

            // Si el usuario lo indica, se muestran los resultados por pantalla.
            showImageResultBody(numberImages, i, flagShowResults, isNearlyBlack);

            Image currentBinarizedImage = {};
            currentBinarizedImage.image_buffer = NULL;

            currentBinarizedImage = classifiedImage(binarizedImage);
            int imageBufferLengthWrite = currentBinarizedImage.height * currentBinarizedImage.width * currentBinarizedImage.color_channel * sizeof(int);            
            
            // Se escribe la imagen binarizada en la salida del padre.
            write(STDOUT_FILENO, &currentBinarizedImage, sizeof(Image));
            write(STDOUT_FILENO, currentBinarizedImage.image_buffer, imageBufferLengthWrite);
        }
        wait(NULL);
    }
    return 0;
}

