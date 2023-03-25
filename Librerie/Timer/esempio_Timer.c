#include <conio.h> // getch()
#include <stdio.h> // printf()

#include "Timer.h"

void main()
{
    Timer esempioTimer = NewTimer();

    // Crea un timer di 30 secondi
    SetTimerReady(esempioTimer, 0, 0, 30);

    // Fai partire il timer
    while (StartTimer(esempioTimer) != 1) {
        ShowTimerStatus(esempioTimer);
        getch();
    }

    // Dealloca il timer
    FreeTimer(esempioTimer);
}