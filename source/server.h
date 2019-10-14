#ifndef SERVER_H
#define SERVER_H

#include <iostream>
#include <sstream>
#include <iterator>
#include <stdlib.h>
#include <time.h>
#include <array>
#include <vector>
#include <stack>
#include <tuple>
#include <chrono>
#include "klondike.h"

bool randomSolve(Klondike game);
bool weightedSolve(Klondike game);
std::array<int, 2> getFoudationMin(Klondike game);

#endif
