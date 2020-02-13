#ifndef MOVE_H
#define MOVE_H

#include <array>
#include "card.h"

class Move {

public:

    std::array<int, 3> getStart();
    std::array<int, 3> getEnd();
    Card getCard();

    Move(Move move);
    Move(std::array<int, 3> start, std::array<int, 3> end, Card card);

    void printMove();

private:

    std::array<int, 3> start;
    std::array<int, 3> end;
    Card card;
};

#endif
