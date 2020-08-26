//Librerías utilizadas
#include <stdlib.h>
#include <stdio.h>
#include <jpeglib.h>
#include <inttypes.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <unistd.h>

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

void printPixels(Image image){
    // Se muestra por pantalla el alto, ancho y los canales que posee la imagen de entrada.
    printf("width = %" PRIu32 "\n", image.width);
    printf("height = %" PRIu32 "\n", image.height);
    printf("channels = %" PRIu32 "\n", image.color_channel);
    // Se inicializan variables
    uint8_t num = 0;
    int loc = 0;
    // Se recorre la imagen e imprime por pantalla el valor de sus pixeles.
    for (int i = 0; i < image.height; i++)
    {
        for (int j = 0; j < image.width*image.color_channel; j++)
        {
            num = image.image_buffer[loc];
            printf("%" PRId8 " ", num);
            loc++;
        }
        printf("\n");
    }
}

int main(int argc, char const *argv[])
{
    Image grayScaleImage = {};
    Image normalImage = {};
    read(STDIN_FILENO, &normalImage, sizeof(Image));
    printPixels(normalImage);
    //grayScaleImage = convertGrayScale(normalImage);

    //free(grayScaleImage.image_buffer);

    return 0;
}
