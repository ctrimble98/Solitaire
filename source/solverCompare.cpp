#include "solverCompare.h"

SolverCompare::SolverCompare(std::vector<Solver> solvers): solvers(solvers), gamesPlayed(0) {
    int n = solvers.size();
    winComp = std::vector<std::vector<int>>(n, std::vector<int>(n, 0));
}

bool SolverCompare::runSolvers(Klondike game, int seed) {

    int n = solvers.size();
    std::vector<bool> wins(n, false);
    std::vector<std::future<bool>> threads;
    bool anyWins = false;

    for (int i = 0; i < n; i++) {
        threads.push_back(std::async(std::launch::async, &Solver::run, &solvers[i], game, seed));
    }

    for (int i = 0; i < n; i++) {
        if (threads[i].get()) {
            wins[i] = true;
            anyWins = true;
        }
    }

    for (int i = 0; i < n; i++) {
        for (int j = 0; j < n; j++) {
            if (i == j && wins[i]) {
                winComp[i][j]++;
            } else if (wins[i] && !wins[j]) {
                winComp[i][j]++;
            }
        }
    }
    gamesPlayed++;
    return anyWins;
}

std::string SolverCompare::toString() {

    int n = solvers.size();
    std::ostringstream winCompString;
    winCompString << gamesPlayed << " games played\n";
    winCompString << ",\t";
    for (int i = 0; i < n; i++) {
        winCompString << solvers[i].getName();
        if (i < n - 1) {
            winCompString << ",\t";
        }
    }
    winCompString << "\n";

    for (int i = 0; i < n; i++) {
        winCompString << solvers[i].getName() << ",\t";
        for (int j = 0; j < n; j++) {
            winCompString << 100*(float)winComp[i][j]/gamesPlayed;
            if (j < n - 1) {
                winCompString << ",\t";
            }
        }
        winCompString << "\n";
    }
    return winCompString.str();
}

std::vector<std::vector<int>> SolverCompare::getWinComp() {
    return winComp;
}
