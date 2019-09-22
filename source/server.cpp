#include "server.h"

int main(int argc, char const *argv[]) {

    std::array<Card, CARD_NO> cards;
    for (size_t i = 0; i < CARD_NO; i++) {
        cards[i] = Card(i);
    }
    shuffle(cards, *argv[1]);
    // for (size_t i = 0; i < CARD_NO; i++) {
    //     std::cout << cards[i].getRank() << ", ";
    // }
    // std::cout << std::endl;

    Klondike game = Klondike(cards);

    game.printGame();

    return 0;
}
