#include <stdlib.h> // srand(); rand()
#include <time.h>   // time()

#include "Random.h"

/**
 * @brief Inizializza un seme per generare numeri casuali
 */
static int INIT = 0;
static void Init()
{
    if (!INIT)
    {
        srand(time(0));
        INIT = 1;
    }
}

/**
 * @brief ritorna un intero casuale tra Min e Max
 *
 * @param Min Valore minimo possibile (incluso)
 * @param Max Valore massimo possibile (escluso)
 *
 * @return int
 */
int RandomInt(int Min, int Max)
{
    if (Max == Min)
        return 0;

    if (Min > Max)
        return RandomInt(Max, Min);

    Init();

    return (rand() % (Max - Min)) + Min;
}

/**
 * @brief Ritorna un intero casuale tra 0 e Max
 *
 * @param Max Massimo valore possibile (escluso)
 *
 * @return int
 */
int RandomIntFrom0ToMax(int Max)
{
    return RandomInt(0, Max);
}
