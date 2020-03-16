#ifndef SOLVER_H
#define SOLVER_H

#include "heuristic.h"
#include <algorithm>

class Solver {
public:

    bool run(Klondike game, int seed);
    std::string getName();

    Solver(/*std::function<bool(Klondike)> solvFcn, */std::string name, std::vector<Heuristic> heuristics, std::function<bool(Klondike, std::vector<Move>)> searchFcn);

private:

    std::string name;
    std::function<bool(Klondike)> solvFcn;
    std::vector<Heuristic> heuristics;
    std::function<bool(Klondike, std::vector<Move>)> searchFcn;
};

bool checkSafeMove(Klondike game, Move move, bool print);

// bool dfs(Klondike game, Move move, int depth, int maxDepth, bool performedStockMove);
bool runSearchCheckStock(Klondike game, std::vector<Move> moves);
bool runSearchNoCheckStock(Klondike game, std::vector<Move> moves);
bool runSearchDFS(Klondike game, std::vector<Move> moves);

int dfs(Klondike game, Move move, int depth, int maxDepth, bool performedStockMove, int currentScore);

// bool randomSolve(Klondike game);
// bool weightedSolveAll(Klondike game);
// bool weightedSolve1(Klondike game);
// bool weightedSolve2(Klondike game);
// bool weightedSolve3(Klondike game);
// std::array<int, 2> getFoudationMin(Klondike game);
// int checkFutureHidden(Klondike game, Move move);
// bool checkNothingMove(Klondike game, Move move);

#endif
