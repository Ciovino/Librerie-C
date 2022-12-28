#include <stdio.h> // printf()

#include "Random.h"

void main()
{
    printf("Dieci numeri a caso tra 0 e 15:\n");
    for(int i = 0; i < 10; i++)
        printf("%d ", RandomIntFrom0ToMax(16));
    
    printf("\nDieci numeri a caso tra 10 e 20:\n");
    for(int i = 0; i < 10; i++)
        printf("%d ", RandomInt(10, 21));
    printf("\n");
}