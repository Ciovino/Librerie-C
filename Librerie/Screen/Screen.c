#include <stdio.h> // printf()

#include "Screen.h"

/**
 * @brief Ripulisci il terminale, ma mantieni la posizione del cursore
 */
void ClearScreen()
{
    printf("\x1b[2J");
}

/**
 * @brief Sposta il cursore all'inizio, ma senza cancellare il terminale
 */
void SetCursorToHome()
{
    printf("\x1b[H");
}

/**
 * @brief Ripulisci il terminale e sposta il cursore all'inizio
 */
void ClearAndHome()
{
    ClearScreen();
    SetCursorToHome();
}