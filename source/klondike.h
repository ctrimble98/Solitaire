#ifndef KLONDIKE_H
#define KLONDIKE_H

#include <iostream>
#include <fstream>
#include <string>
#include <stdlib.h>
#include <time.h>
#include <array>
#include <vector>
#include <stack>
#include <tuple>
#include <chrono>
#include "move.h"

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

class Klondike {
public:

    std::vector<Card> getStock();
    std::array<std::vector<Card>, 7> getTableau();
    std::array<std::stack<Card>, 4> getFoundation();
    bool isWon();

    Klondike(int seed);

    std::vector<Move> findMoves(bool allLegalMoves);
    void makeMove(Move move);
    void placeCards(Move move, std::vector<Card> cardsToMove);

    void printGame(bool hideFaceDown);
    void printJsonToFile(bool hideFaceDown, std::string file);

private:

    bool won;
    std::vector<Card> stock;
    std::array<std::vector<Card>, 7> tableau;
    std::array<std::stack<Card>, 4> foundation;

    bool evalMove(Card dest, Card pot);
};

#endif
