#ifndef KLONDIKE_SOLVER_H
#define KLONDIKE_SOLVER_H

#include <iostream>
#include <ios>
#include <sstream>
#include <fstream>
#include <stdlib.h>
#include <time.h>
#include <array>
#include <vector>
#include <stack>
#include <tuple>
#include <chrono>
#include <unistd.h>
#include "../rapidjson/document.h"
#include "../rapidjson/istreamwrapper.h"
#include "klondike.h"
#include "solverCompare.h"
#include "solver.h"

std::vector<Solver> setSolvers(std::vector<std::string> hFiles);
void findHeuristicValue(std::vector<Heuristic>* heuristics, const rapidjson::Value* obj, HeuristicType type, std::string name);
SolverCompare runGames(SolverCompare comp, int deal, int seed, int games);

#endif
