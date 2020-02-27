#include "klondike.h"

//Durstenfeld shuffle
std::array<Card, CARD_NO> shuffle(std::array<Card, CARD_NO> cards, int seed) {

    Card temp;
    int index;

    std::mt19937 eng(seed);

    for (int i = CARD_NO - 1; i > 1; i--) {
        std::uniform_int_distribution<> distr(0 , i);
        index = distr(eng);
        temp = cards.at(index);
        cards.at(index) = cards.at(i);
        cards.at(i) = temp;
    }
    return cards;
}
