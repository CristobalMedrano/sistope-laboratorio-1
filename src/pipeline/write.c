//Bibliotecas utilizadas
#include <stdlib.h>
#include <stdio.h>
#include <jpeglib.h>
#include <string.h>
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

//Cabeceras
void writeImage(Image image, int imageNumber);
int writeJPG(Image* image, int imageNumber, char* filename, struct jpeg_error_mgr* jerr);

//Entradas:
//  Image image: Estructura con la imagen a escribir.
//  int imageNumber: Entero que representa el número asociado a la imagen.
//
//Funcionamiento: Procedimiento que realiza la segunda parte de la quinta etapa del pipeline, es decir, escribir en disco
//                la imagen resultante en formato jpg.
//Salida: No tiene por ser un procedimiento.
void writeImage(Image image, int imageNumber){
    // Se procede a generar y dar formato al nombre del archivo de salida
    char filename[50] = "";
    char number[100];
    sprintf(number, "%d", imageNumber);
    strcat(filename, "salida_imagen_");
    strcat(filename, number);
    strcat(filename, ".jpg");
    // Estructura para el manejador de errores JPEG
    struct jpeg_error_mgr jerr;
    // Se realiza la escritura y valida su funcionamiento
    if (!writeJPG(&image, imageNumber, filename, &jerr)){
        // En caso de error, se muestra mensaje por pantalla.
        fprintf(stderr, "fallo writeJPG\n");;
    }
}

//Entradas:
// Image* image: Puntero a estructura imagen.
// int imageNumber: Entero que representa el número de una imagen.
// char* filename: String que contiene el nombre del archivo a escribir.
// struct jpeg_error_mgr* jerr: estructura que representa el manejador de errores de jpeglib.
//
//Funcionamiento: Función que presenta la rutina que permite la escritura o compresión JPEG y ocupa elementos de jpeglib.
//
//Salida: Devuelve un entero. Si retorna verdadero (TRUE 1), representa éxito en la lectura. 
//         Por el contrario, falso (FALSE 0) en caso de error.
int writeJPG(Image* image, int imageNumber, char* filename, struct jpeg_error_mgr* jerr){
    // Estructura que contiene los parámetros y punteros de compresión JPEG 
    // al espacio de trabajo, definidos según jpeglib.h
    struct jpeg_compress_struct cinfo;
    
    // Se abre el archivo de entrada modo escritura con "b" para leer archivos binarios.
    FILE *fp = fopen(filename, "wb");
    if (fp == NULL) {
        fprintf(stderr, "Error al escribir el archivo %s\n", filename);
        return FALSE;
    }

    // Se configuran las rutinas de error JPEG normales.
    cinfo.err = jpeg_std_error(jerr);

    // Se inicializa el objeto de compresión JPEG.
    jpeg_create_compress(&cinfo);
    jpeg_stdio_dest(&cinfo, fp);

    // Se establecen los parámetros para la compresión
    cinfo.image_width = image->width;
    cinfo.image_height = image->height;
    cinfo.input_components = image->color_channel;
    //cinfo.in_color_space = JCS_GRAYSCALE;

    // También se establecen aquellos parámetros de compresión predeterminados.
    jpeg_set_defaults(&cinfo);

    // Se inicia la compresión
    jpeg_start_compress(&cinfo, TRUE);
    
    int row_stride = image->width * image->color_channel;
    JSAMPLE* buffer = (JSAMPLE*)image->image_buffer;
    int i = 0;
    // Se realiza la escritura escaneando líneas, teniendo un único elemento de largo
    for (i=0; i< image->height; i++) {
        jpeg_write_scanlines(&cinfo, &buffer, 1);
        buffer += row_stride;
    }

    // Se finaliza la compresión
    jpeg_finish_compress(&cinfo);
    // Se libera memoria del objeto de compresión
    jpeg_destroy_compress(&cinfo);
    // Se cierra el archivo
    fclose(fp);
    // En este punto, se retorna (TRUE 1) indicando éxito.
    return TRUE;
}

//Entradas:
//  Image image: Estructura con la imagen clasificada.
//
//Funcionamiento: Función encargada de copiar el buffer de una imagen.
//
//Salida: Retorna la estructura de una imagen.
Image writedImage(Image image){
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

        Image nextImage = {};
        nextImage.image_buffer = NULL;

        nextImage = writedImage(binarizedImage);

        // Se escribe la imagen resultante en disco.
        writeImage(nextImage, i);
    }
    return 0;
}