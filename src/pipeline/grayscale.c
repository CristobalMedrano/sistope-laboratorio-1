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

//Entrada:
//  Image image: Estructura con la imagen proveniente de la primera etapa (lectura).
//
//Funcionamiento: Función que realiza la segunda etapa del pipeline, es decir, convierte una imagen a escala de grises
//                según la ecuación de luminiscencia Y = R*0.3 + G*0.59 + B*0.11
//
//Salida: Estructura imagen que representa o contiene la imagen convertida.
Image convertGrayScale(Image image){

    // Se inicializan los datos a utilizar
    int totalComponents = 0;
    int R = 0;    
    int G = 0;    
    int B = 0;
    int Y = 0;

    // Se calcula el total de componentes de la imagen
    totalComponents = image.height*image.width*image.color_channel;
    
    // Se inicializa la estructura para la imagen convertida
    Image convertedImage;
    convertedImage.height = image.height;
    convertedImage.width = image.width;
    convertedImage.color_channel = 1;
    convertedImage.image_buffer = NULL;
    convertedImage.image_buffer = (JSAMPLE*) malloc(sizeof(int) *
                                       convertedImage.width  *
                                       convertedImage.height *
                                       convertedImage.color_channel);
    // Se recorre la totalidad de los componentes de la imagen
    int pos = 0;
    for (int i = 0; i < totalComponents; i+=3){
        R = image.image_buffer[i]; //se obtiene valor R
        G = image.image_buffer[i+1]; // se obtiene valor G
        B = image.image_buffer[i+2]; // se obtiene valor B
        Y = R*0.3 + G*0.59 + B*0.11; // ecuación de luminiscencia
        convertedImage.image_buffer[pos] = Y;// se guarda el valor obtenido en ecuación
        pos++;
    }
    // Se retorna la estructura de la imagen convertida a escala de grises.
    return convertedImage;
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

    // Se crea el pipe de grayscale->convolution 
    // (escala de grises a la aplicacion del filtro laplaciano - convolucion)
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
        char *args[] = {"convolution.out", argv[1], argv[2], argv[3], argv[4], argv[5], NULL};
        execvp("src/pipeline/convolution.out", args);

    } else { 
        // Soy el padre

        // Conexion entre la escritura del pipe y la salida del padre.
        dup2(pipedes[WRITE], STDOUT_FILENO);
        close(pipedes[READ]);
        close(pipedes[WRITE]);

        int i = 0;
        for (i = 1; i <= numberImages; i++)
        {
            // Se inicializa en 0 la memoria de la imagen normal
            Image normalImage = {};
            
            // Se lee la imagen desde el pipe
            read(STDIN_FILENO, &normalImage, sizeof(Image));
            
            normalImage.image_buffer = (JSAMPLE*) malloc(sizeof(int) *
                                        normalImage.width  *
                                        normalImage.height *
                                        normalImage.color_channel);

            int imageBufferLengthRead = normalImage.height * normalImage.width * normalImage.color_channel * sizeof(int);
            read(STDIN_FILENO, normalImage.image_buffer, imageBufferLengthRead);

            // Se inicializa en 0 la memoria de la imagen en escala de grises
            Image grayScaleImage = {};
            grayScaleImage.image_buffer = NULL;
            
            // Se convierte la imagen a escala de grises.
            grayScaleImage = convertGrayScale(normalImage);
            int imageBufferLengthWrite = grayScaleImage.height * grayScaleImage.width * grayScaleImage.color_channel * sizeof(int);

            // Se escribe la imagen en escala de grises en la salida del padre.
            write(STDOUT_FILENO, &grayScaleImage, sizeof(Image));
            write(STDOUT_FILENO, grayScaleImage.image_buffer, imageBufferLengthWrite);
        }
        wait(NULL);
    }
    return 0;
}
