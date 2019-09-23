#include "server.h"

int main(int argc, char const *argv[]) {

    int seed;
    if (argc < 2) {
        seed = 0;
    }
    else {
        seed = *argv[1];
    }

    std::array<Card, CARD_NO> cards;
    for (size_t i = 0; i < CARD_NO; i++) {
        cards[i] = Card(i);
    }
    cards = shuffle(cards, seed);

    Klondike game = Klondike(cards);

    game.printGame();
    game.findMoves();

    return 0;
}
