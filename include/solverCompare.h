#ifndef SOLV_COMP_H
#define SOLV_COMP_H

#include "klondike.h"
#include "solver.h"
#include <sstream>
#include <future>

class SolverCompare {
public:

    bool runSolvers(Klondike game, int seed);

    SolverCompare(std::vector<Solver> solvers);

    std::string toString();

    std::vector<std::vector<int>> getWinComp();

private:

    std::vector<Solver> solvers;
    std::vector<std::vector<int>> winComp;
    int gamesPlayed;
};

#endif
