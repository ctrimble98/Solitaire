#include "klondike.h"

std::vector<Card> Klondike::getStock() {
    return stock;
}

std::array<std::vector<Card>, 7> Klondike::getTableau() {
    return tableau;
}
std::array<std::stack<Card>, 4> Klondike::getFoundation() {
    return foundation;
}

bool Klondike::isWon() {
    return won;
}

bool Klondike::hasHiddenCards() {
    for (auto &stack: tableau) {
        if (stack.size() > 0 && stack[0].isFaceDown()) {
            return true;
        }
    }
    return false;
}

int Klondike::getDeal() {
    return deal;
}

int Klondike::getStockPointer() {
    return stockPointer;
}

std::vector<Move> Klondike::getMoves(bool allLegalMoves) {
    if (!allLegalMoves) {
        return availableMoves;
    } else {
        availableMoves.insert(std::end(availableMoves), std::begin(extraMoves), std::end(extraMoves));
    }
}

// Function to set up a game of Klondike
Klondike::Klondike(int seed, int deal) : deal(deal) {

    std::array<Card, CARD_NO> cards;
    for (int i = 0; i < CARD_NO; i++) {
        cards[i] = Card(i);
    }
    cards = shuffle(cards, seed);

    for (int i = 0; i < STOCK_SIZE; i++) {
        stock.push_back(cards[i]);
    }
    stockPointer = deal - 1;

    int cardsPlaced = 0;
    for (int i = 0; i < STACKS; i++) {
        for (int j = 0; j <= i; j++) {
            tableau[i].push_back(cards[STOCK_SIZE + cardsPlaced]);
            if (j < i) {
                tableau[i][j].turnFaceDown();
            }
            cardsPlaced++;
        }
    }
    won = false;
    visitedStates.insert(hashGame());

    findMoves();
}

void Klondike::printGame(bool hideFaceDown) {

    printStock(hideFaceDown);
    printTableau(hideFaceDown);
    printFoundation(hideFaceDown);
}

void Klondike::printStock(bool hideFaceDown) {

    std::cout << "Stock Pointer: " << stockPointer << " = " << stock[stockPointer].toString(false) << std::endl;
    std::cout << "Stock Size: " << stock.size() << std::endl;

    std::cout << "Stock: [ ";

    if (!stock.empty()) {
        std::vector<bool> available(stock.size(), false);
        std::vector<int>  availableStock = getAvailableStock(stockPointer);
        for (auto const &i: availableStock) {
            available[i] = true;
        }
        for (size_t i = 0; i < stock.size(); i++) {
            if (available[i]) {
                std::cout << '!';
            }
            std::cout << stock[i].toString(hideFaceDown) << ' ';
        }
    }
    std::cout << "]" << std::endl;
}
void Klondike::printTableau(bool hideFaceDown) {

    std::cout << "Tableau: [ " << std::endl;

    for (auto const &stack: tableau) {
        std::cout << "\t[ ";
        for (auto const &card: stack) {
            std::cout << card.toString(hideFaceDown) << ' ';
        }
        std::cout << ']' << std::endl;
    }
    std::cout << "]" << std::endl;
}

void Klondike::printFoundation(bool hideFaceDown) {

    std::cout << "Foundation: [ ";
    for (auto &topCard: foundation) {
        if (!topCard.empty()) {
            std::cout << topCard.top().toString(hideFaceDown) << ' ';
        }
        else {
            std::cout << "X ";
        }
    }
    std::cout << "]" << std::endl;
}

void Klondike::printJsonToFile(bool hideFaceDown, std::string file) {

    std::ofstream out(file);
    std::streambuf *coutbuf = std::cout.rdbuf();
    std::cout.rdbuf(out.rdbuf());

    std::string jsonGame;

    jsonGame.append("{\n");

    jsonGame.append("\"tableau piles\":[");

    for (auto const &stack: tableau) {
        jsonGame.append("\n[");
        for (auto const &card: stack) {
            jsonGame.append("\"");
            jsonGame.append(card.toString(hideFaceDown));
            jsonGame.append("\",");
        }
        jsonGame.pop_back();
        jsonGame.append("],");
    }
    jsonGame.pop_back();
    jsonGame.append("\n],\n");

    jsonGame.append("\"stock\": [");
    for (auto const &card: stock) {
        jsonGame.append("\"");
        jsonGame.append(card.toString(hideFaceDown));
        jsonGame.append("\",");
    }
    jsonGame.pop_back();
    jsonGame.append("]\n");

    jsonGame.append("}\n");

    std::cout << jsonGame;

    std::cout.rdbuf(coutbuf);
}

void Klondike::findMoves() {

    availableMoves.clear();
    extraMoves.clear();

    std::vector<std::tuple<Card, int, int>> tableauMovableCards;

    for (int i = 0; i < STACKS; i++) {
        int j = tableau[i].size() - 1;
        while (j >= 0 && !tableau[i][j].isFaceDown()) {
            tableauMovableCards.push_back(std::tuple<Card, int, int>(tableau[i][j], i, j));
            j--;
        }
    }

    std::vector<int> availableStock;
    if (!stock.empty()) {
        availableStock = getAvailableStock(stockPointer);
    }

    Card dest;
    int i = 0;
    std::array<int, 3> moveStart;
    std::array<int, 3> moveEnd;

    findTableauMoves(tableauMovableCards, availableStock);
    findFoundationMoves(tableauMovableCards, availableStock);

    auto iter = availableMoves.begin();
    while (iter != availableMoves.end()) {
        if (!Klondike(*this).makeMove(iter - availableMoves.begin(), false)) {
            extraMoves.push_back(*iter);
            iter = availableMoves.erase(iter);
        } else {
            iter++;
        }
    }

    if (availableMoves.size() == 0) {
        won = true;
        for (auto &topCard: foundation) {
            if (topCard.empty() || topCard.top().getRank() != 13) {
                won = false;
            }
        }
    }
}

void Klondike::findTableauMoves(std::vector<std::tuple<Card, int, int>> tableauMovableCards, std::vector<int> availableStock) {

    Card dest;
    int i = 0;
    std::array<int, 3> moveStart;
    std::array<int, 3> moveEnd;

    for (auto const &stack: tableau) {

        if (stack.empty()) {

            for (auto const &card: tableauMovableCards) {
                if (std::get<0>(card).getRank() == HIGH_CARD_RANK) {
                    moveStart = {static_cast<int>(CardLocation::TABLEAU), std::get<1>(card), std::get<2>(card)};
                    moveEnd = {static_cast<int>(CardLocation::TABLEAU), i, 0};
                    if (std::get<2>(card) != 0) {
                        availableMoves.push_back(Move(moveStart, moveEnd, std::get<0>(card)));
                    } else {
                        extraMoves.push_back(Move(moveStart, moveEnd, std::get<0>(card)));
                    }
                }
            }

            for (size_t j = 0; j < availableStock.size(); j++) {
                int stockIndex = availableStock[j];
                if (stock[stockIndex].getRank() == HIGH_CARD_RANK) {
                    moveStart = {static_cast<int>(CardLocation::STOCK), 0, stockIndex};
                    moveEnd = {static_cast<int>(CardLocation::TABLEAU), i, 0};
                    availableMoves.push_back(Move(moveStart, moveEnd, stock[stockIndex]));
                }
            }

            for (int j = 0; j < 4; j++) {
                if (!foundation[j].empty() && foundation[j].top().getRank() == HIGH_CARD_RANK) {
                    moveStart = {static_cast<int>(CardLocation::FOUNDATION), 0, j};
                    moveEnd = {static_cast<int>(CardLocation::TABLEAU), i, 0};
                    extraMoves.push_back(Move(moveStart, moveEnd, foundation[j].top()));
                }
            }
        }
        else {
            dest = stack.back();

            for (auto const &card: tableauMovableCards) {
                if (evalMove(dest, std::get<0>(card))) {
                    moveStart = {static_cast<int>(CardLocation::TABLEAU), std::get<1>(card), std::get<2>(card)};
                    moveEnd = {static_cast<int>(CardLocation::TABLEAU), i, (int)stack.size() - 1};
                    availableMoves.push_back(Move(moveStart, moveEnd, std::get<0>(card)));
                }
            }

            for (size_t j = 0; j < availableStock.size(); j++) {
                int stockIndex = availableStock[j];
                if (evalMove(dest, stock[stockIndex])) {
                    moveStart = {static_cast<int>(CardLocation::STOCK), 0, stockIndex};
                    moveEnd = {static_cast<int>(CardLocation::TABLEAU), i, (int)stack.size() - 1};
                    availableMoves.push_back(Move(moveStart, moveEnd, stock[stockIndex]));
                }
            }

            for (int j = 0; j < 4; j++) {
                if (!foundation[j].empty() && evalMove(dest, foundation[j].top())) {
                    moveStart = {static_cast<int>(CardLocation::FOUNDATION), 0, j};
                    moveEnd = {static_cast<int>(CardLocation::TABLEAU), i, (int)stack.size() - 1};
                    extraMoves.push_back(Move(moveStart, moveEnd, foundation[j].top()));
                }
            }
        }
        i++;
    }
}

void Klondike::findFoundationMoves(std::vector<std::tuple<Card, int, int>> tableauMovableCards, std::vector<int> availableStock) {

    Card dest;
    int i = 0;
    std::array<int, 3> moveStart;
    std::array<int, 3> moveEnd;

    for (int i = 0; i < 4; i++) {

        int target = 1;
        if (!foundation[i].empty()) {
            target = foundation[i].top().getRank() + 1;
        }
        for (int j = 0; j < STACKS; j++) {
            if (tableau[j].size() > 0 && static_cast<int>(tableau[j].back().getSuit()) == i && tableau[j].back().getRank() == target) {
                moveStart = {static_cast<int>(CardLocation::TABLEAU), j, (int)tableau[j].size() - 1};
                moveEnd = {static_cast<int>(CardLocation::FOUNDATION), 0, i};
                availableMoves.push_back(Move(moveStart, moveEnd, tableau[j].back()));
            }
        }

        for (size_t j = 0; j < availableStock.size(); j++) {
            int stockIndex = availableStock[j];
            if (static_cast<int>(stock[stockIndex].getSuit()) == i && stock[stockIndex].getRank() == target) {
                moveStart = {static_cast<int>(CardLocation::STOCK), 0, stockIndex};
                moveEnd = {static_cast<int>(CardLocation::FOUNDATION), 0, i};
                availableMoves.push_back(Move(moveStart, moveEnd, stock[stockIndex]));
            }
        }
    }
}

bool Klondike::evalMove(Card dest, Card pot) {
    if (dest.getColour() != pot.getColour() && dest.getRank() == (pot.getRank() + 1)) return true;
    return false;
}

bool Klondike::makeMove(int moveIndex, bool findNextMoves /*=true*/) {
    if (moveIndex < 0 || moveIndex > availableMoves.size() + extraMoves.size()) {
        std::cout << "Invalid move Index" << '\n';
        return false;
    }

    // Move move = availableMoves[0];
    // if (moveIndex < availableMoves.size()) {
    //     move = availableMoves[moveIndex];
    // } else {
    //     move = extraMoves[moveIndex - availableMoves.size()];
    // }
    Move move = moveIndex < availableMoves.size() ? availableMoves[moveIndex] : extraMoves[moveIndex - availableMoves.size()];

    std::vector<Card> cardsToMove;
    switch (move.getStart()[0]) {
        case static_cast<int>(CardLocation::STOCK): {
            int stockIndex = move.getStart()[2];
            cardsToMove.push_back(stock[stockIndex]);
            stock.erase(stock.begin() + stockIndex);
            if (stockIndex > 0) {
                stockPointer = stockIndex - 1;
            } else if (stock.size() >= (unsigned)deal) {
                stockPointer = deal - 1;
            } else {
                stockPointer = stock.size() - 1;
            }
            break;
        }
        case static_cast<int>(CardLocation::FOUNDATION):
            cardsToMove.push_back(foundation[move.getStart()[2]].top());
            foundation[move.getStart()[2]].pop();
            break;
        default:
            int tableauIndex = move.getStart()[1];
            if (move.getStart()[2] > 0 && tableau[tableauIndex][move.getStart()[2] - 1].isFaceDown()) {
                tableau[tableauIndex][move.getStart()[2] - 1].turnFaceUp();
            }
            for (size_t i = move.getStart()[2]; i < tableau[tableauIndex].size(); i++) {
                cardsToMove.push_back(tableau[tableauIndex][i]);
            }
            tableau[tableauIndex].erase(tableau[tableauIndex].begin() + move.getStart()[2], tableau[tableauIndex].end());
    }
    placeCards(move, cardsToMove);

    int hash = hashGame();
    bool notVisited = true;
    if (visitedStates.find(hash) == visitedStates.end()) {
        visitedStates.insert(hashGame());
    } else {
        notVisited = false;
    }

    if (findNextMoves) {
        findMoves();
    }

    return notVisited;
}

void Klondike::placeCards(Move move, std::vector<Card> cardsToMove) {

    switch (move.getEnd()[0]) {
        case static_cast<int>(CardLocation::FOUNDATION):
            foundation[move.getEnd()[2]].push(cardsToMove[0]);
            break;
        default:
            for (auto const &card: cardsToMove) {
                tableau[move.getEnd()[1]].push_back(card);
            }
    }
}

std::vector<int> Klondike::getAvailableStock(int stockPointer) {
    int n = stock.size();
    int i = stockPointer;
    bool foundDup = false;
    bool reachedEnd = false;
    bool atEnd = false;
    int seenEnd = 0;
    std::vector<int> availableCards;
    std::vector<bool> occurences(n, false);

    while (occurences[i] != true) {
        occurences[i] = true;
        if (i == n - 1) {
            i = -1;
        }
        i += deal;
        if (i > n - 1) {
            i = n - 1;
        }
    }

    for (size_t i = 0; i < n; i++) {
        if (occurences[i] == true) {
            availableCards.push_back(i);
        }
    }

    return availableCards;
}

int Klondike::hashGame() {

    int result = 1;
    int prime = 31;

    result = prime * result + stockPointer;

    for (auto &card: stock) {
        result = prime * result + card.getUniqueValue();
    }

    for (int i = 0; i < STACKS; i++) {
        for (auto &card: tableau[i]) {
            result = prime * result + card.getUniqueValue() + (104*i);
        }
    }

    for (int i = 0; i < 4; i++) {
        result = prime * result + foundation.size() + (13*i);
    }

    return result;
}
