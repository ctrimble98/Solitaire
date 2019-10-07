#ifndef KLONDIKE_H
#define KLONDIKE_H

//#include "server.h"
#include "card.h"

class Klondike {
public:

    std::vector<Card> getStock();
    std::array<std::vector<Card>, 7> getTableau();
    std::array<std::stack<Card>, 4> getFoundation();
    bool isWon();

    Klondike(int seed);

    std::vector<std::array<int, 4>> findMoves(bool allLegalMoves);
    void makeMove(std::array<int, 4> move);
    void placeCards(std::array<int, 4> move, std::vector<Card> cardsToMove);

    void printGame(bool hideFaceDown);

private:

    bool won;
    std::vector<Card> stock;
    std::array<std::vector<Card>, 7> tableau;
    std::array<std::stack<Card>, 4> foundation;

    bool evalMove(Card dest, Card pot);
};

#endif
