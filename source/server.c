#include "server.h"

int main(int argc, char const *argv[]) {

    int cards[CARD_NO];
    for (size_t i = 0; i < CARD_NO; i++) {
        cards[i] = i;
    }
    shuffle(cards, *argv[1]);
    for (size_t i = 0; i < 52; i++) {
        printf("%d, ", cards[i]);
    }
    printf("\n");

    stock = cards;
    tableau = [STACKS];
    
    foundation

    return 0;
}
