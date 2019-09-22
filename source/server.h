#ifndef SERVER_H
#define SERVER_H

#include <iostream>
#include <stdlib.h>
#include <array>
#include <vector>
#include <stack>
#include "card.h"
#include "klondike.h"

const int CARD_NO = 52;
const int STOCK_SIZE = 24;
const int STACKS = 7;

void shuffle(std::array<Card, CARD_NO> cards, int seed);

#endif
