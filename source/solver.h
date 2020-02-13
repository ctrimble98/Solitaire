#ifndef SOLVER_H
#define SOLVER_H

#include "heuristic.h"
#include <algorithm>

class Solver {
public:

    bool run(Klondike game, int seed);
    std::string getName();

    Solver(/*std::function<bool(Klondike)> solvFcn, */std::string name, std::vector<Heuristic> heuristics);

private:

    std::string name;
    std::function<bool(Klondike)> solvFcn;
    std::vector<Heuristic> heuristics;
};

bool checkSafeMove(Klondike game, Move move);

bool dfs(Klondike game, int depth);

// bool randomSolve(Klondike game);
// bool weightedSolveAll(Klondike game);
// bool weightedSolve1(Klondike game);
// bool weightedSolve2(Klondike game);
// bool weightedSolve3(Klondike game);
// std::array<int, 2> getFoudationMin(Klondike game);
// int checkFutureHidden(Klondike game, Move move);
// bool checkNothingMove(Klondike game, Move move);

#endif
