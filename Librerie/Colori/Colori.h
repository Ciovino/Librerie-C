#ifndef COLORI_H_INCLUDED
#define COLORI_H_INCLUDED

/*
    Vuoi rendere più figo il tuo programma con tanti bei colori?
    Usa questa bella libreria e dai sfogo alla tua fantasia.
*/

typedef enum
{
    COL_BLACK,
    COL_BLUE,
    COL_GREEN,
    COL_CYAN,
    COL_RED,
    COL_MAGENTA,
    COL_BROWN,
    COL_NORMAL, // COL_LIGHT_GRAY
    COL_DARK_GRAY,
    COL_LIGHT_BLUE,
    COL_LIGHT_GREEN,
    COL_LIGHT_CYAN,
    COL_LIGHT_RED,
    COL_PINK,
    COL_YELLOW,
    COL_WHITE,
} COLOR;

void Textcolor(COLOR);
void BackGroundAndText(COLOR, COLOR);

#endif // COLORI_H_INCLUDED