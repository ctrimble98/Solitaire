#include "server.h"

//Durstenfeld shuffle
std::array<Card, CARD_NO> shuffle(std::array<Card, CARD_NO> cards, int seed) {
    srand(seed);
    Card temp;
    int index;

    for (int i = CARD_NO - 1; i > 1; i--) {

        index = rand() % i;
        temp = cards.at(index);
        cards.at(index) = cards.at(i);
        cards.at(i) = temp;
    }
    return cards;
}
