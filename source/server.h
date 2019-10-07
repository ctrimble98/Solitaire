#ifndef SERVER_H
#define SERVER_H

#include <iostream>
#include <stdlib.h>
#include <time.h>
#include <array>
#include <vector>
#include <stack>
#include <tuple>
#include <chrono>
#include "card.h"
#include "klondike.h"

const int CARD_NO = 52;
const int STOCK_SIZE = 24;
const int STACKS = 7;
const int HIGH_CARD_RANK = 13;

enum class CardLocation {
    STOCK = 0,
    TABLEAU_START = 1,
    FOUNDATION = TABLEAU_START + STACKS
};

std::array<Card, CARD_NO> shuffle(std::array<Card, CARD_NO> cards, int seed);

bool randomSolve(Klondike game);
bool weightedSolve(Klondike game);
std::array<int, 2> getFoudationMin(Klondike game);

#endif
