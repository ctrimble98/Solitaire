#ifndef HEURISTIC_H
#define HEURISTIC_H
#include "klondike.h"
#include <functional>

const int SAFE_FOUNDATION_SCORE = 1000;
const int REVEAL_HIDDEN_SCORE = 100;
const int PLAN_REVEAL_HIDDEN_SCORE = 50;
const int EMPTY_SPACE_NO_KING_SCORE = -1;

class Heuristic {
public:

    std::function<bool(Klondike, Move)> getFcn();
    int getScore();
    Heuristic(std::function<bool(Klondike, Move)> fcn, int score);

    friend bool operator> (Heuristic h1, Heuristic h2);

private:
    std::function<bool(Klondike, Move)> fcn;
    int score;
};

bool safeFoundationHeur(Klondike game, Move move);
bool revealHiddenHeur(Klondike game, Move move);
bool planRevealHiddenHeur(Klondike game, Move move);
bool emptyNoKingHeur(Klondike game, Move move);

std::array<int, 2> getFoudationMin(Klondike game);
int checkFutureHidden(Klondike game, Move move);
bool checkNothingMove(Klondike game, Move move);

#endif
