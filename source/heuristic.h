#ifndef HEURISTIC_H
#define HEURISTIC_H
#include "klondike.h"
#include <functional>
#include <limits>

const int SAFE_FOUNDATION_SCORE = 1000;
const int REVEAL_HIDDEN_SCORE = 100;
const int PLAN_REVEAL_HIDDEN_SCORE = 50;
const int EMPTY_SPACE_NO_KING_SCORE = -1;
const int NOT_SATISFIED_SCORE = std::numeric_limits<int>::min();

enum HeuristicType {
    SAFE_FOUNDATION,
    REVEAL_HIDDEN,
    PLAN_REVEAL_HIDDEN,
    EMPTY_SPACE_NO_KING
};

class Heuristic {
public:

    std::function<int(Klondike, Move)> getFcn();
    int getScore();
    Heuristic(HeuristicType type, int score);

    friend bool operator> (Heuristic h1, Heuristic h2);

    int safeFoundationHeur(Klondike game, Move move);
    int revealHiddenHeur(Klondike game, Move move);
    int planRevealHiddenHeur(Klondike game, Move move);
    int emptyNoKingHeur(Klondike game, Move move);

private:
    std::function<bool(Klondike, Move)> fcn;
    int score;
};

std::array<int, 2> getFoudationMin(Klondike game);
int checkFutureHidden(Klondike game, Move move);
bool checkNothingMove(Klondike game, Move move);

#endif
