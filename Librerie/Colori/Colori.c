#include <windows.h>
#include "Colori.h"

/**
 * @brief Cambia il colore del testo
 *
 * @param color
 */
void Textcolor(COLOR color) // Funzione per i colori
{
    HANDLE hconsole = GetStdHandle(STD_OUTPUT_HANDLE);
    if (hconsole == INVALID_HANDLE_VALUE)
        return;
    SetConsoleTextAttribute(hconsole, color);
}

/**
 * @brief Cambia il colore del testo e dello sfondo del terminale
 *
 * @param BackGround
 * @param Text
 */
void BackGroundAndText(COLOR BackGround, COLOR Text)
{
    Textcolor(BackGround * 16 + Text);
}