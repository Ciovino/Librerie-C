#include <stdio.h>
#include <stdlib.h>
#include <conio.h>
#include <time.h>

#include "Timer.h"
#include "Supporto/Screen.h"
#include "Supporto/Colori.h"

#define DELAY 1000

typedef enum
{
    DISATTIVO,
    PRONTO,
    ATTIVO,
    PAUSA,
    CONCLUSO,
} STATO_TIMER;

struct timer
{
    int Ore;
    int Minuti;
    int Secondi;

    clock_t millisecond;

    STATO_TIMER stato;
};

/**
 * @brief Crea un nuovo Timer
 *
 * @return Timer
 */
Timer NewTimer()
{
    Timer t;

    t = malloc(sizeof(*t));

    if (t == NULL)
    {
        Textcolor(COL_RED);
        fprintf(stderr, "NewTimer() error: Out of memory\n");
        Textcolor(COL_NORMAL);
    }
    else
    {
        t->Ore = 0;
        t->Minuti = 0;
        t->Secondi = 0;

        t->stato = DISATTIVO;
        t->millisecond = 0;
    }

    return t;
}

/**
 * @brief Imposta il timer
 *
 * @param T Timer
 * @param Ore
 * @param Minuti
 * @param Secondi
 * 
 * @return 0 se il Timer è Null, -1 se i valori passati sono negativi, 
 *         2 se i valori passati devono essere convertiti (es: Minuti == 60 => Ore++), 1 altrimenti
 */
int SetTimerReady(Timer T, int Ore, int Minuti, int Secondi)
{
    if (T == NULL)
    {
        Textcolor(COL_RED);
        fprintf(stderr, "SetTimer() error: Timer is NULL\n");
        Textcolor(COL_NORMAL);
        return 0;
    }

    if (Ore < 0 || Minuti < 0 || Secondi < 0)
    {
        Textcolor(COL_RED);
        fprintf(stderr, "SetTimer() error: Params are negative\n");
        Textcolor(COL_NORMAL);
        return -1;
    }

    // Check for conversion
    int returnValue = 1;

    // Secondi
    while (Secondi >= 60)
    {
        returnValue = 2;

        Minuti++;
        Secondi -= 60;
    }

    // Minuti
    while (Minuti >= 60)
    {
        returnValue = 2;

        Ore++;
        Minuti -= 60;
    }

    T->Ore = Ore;
    T->Minuti = Minuti;
    T->Secondi = Secondi;
    T->stato = PRONTO;

    return returnValue;
}

/**
 * @brief Stampa il timer
 *
 * @param T Timer
 */
void ShowTimerStatus(Timer T)
{
    if (T == NULL)
    {
        Textcolor(COL_RED);
        fprintf(stderr, "ShowTimerStatus() error: Timer is NULL\n");
        Textcolor(COL_NORMAL);
        return;
    }

    switch (T->stato)
    {
    case DISATTIVO:
        // Valore Timer
        printf("%d:%02d:%02d ", T->Ore, T->Minuti, T->Secondi);

        // Avvisi
        Textcolor(COL_YELLOW);
        printf("(ShowTimerStatus() warning: Timer disattivo. Impostalo prima di avviarlo)\n");
        Textcolor(COL_NORMAL);
        break;
    case PRONTO:
        // Valore Timer
        printf("%d:%02d:%02d ", T->Ore, T->Minuti, T->Secondi);

        // Avvisi
        Textcolor(COL_LIGHT_GREEN);
        printf("(ShowTimerStatus() warning: Timer pronto per essere avviato)\n");
        Textcolor(COL_NORMAL);
        break;
    case PAUSA:
        // Valore Timer
        printf("%d:%02d:%02d:%03d ", T->Ore, T->Minuti, T->Secondi, DELAY - T->millisecond);

        // Avvisi
        Textcolor(COL_CYAN);
        printf("(ShowTimerStatus() warning: Timer in pausa)\n");
        Textcolor(COL_NORMAL);
        break;
    case CONCLUSO:
        // Valore Timer
        printf("%d:%02d:%02d ", T->Ore, T->Minuti, T->Secondi);

        // Avvisi
        Textcolor(COL_YELLOW);
        printf("(ShowTimerStatus() warning: Timer concluso. Impostalo prima di avviarlo)\n");
        Textcolor(COL_NORMAL);
        break;
    case ATTIVO:
        ClearAndHome();
        printf("%d:%02d:%02d\n", T->Ore, T->Minuti, T->Secondi);

        Textcolor(COL_CYAN);
        printf("Premi 'Spazio' per mettere in pausa\n");
        Textcolor(COL_NORMAL);
        break;

    default:
        Textcolor(COL_RED);
        fprintf(stderr, "ShowTimerStatus() error: Invalid Timer state\n");
        Textcolor(COL_NORMAL);
        break;
    }
}

static void DecrementTimer(Timer T)
{
    if (T->Secondi == 0)
    {
        if (T->Minuti == 0)
        {
            T->Ore--;
            T->Minuti = 60;
        }
        T->Minuti--;
        T->Secondi = 60;
    }
    T->Secondi--;
}

static void _INTERNAL_StartTimer(Timer T)
{
    int secondPassed = 0;
    clock_t start = clock(), offset = T->millisecond;

    ShowTimerStatus(T);
    while (T->stato == ATTIVO)
    {
        if (secondPassed)
        {
            secondPassed = 0;
            DecrementTimer(T);

            start = clock();
            offset = T->millisecond = 0;

            ShowTimerStatus(T);
        }
        else
        {
            T->millisecond = clock() - start + offset;

            if (T->millisecond >= DELAY)
                secondPassed = 1;
        }

        // Timer in pausa
        if (kbhit())
        {
            if (getch() == 32) // Spacebar pressed
            {
                T->stato = PAUSA;
            }
        }

        // Timer finito
        if (T->Ore == 0 && T->Minuti == 0 && T->Secondi == 0)
        {
            T->stato = CONCLUSO;
            DoMiSolDo();
        }
    }
}

/**
 * @brief Fai partire il timer
 *
 * @param T Timer
 */
int StartTimer(Timer T)
{
    if (T == NULL)
    {
        Textcolor(COL_RED);
        fprintf(stderr, "StartTimer() error: Timer is NULL\n");
        Textcolor(COL_NORMAL);
        return -1;
    }

    int returnValue = 0;

    switch (T->stato)
    {
    case DISATTIVO:
        Textcolor(COL_YELLOW);
        fprintf(stderr, "StartTimer() warning: Timer not set\n");
        Textcolor(COL_NORMAL);
        returnValue = 0;
        break;
    case PRONTO:
        T->millisecond = DELAY;
    case PAUSA:
        T->stato = ATTIVO;
        _INTERNAL_StartTimer(T);
        returnValue = 1;
        break;
    case CONCLUSO:
        Textcolor(COL_YELLOW);
        fprintf(stderr, "StartTimer() warning: Timer already finished. Set it again\n");
        Textcolor(COL_NORMAL);
        returnValue = 2;
        break;
    case ATTIVO: // Impossibile, ma ok
        Textcolor(COL_RED);
        fprintf(stderr, "StartTimer() error: Timer is still active\n");
        Textcolor(COL_NORMAL);
        returnValue = -1;
        break;

    default:
        Textcolor(COL_RED);
        fprintf(stderr, "StartTimer() error: Invalid Timer state\n");
        Textcolor(COL_NORMAL);
        returnValue = -1;
        break;
    }

    return returnValue;
}

/**
 * @brief Dealloca il timer
 *
 * @param T Timer
 */
void FreeTimer(Timer T)
{
    if (T != NULL)
        free(T);
}

/**
 * @brief Ritarda il programma
 *
 * @param MilliSeconds
 */
void delay(int MilliSeconds)
{
    clock_t time = clock() + MilliSeconds;

    while (time > clock())
        ;
}
