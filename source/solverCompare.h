#ifndef SOLV_COMP_H
#define SOLV_COMP_H

#include "klondike.h"
#include "solver.h"
#include <sstream>

class SolverCompare {
public:

    void runSolvers(Klondike game);

    SolverCompare(std::vector<Solver> solvers);

    std::string toString();

private:

    std::vector<Solver> solvers;
    std::vector<std::vector<int>> winComp;
    int gamesPlayed;
};

#endif
