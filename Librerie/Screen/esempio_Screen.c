#include <conio.h> // getch()
#include <stdio.h> // printf()

#include "Screen.h"

void main()
{
    printf("Premi spazio per cancellare tutto\n");
    getch();
    ClearAndHome();

    printf("Premi spazio per spostare il cursore all'inizio, senza cancellare\n");
    getch();
    SetCursorToHome();

    printf("Premi spazio per cancellare tutto, ma senza spostare il cursore\n");
    getch();
    ClearScreen();

    printf("Ole'\n");
}