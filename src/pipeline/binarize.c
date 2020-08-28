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

//Entradas:
//  Image image: Estructura con la imagen proveniente de la tercera etapa (filtro laplaciano).
//  int binarizationThreshold: Entero con el umbral de binarización
//
//Funcionamiento: Función que realiza la cuarta etapa del pipeline, es decir, binariza una imagen según un algoritmo definido
//                y un umbral de binarización entregado.
//
//Salida: Estructura con imagen binarizada.
Image binarizeImage(Image image, int binarizationThreshold){
    // Se guardan las dimensiones de la imagen de entrada.
    Image temporalImage = {};
    temporalImage.height = image.height;
    temporalImage.width = image.width;
    temporalImage.color_channel = image.color_channel;
    // Se crea el buffer necesario.
    temporalImage.image_buffer = (JSAMPLE*) malloc(sizeof(int) *
                                    temporalImage.width  *
                                    temporalImage.height *
                                    temporalImage.color_channel);
    int len = temporalImage.height * temporalImage.width;
    // Se recorre buffer de la imagen
    for (int i = 0; i < len; i++){
        // En caso que un valor sea mayor al umbral entregado, se vuelve 255.
        if (image.image_buffer[i] > binarizationThreshold){
            temporalImage.image_buffer[i] = 255;
        }
        else{
            // En caso contrario, se vuelve 0.
            temporalImage.image_buffer[i] = 0;
        }
    }
    // Se retorna estructura de imagen binarizada resultante.
    return temporalImage;
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
    int binarizationThreshold = atoi(argv[2]);

    // Se crea el pipe de binarize->classify 
    // (binarizar a clasificar)
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

        // Se realiza un EXECVP para reemplazar este proceso con la cuarta etapa 
        // (Binarizacion de la imagen) del pipeline.
        char *args[] = {"classify.out", argv[1], argv[2], argv[3], argv[4], argv[5], NULL};
        execvp("src/pipeline/classify.out", args);

    } else { 
        // Soy el padre

        // Conexion entre la escritura del pipe y la salida del padre.
        dup2(pipedes[WRITE], STDOUT_FILENO);
        close(pipedes[READ]);
        close(pipedes[WRITE]);

        int i = 0;
        for (i = 1; i <= numberImages; i++)
        {
            // Se inicializa en 0 la memoria de la imagen con el filtro laplaciano
            Image laplacianFilterImage = {};

            // Se lee la imagen con el filtro laplaciano desde el pipe
            read(STDIN_FILENO, &laplacianFilterImage, sizeof(Image));
            
            laplacianFilterImage.image_buffer = (JSAMPLE*) malloc(sizeof(int) *
                                        laplacianFilterImage.width  *
                                        laplacianFilterImage.height *
                                        laplacianFilterImage.color_channel);

            int imageBufferLengthRead = laplacianFilterImage.height * laplacianFilterImage.width * laplacianFilterImage.color_channel * sizeof(int);            
            read(STDIN_FILENO, laplacianFilterImage.image_buffer, imageBufferLengthRead);

            // Se inicializa en 0 la memoria de la imagen a binarizar
            Image binarizedImage = {};
            binarizedImage.image_buffer = NULL;

            // Se binariza la imagen segun el umbral ingresado por el usuario.       
            binarizedImage = binarizeImage(laplacianFilterImage, binarizationThreshold);
            int imageBufferLengthWrite = binarizedImage.height * binarizedImage.width * binarizedImage.color_channel * sizeof(int);            

            // Se libera la memoria usada en la imagen con el filtro laplaciano.

            // Se escribe la imagen binarizada en la salida del padre.
            write(STDOUT_FILENO, &binarizedImage, sizeof(Image));
            write(STDOUT_FILENO, binarizedImage.image_buffer, imageBufferLengthWrite);
        }
        wait(NULL);
    }
    return 0;
}