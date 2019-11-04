#ifndef HEURISTIC_H
#define HEURISTIC_H
#include "klondike.h"

const int SAFE_FOUNDATION_SCORE = 1000;
const int REVEAL_HIDDEN_SCORE = 100;
const int PLAN_REVEAL_HIDDEN_SCORE = 50;
const int EMPTY_SPACE_NO_KING_SCORE = -1;

class Heuristic {
public:

    std::function<bool(Klondike, Move)> getFcn();
    Heuristic(std::function<bool(Klondike, Move)> fcn, int score);

    friend bool operator> (Heuristic h1, Heuristic h2);

private:
    std::function<bool(Klondike, Move)> fcn;
    int score;
};

#endif
