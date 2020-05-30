#include <stdio.h>
#include <stdlib.h>
#include <menu.h>

void showMenu()
{
    printf("\n***------------------------------***\n");
    printf("**                                **\n");
    #ifdef DEBUG
    printf("*     Laboratorio 1(Debug Mode)    *\n");
    #endif
    #ifndef DEBUG
    printf("*           Laboratorio 1          *\n");
    printf("*     Sistemas Operativos 1-2020   *\n");
    #endif
    printf("*            Version 0.1           *\n");
    printf("**                                **\n");
    printf("***------------------------------***\n");
    printf("\n");
    printf("1.- Iniciar programa.\n");
    printf("2.- Acerca de.\n");
    printf("3.- Salir.\n");
    printf("\nIngrese la opcion deseada: ");
}

void validateMenuOption(int* option, int min, int max)
{
    do
    {
        fflush(stdin);
        if ((scanf("%d", option) == 0) || (*option < min || *option > max))
        {
            while (getchar() != '\n');
            printf("Error. Ingrese una opcion valida: ");
            fflush(stdin);
        }
         
    } while (*option < min || *option > max);
}
  
void cleanScreen()
{
    #if defined(__linux__) || defined(__unix__) || defined(__APPLE__)
        system("clear");
    #endif
 
    #if defined(_WIN32) || defined(_WIN64)
        system("cls");
    #endif
}

void pressToContinue()
{
    fflush(stdin);
    printf("\nPresione intro para continuar...");
    getchar();
    #if defined(__linux__) || defined(__unix__) || defined(__APPLE__)
        getchar();
    #endif      
}

void about()
{
	cleanScreen();
    printf("Laboratorio 1 - Sistemas Operativos 1-2020.\n");
    printf("Universidad de Santiago de Chile\n\n");
    printf("Desarrollado por:- Cristobal Medrano A. (19.083.864-1)\n");
    printf("                 - Mauricio Soto P. (19.603.152-9)\n\n");
    printf("Contacto: cristobal.medrano@usach.cl - mauricio.soto.p@usach.cl\n");
	pressToContinue();
}

void menu(int* option, int min, int max)
{
	cleanScreen();
	showMenu();
	validateMenuOption(option, min, max);
}