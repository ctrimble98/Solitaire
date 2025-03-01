#ifndef HEURISTIC_H
#define HEURISTIC_H
#include "klondike.h"
#include <functional>
#include <limits>

const int SAFE_SCORE = 100;
const int FOUNDATION_SCORE = 0;
const int REVEAL_HIDDEN_SCORE = 20;
const int PLAN_REVEAL_HIDDEN_SCORE = 10;
const int EMPTY_SPACE_NO_KING_SCORE = -10;
const int STOCK_SAFE_SCORE = 0;
const int STOCK_MOD_DEAL_SCORE = 0;
const int STOCK_DISTANCE_SCORE = 0;
const int SMOOTH_SCORE = 0;
const int TABLEAU_SCORE = 0;
const int MIN_SCORE = std::numeric_limits<int>::min();

enum HeuristicType {
    FOUNDATION,
    REVEAL_HIDDEN,
    PLAN_REVEAL_HIDDEN,
    EMPTY_SPACE_NO_KING,
    STOCK_SAFE,
    STOCK_MOD_DEAL,
    STOCK_DISTANCE,
    SMOOTH,
    TABLEAU
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

bool getSafeFoundation(Klondike game, Move move);
std::array<int, 2> getFoudationMin(Klondike game);
int checkFutureHidden(Klondike game, Move move);
bool checkNothingMove(Klondike game, Move move);

int foundationHeur(Klondike game, Move move, int score);
int revealHiddenHeur(Klondike game, Move move, int score);
int planRevealHiddenHeur(Klondike game, Move move, int score);
int emptyNoKingHeur(Klondike game, Move move, int score);
int scoreTableauMove(Klondike game, Move move, int score);
int scoreSafeStockMove(Klondike game, Move move, int score);
int scoreStockDistanceMove(Klondike game, Move move, int score);
int scoreSmoothMove(Klondike game, Move move, int score);

int checkFutureStock(Klondike game, Move move);
bool findCardInStock(Klondike game, int rank, Colour colour);

#endif
