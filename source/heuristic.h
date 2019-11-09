#ifndef HEURISTIC_H
#define HEURISTIC_H
#include "klondike.h"
#include <functional>
#include <limits>

const int FOUNDATION_SCORE = 50;
const int REVEAL_HIDDEN_SCORE = 200;
const int PLAN_REVEAL_HIDDEN_SCORE = 100;
const int EMPTY_SPACE_NO_KING_SCORE = -1;
const int NOT_SATISFIED_SCORE = std::numeric_limits<int>::min();

enum HeuristicType {
    FOUNDATION,
    REVEAL_HIDDEN,
    PLAN_REVEAL_HIDDEN,
    EMPTY_SPACE_NO_KING
};

class Heuristic {
public:

    int run(Klondike game, Move move);
    int getScore();
    Heuristic(HeuristicType type, int score);

    friend bool operator> (Heuristic h1, Heuristic h2);

private:
    std::function<int(Klondike, Move, int)> fcn;
    int score;
};

std::array<int, 2> getFoudationMin(Klondike game);
int checkFutureHidden(Klondike game, Move move);
bool checkNothingMove(Klondike game, Move move);

int foundationHeur(Klondike game, Move move, int score);
int revealHiddenHeur(Klondike game, Move move, int score);
int planRevealHiddenHeur(Klondike game, Move move, int score);
int emptyNoKingHeur(Klondike game, Move move, int score);

#endif
