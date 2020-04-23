#ifndef SOLVER_H
#define SOLVER_H

#include "heuristic.h"
#include <algorithm>

class Solver {
public:

    bool run(Klondike game, int seed, bool verbose);
    std::string getName();
    std::function<int(int, int)> getLimitFcn();
    std::function<int(Solver*, int)> getSearchValueFcn();
    void addNode();
    int getNodes();
    int getLimit();

    Solver(std::string name,
        std::vector<Heuristic> heuristics,
        std::function<int(Solver*, Klondike, std::vector<Move>)> searchFcn,
        std::function<int(Solver*, int)> searchValueFcn,
        int searchLimit);

private:

    std::string name;
    std::function<bool(Klondike)> solvFcn;
    std::vector<Heuristic> heuristics;
    std::function<int(Solver*, Klondike, std::vector<Move>)> searchFcn;
    std::function<bool(int, int)> limitFcn;
    std::function<int(Solver*, int)> searchValueFcn;
    int nodes;
    int limit;
};

bool checkSafeMove(Klondike game, Move move);

int runNoSearch(Solver* solver, Klondike game, std::vector<Move> moves);
int runSearchIDDFSCheckStock(Solver* solver, Klondike game, std::vector<Move> moves);
int runSearchIDDFSNoCheckStock(Solver* solver, Klondike game, std::vector<Move> moves);
int runSearchDFS(Solver* solver, Klondike game, std::vector<Move> moves);

int dfs(Solver* solver, Klondike game, Move move, int depth, int maxDepth, bool performedStockMove, int currentScore);

bool checkLimit(int value, int limit);

bool depthLimit(int depth, int nodes);
bool nodeLimit(int depth, int nodes);

int getNodes(Solver* solver, int depth);
int getDepth(Solver* solver, int depth);

#endif
