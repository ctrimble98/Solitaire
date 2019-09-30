#ifndef KLONDIKE_H
#define KLONDIKE_H

#include "server.h"
#include "card.h"

class Klondike {
public:


    Klondike(std::array<Card, 52> cards);

    std::vector<std::array<int, 4>> findMoves();
    void makeMove(std::array<int, 4> move);
    void placeCards(std::array<int, 4> move, std::vector<Card> cardsToMove);
    bool isWon();

    void printGame(bool hideFaceDown);

private:

    bool won;
    std::vector<Card> stock;
    std::array<std::vector<Card>, 7> tableau;
    std::array<std::stack<Card>, 4> foundation;

    bool evalMove(Card dest, Card pot);
};

#endif
