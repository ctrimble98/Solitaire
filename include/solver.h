#ifndef SOLVER_H
#define SOLVER_H

#include "heuristic.h"
#include <algorithm>

class Solver {
public:

    bool run(Klondike game, int seed);
    std::string getName();
    std::function<bool(int, int)> getLimitFcn();
    void addNode();
    int getNodes();

    Solver(/*std::function<bool(Klondike)> solvFcn, */std::string name, std::vector<Heuristic> heuristics, std::function<bool(Solver*, Klondike, std::vector<Move>)> searchFcn, std::function<bool(int, int)> limitFcn);

private:

    std::string name;
    std::function<bool(Klondike)> solvFcn;
    std::vector<Heuristic> heuristics;
    std::function<bool(Solver*, Klondike, std::vector<Move>)> searchFcn;
    std::function<bool(int, int)> limitFcn;
    int nodes;
};

bool checkSafeMove(Klondike game, Move move, bool print);

// bool dfs(Klondike game, Move move, int depth, int maxDepth, bool performedStockMove);
bool runSearchCheckStock(Solver* solver, Klondike game, std::vector<Move> moves);
bool runSearchNoCheckStock(Solver* solver, Klondike game, std::vector<Move> moves);
bool runSearchDFS(Solver* solver, Klondike game, std::vector<Move> moves);

int dfs(Solver* solver, Klondike game, Move move, int depth, int maxDepth, bool performedStockMove, int currentScore);

bool depthLimit(int depth, int nodes);
bool nodeLimit(int depth, int nodes);

// bool randomSolve(Klondike game);
// bool weightedSolveAll(Klondike game);
// bool weightedSolve1(Klondike game);
// bool weightedSolve2(Klondike game);
// bool weightedSolve3(Klondike game);
// std::array<int, 2> getFoudationMin(Klondike game);
// int checkFutureHidden(Klondike game, Move move);
// bool checkNothingMove(Klondike game, Move move);

#endif
