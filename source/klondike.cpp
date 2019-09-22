#include "klondike.h"


Klondike::Klondike(std::array<Card, CARD_NO> cards) {

    for (size_t i = 0; i < STOCK_SIZE; i++) {
        stock.push_back(cards[i]);
    }

    for (size_t i = 0; i < STACKS; i++) {
        for (size_t j = 0; j <= i; j++) {
            tableau[i].push_back(cards[STOCK_SIZE + i + j]);
            if (j < i) {
                tableau[i][j].turnFaceDown();
            }
        }
    }
}

void Klondike::printGame() {

    std::cout << stock[0].getRank() << std::endl;
}
