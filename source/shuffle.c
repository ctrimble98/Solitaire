#include "server.h"

//Durstenfeld shuffle
void shuffle(int cards[], int seed) {

    srand(seed);

    int temp, index;

    for (int i = CARD_NO - 1; i > 1; i--) {

        index = rand() % i;
        temp = cards[index];
        cards[index] = cards[i];
        cards[i] = temp;
    }
}
