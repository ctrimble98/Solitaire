#include <iostream>
#include <stdlib.h>
#include <array>
#include "card.h"

const int CARD_NO = 52;
const int STOCK_SIZE = 24;
const int STACKS = 7;

void shuffle(std::array<Card, CARD_NO> cards, int seed);
