#include "server.h"

int main(int argc, char const *argv[]) {

    std::array<Card, CARD_NO> cards;
    for (uint8_t i = 0; i < CARD_NO; i++) {
        cards[i] = Card(i);
    }
    shuffle(cards, *argv[1]);
    for (size_t i = 0; i < 52; i++) {
        std::cout << cards[i].getRank() << ", ";
    }
    std::cout << std::endl;

    // stock = cards;
    // tableau = [STACKS];

    return 0;
}
