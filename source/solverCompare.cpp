#include "solverCompare.h"

SolverCompare::SolverCompare(std::vector<Solver> solvers): solvers(solvers), gamesPlayed(0) {
    int n = solvers.size();
    winComp = std::vector<std::vector<int>>(n, std::vector<int>(n, 0));
}

void SolverCompare::runSolvers(Klondike game) {

    int n = solvers.size();
    std::vector<bool> wins(n, false);
    for (int i = 0; i < n; i++) {
        if (solvers[i].run(game)) {
            wins[i] = true;
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
    // std::cout << winCompString << '\n';
    return winCompString.str();
}
