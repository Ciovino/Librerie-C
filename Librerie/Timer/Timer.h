#ifndef TIMER_H_INCLUDED
#define TIMER_H_INCLUDED

/*
    C'è sempre bisogno di dare un tempo alle nostre attività.
    Nel caso di un programma, puoi usare questa libreria.

    Note: Sono necessarie le librerie "Screen" e "Colori", che si
        trovano sempre in questo repository.
        Le due librerie devono essere copiate in una cartella "Supporto",
        nella stessa cartella di dove viene salvata la libreria Timer.
        Si può impostare diversamente andando a modificare il file "Timer.c".
*/

typedef struct timer *Timer;

Timer NewTimer(void);
int SetTimerReady(Timer T, int Ore, int Minuti, int Secondi);
void ShowTimerStatus(Timer T);
int StartTimer(Timer T);
void FreeTimer(Timer T);

void delay(int MilliSeconds);

#endif // TIMER_H_INCLUDED