#ifndef _MENU_H
#define _MENU_H
 
/*
    Funciones del Menu.
    Menu Functions.
*/

void showMenu();
void validateOption(int* option, int min, int max);
void cleanScreen();
void pressToContinue();
void about();
void menu(int* option, int min, int max);
#endif