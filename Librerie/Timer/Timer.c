#include <conio.h>
#include <stdio.h>
#include <stdlib.h>
#include <time.h>

#include "Supporto/Colori.h"
#include "Supporto/Screen.h"
#include "Timer.h"

#define DELAY 1000

typedef enum {
    DISATTIVO,
    PRONTO,
    ATTIVO,
    PAUSA,
    CONCLUSO,
} STATO_TIMER;

struct timer {
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

    if (t == NULL) {
        Textcolor(COL_RED);
        fprintf(stderr, "NewTimer() errore: Impossibile allocare memoria\n");
        Textcolor(COL_NORMAL);
    } else {
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
 * @return 0 se il Timer è Null, -1 se i valori passati sono negativi, 2 se i valori passati devono essere convertiti
 * (es: Minuti == 60 => Ore++), 1 altrimenti
 */
int SetTimerReady(Timer T, int Ore, int Minuti, int Secondi)
{
    // Timer nullo
    if (T == NULL) {
        Textcolor(COL_RED);
        fprintf(stderr, "SetTimer() errore: Timer è nullo\n");
        Textcolor(COL_NORMAL);
        return 0;
    }

    // Parametri negativi
    if (Ore < 0 || Minuti < 0 || Secondi < 0) {
        Textcolor(COL_RED);
        fprintf(stderr, "SetTimer() errore: I parametri inseriti sono negativi\n");
        Textcolor(COL_NORMAL);
        return -1;
    }

    // Controlla se i parametri devono essere convertiti
    int returnValue = 1;

    // Secondi
    while (Secondi >= 60) {
        returnValue = 2;

        Minuti++;
        Secondi -= 60;
    }

    // Minuti
    while (Minuti >= 60) {
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
    if (T == NULL) {
        Textcolor(COL_RED);
        fprintf(stderr, "ShowTimerStatus() errore: Timer è nullo\n");
        Textcolor(COL_NORMAL);
        return;
    }

    switch (T->stato) {
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
        fprintf(stderr, "ShowTimerStatus() errore: Stato del Timer non valido\n");
        Textcolor(COL_NORMAL);
        break;
    }
}

static void DecrementTimer(Timer T)
{
    if (T->Secondi == 0) {
        if (T->Minuti == 0) {
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
    while (T->stato == ATTIVO) {
        if (secondPassed) {
            secondPassed = 0;
            DecrementTimer(T);

            start = clock();
            offset = T->millisecond = 0;

            ShowTimerStatus(T);
        } else {
            T->millisecond = clock() - start + offset;

            if (T->millisecond >= DELAY) secondPassed = 1;
        }

        // Timer in pausa
        if (kbhit()) {
            if (getch() == 32) // Spacebar pressed
            {
                T->stato = PAUSA;
            }
        }

        // Timer finito
        if (T->Ore == 0 && T->Minuti == 0 && T->Secondi == 0) {
            T->stato = CONCLUSO;
            ShowTimerStatus(T);
        }
    }
}

/**
 * @brief Fai partire il timer
 *
 * @param T Timer
 *
 * @returns -1 in caso di errore (timer nullo, timer ancora attivo, stato del timer invalido), 0 se il timer non è
 * stato ancora impostato, 1 se il timer si è concluso, 2 se il timer è in pausa, 3 se il timer è scaduto e non è ancora
 * stato impostato
 */
int StartTimer(Timer T)
{
    if (T == NULL) {
        Textcolor(COL_RED);
        fprintf(stderr, "StartTimer() errore: Timer è nullo\n");
        Textcolor(COL_NORMAL);
        return -1;
    }

    int returnValue = 0;

    switch (T->stato) {
    case DISATTIVO:
        Textcolor(COL_YELLOW);
        fprintf(stderr, "StartTimer() warning: Timer non impostato\n");
        Textcolor(COL_NORMAL);
        returnValue = 0;
        break;

    case PRONTO: T->millisecond = DELAY;
    case PAUSA:
        T->stato = ATTIVO;
        _INTERNAL_StartTimer(T);

        if (T->stato == CONCLUSO) {
            returnValue = 1;
        } else if (T->stato == PAUSA) {
            returnValue = 2;
        }

        break;

    case CONCLUSO:
        Textcolor(COL_YELLOW);
        fprintf(stderr, "StartTimer() warning: Timer scaduto. Impostalo prima di avviarlo\n");
        Textcolor(COL_NORMAL);
        returnValue = 3;
        break;

    case ATTIVO: // Impossibile, ma ok
        Textcolor(COL_RED);
        fprintf(stderr, "StartTimer() errore: Timer è già attivo\n");
        Textcolor(COL_NORMAL);
        returnValue = -1;
        break;

    default:
        Textcolor(COL_RED);
        fprintf(stderr, "StartTimer() errore: Stato del Timer non valido\n");
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
    if (T != NULL) free(T);
}

/**
 * @brief Ritarda il programma
 *
 * @param MilliSeconds
 */
void delay(int MilliSeconds)
{
    clock_t time = clock() + MilliSeconds;

    while (time > clock()) {}
}
