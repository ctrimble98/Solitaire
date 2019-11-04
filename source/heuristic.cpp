#include "heuristic.h"

std::function<bool(Klondike, Move)> Heuristic::getFcn() {
    return fcn;
}

Heuristic::(std::function<bool(Klondike, Move)> fcn, int score) : fcn(fcn), score(score) {
}

bool operator> (Heuristic h1, Heuristic h2) {
    return h1.score > h2.score;
}
