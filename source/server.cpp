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

    game.printGame(true);
    std::vector<std::array<int, 4>> moves = game.findMoves();
    int j = 0;
    srand(seed);
    while (moves.size() > 0 && j < 100) {
        game.makeMove(moves[rand() % moves.size()]);
        game.printGame(true);
        moves = game.findMoves();
        j++;
    }

    return 0;
}
