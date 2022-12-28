#include <stdio.h> // printf()

#include "Colori.h"

void main()
{
    printf("Colore Normale\n");
    Textcolor(COL_GREEN);
    printf("Colore Verde\n");
    Textcolor(COL_RED);
    printf("Colore Rosso\n");
    BackGroundAndText(COL_LIGHT_BLUE, COL_MAGENTA);
    printf("Sfondo blu chiaro, testo magenta\n");
    Textcolor(COL_NORMAL);
    printf("Colore Normale\n");
}