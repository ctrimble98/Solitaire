#ifndef SERVER_H
#define SERVER_H

#include <iostream>
#include <ios>
#include <sstream>
#include <stdlib.h>
#include <time.h>
#include <array>
#include <vector>
#include <stack>
#include <tuple>
#include <chrono>
#include <unistd.h>
#include "klondike.h"
#include "solverCompare.h"
#include "solver.h"
#include "verifier.h"

std::vector<Solver> setSolvers(std::string hFileName);
SolverCompare runGames(SolverCompare comp, int deal, int seed, int games, bool verify, Verifier verifier);

#endif
