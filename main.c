

int main(int argc, char *argv[]) {
    int cValue = 0;
    char* mValue = NULL;
    int nValue = 0;
    int bFlag = 0;
    int c;

    while ((c = getopt (argc, argv, "c:m:n:b")) != -1)
    {
      switch (c){
        case 'c':
            cValue = atoi(optarg);		  //cantidad imágenes.
            printf("cantidad imagenes: %d\n", cValue);
            break;
        case 'm':
            mValue = optarg;		      //máscara.
            printf("mascara: %s\n", mValue);
            break;
        case 'n':
            nValue = atoi(optarg);	      //umbral.
            printf("umbral de clasificacion: %d\n", nValue);
            break;
        case 'b':
            bFlag = 1;				     //bandera.
            printf("bandera: %d\n", bFlag);
            break;
        default:
            abort ();
      }
    }

    //Validaciones de entradas
    if (cValue <= 0 || cValue > 6){
        printf("ERROR. Cantidad de imagenes incorrecta\n");
        return -1;
	}
    else if (fopen(mValue, "rb") == NULL){
        printf("ERROR. Archivo de mascara no existe.\n");
        return -1;
    }
    else if (nValue < 0){
        printf("ERROR. Umbral invalido (negativo).\n");
        return -1;
    }
        
    for (int i = 1; i <= cValue; i++){
        char imageName[40];
        char dir[40] = "imagen_";
        char imageNumber[100];
        int rows, columns;
        sprintf(imageNumber, "%d", i);
        strcpy(imageName, strcat(strcat(dir, imageNumber), ".png"));
        FILE * fp;
        fp = fopen(imageName, "r");

        if (! fp) {
            printf("Error. Archivo no encontrado / invalido.");
            abort();
        }
    }
}
