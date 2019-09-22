#ifndef KLONDIKE_H
#define KLONDIKE_H

#include "server.h"
#include "card.h"

class Klondike {
public:


    Klondike(std::array<Card, 52> cards);

    void printGame();

private:

    std::vector<Card> stock;
    std::array<std::vector<Card>, 7> tableau;
    std::array<std::stack<Card>, 4> foundation;
};

#endif
