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
#include <random>
#include <algorithm>
#include <unordered_set>
#include "move.h"

const int CARD_NO = 52;
const int STOCK_SIZE = 24;
const int STACKS = 7;
const int HIGH_CARD_RANK = 13;

enum class CardLocation {
    STOCK = 0,
    TABLEAU = 1,
    FOUNDATION = 2
};

std::array<Card, CARD_NO> shuffle(std::array<Card, CARD_NO> cards, int seed);

class Klondike {
public:

    std::vector<Card> getStock();
    std::array<std::vector<Card>, 7> getTableau();
    std::array<std::stack<Card>, 4> getFoundation();
    int getDeal();
    int getStockPointer();
    bool hasHiddenCards();
    bool isWon();

    Klondike(int seed, int deal);

    std::vector<Move> findMoves(bool allLegalMoves);
    std::vector<Move> findTableauMoves(std::vector<Move> moves, std::vector<std::tuple<Card, int, int>> tableauMovableCards, std::vector<int> availableStock, bool allLegalMoves);
    std::vector<Move> findFoundationMoves(std::vector<Move> moves, std::vector<std::tuple<Card, int, int>> tableauMovableCards, std::vector<int> availableStock, bool allLegalMoves);

    bool makeMove(Move move);
    void placeCards(Move move, std::vector<Card> cardsToMove);
    std::vector<int> getAvailableStock(int stockPointer);

    void printGame(bool hideFaceDown);
    void printStock(bool hideFaceDown);
    void printTableau(bool hideFaceDown);
    void printFoundation(bool hideFaceDown);

    void printJsonToFile(bool hideFaceDown, std::string file);

    int hashGame();

private:

    bool won;
    std::vector<Card> stock;
    std::array<std::vector<Card>, 7> tableau;
    std::array<std::stack<Card>, 4> foundation;
    int deal;
    int stockPointer;

    std::unordered_set<int> visitedStates;

    bool evalMove(Card dest, Card pot);
};

#endif
