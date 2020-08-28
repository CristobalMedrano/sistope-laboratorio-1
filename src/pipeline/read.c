// Bibliotecas utilizadas
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

int readJPG(char* filename, Image* image, struct jpeg_error_mgr* jerr);
void printPixels(Image image);
Image readImage(int imageNumber);


//Entradas: 
//  char* filename: Nombre del archivo de imagen
//  Image* image: puntero a estructura con imagen almacenada
//  struct jpeg_error_mgr* jerr: estructura que representa el manejador de errores de jpeglib
//
//Procedimiento: Función que presenta la rutina que permite la lectura o descompresión JPEG y ocupa elementos de jpeglib.
//
//Salidas: Devuelve un entero. Si retorna verdadero (TRUE 1), representa éxito en la lectura. 
//         Por el contrario, falso (FALSE 0) en caso de error.
int readJPG(char* filename, Image* image, struct jpeg_error_mgr* jerr){
    // Estructura que contiene los parámetros y punteros de descompresión JPEG 
    // al espacio de trabajo, definidos según jpeglib.h
    struct jpeg_decompress_struct cinfo;

    // Se configuran las rutinas de error JPEG normales
    cinfo.err = jpeg_std_error(jerr);

    // Se abre el archivo de entrada modo lectura con "b" para leer archivos binarios.
    FILE *fp = fopen(filename, "rb");
    if (fp == NULL) {
        printf("Error al abrir el archivo %s\n", filename);
        return FALSE;
    }

    // Se inicializa el objeto de descompresión JPEG
    jpeg_create_decompress(&cinfo);
    // Se especifica la fuente u origen de los datos (archivo fp)
    jpeg_stdio_src(&cinfo, fp);
    // Se leen los parámetros del archivo
    jpeg_read_header(&cinfo, 1);
    // Se inicia el descompresor
    jpeg_start_decompress(&cinfo);

    // Como en este punto ya se tienen las dimensiones de la imagen, se guardan
    // y se crea el buffer necesario para su posterior uso.
    image->height = cinfo.image_height;
    image->width = cinfo.image_width;
    image->color_channel = cinfo.num_components;
    image->image_buffer = NULL;
    image->image_buffer = (JSAMPLE*) malloc(sizeof(int) *
                                       image->width  *
                                       image->height *
                                       image->color_channel);

    int row_stride = image->width * image->color_channel;
    JSAMPLE* buffer = (JSAMPLE*)image->image_buffer;
    
    for (int i = 0; i < image->height; i++)
    {
        // Se realiza el escaneo o lectura de lineas, en este caso tenemos solo un elemento de largo
        jpeg_read_scanlines(&cinfo, &buffer, 1);
        buffer += row_stride;
    }
    // Se finaliza la descompresión
    jpeg_finish_decompress(&cinfo);
    // Se libera la memoria del objeto de descompresión
    jpeg_destroy_decompress(&cinfo);
    // Se cierra el archivo de entrada
    fclose(fp);
    // En este punto se retorna 1 indicando éxito.
    return TRUE;
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

//Entrada:
//  int imageNumber: Entero que representa el número asociado a una imagen.
//
//Funcionamiento: Función encargada de realizar la primera etapa del pipeline, es decir, lee la imagen "i".
//
//Salida: Retorna la estructura de la imagen, útil para desarrollar las próximas etapas.
Image readImage(int imageNumber){
    // Se inicializa la estructura imagen
    Image image;
    // Se inicializa el nombre del archivo e imagen "i", según la entrada.
    char fileName[40] = "";
    char imageName[40] = "";
    sprintf(fileName, "./imagen_%i.jpg", imageNumber);
    sprintf(imageName, "image_%i.jpg", imageNumber);
    // Estructura para el manejador de errores JPEG
    struct jpeg_error_mgr jerr;
    // Se realiza la lectura y valida su funcionamiento
    if (!readJPG(fileName, &image, &jerr)){
        // Se indica por pantalla en caso de error
        fprintf(stderr, "fallo readJPG\n");
    }
    // Se muestra por pantalla los pixeles (se omite para efectos finales)
    //printPixels(image);
    
    // Se retorna la estructura con imagen leida
    return image;
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
    
    /*printf("Numero de imagenes: %d\n", numberImages);
    printf("Umbral de binarizacion: %d\n", binarizationThreshold);
    printf("Umbral de clasificacion: %d\n", classificationThreshold);
    printf("Nombre de la mascara: %s\n", maskFilename);
    if (flagShowResults == TRUE) printf("Mostrar resultados: Si\n");
    if (flagShowResults == FALSE) printf("Mostrar resultados: No\n");*/

    
    // Se crea el pipe de read->grayscale (leer imagen a escala de grises)
    int pipedes[2];
    pipe(pipedes);

    // Se crea el hijo
    pid_t pid = fork();
    if (pid < 0){ 
        // No se creo el hijo
        perror("Existe un error en el Fork de 'read.c'\n");
        exit(-1);
        
    } else if (pid == 0){
        // Soy el hijo

        // Conexion entre la lectura del pipe y la entrada del hijo.
        dup2(pipedes[READ], STDIN_FILENO);
        close(pipedes[READ]);
        close(pipedes[WRITE]);
        // Se realiza un EXECVP para reemplazar este proceso con la segunda etapa (Escala de grises) del pipeline
        char *args[] = {"grayscale.out", argv[1], argv[2], argv[3], argv[4], argv[5], NULL};
        if (execvp("src/pipeline/grayscale.out", args) < 0) exit(0);
    } else {
        // Soy el padre

        // Conexion entre la escritura del pipe y la salida del padre.
        dup2(pipedes[WRITE], STDOUT_FILENO);
        close(pipedes[READ]);
        close(pipedes[WRITE]);

        int i = 0;
        for (i = 1; i <= numberImages; i++)
        {
            // Se inicializa en 0 la memoria de la imagen
            Image normalImage = {};
            normalImage.image_buffer = NULL;

            // Se obtiene la imagen desde el archivo.
            normalImage = readImage(i);

            int imageBufferLengthWrite = normalImage.height * normalImage.width * normalImage.color_channel * sizeof(int);
            
            // Se escribe la imagen leida en la salida del padre.
            write(STDOUT_FILENO, &normalImage, sizeof(Image));
            write(STDOUT_FILENO, normalImage.image_buffer, imageBufferLengthWrite);
        }
        wait(NULL);
    }
    return 0;
}
