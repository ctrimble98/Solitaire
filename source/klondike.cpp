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

int Klondike::getDeal() {
    return deal;
}
int Klondike::getStockPointer() {
    return stockPointer;
}

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
}

void Klondike::printGame(bool hideFaceDown) {

    std::cout << "Stock Pointer: " << stockPointer << " = " << stock[stockPointer].toString(false) << std::endl;

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

    std::cout << "Tableau: [ " << std::endl;

    for (auto const &stack: tableau) {
        std::cout << "\t[ ";
        for (auto const &card: stack) {
            std::cout << card.toString(hideFaceDown) << ' ';
        }
        std::cout << ']' << std::endl;
    }
    std::cout << "]" << std::endl;

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

std::vector<Move> Klondike::findMoves(bool allLegalMoves) {

    std::vector<Move> moves;
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
        for (size_t i = 0; i < stock.size(); i++) {
            availableStock.push_back(i);
        }
        // availableStock = getAvailableStock(stockPointer);
    }

    Card dest;
    int i = 0;
    std::array<int, 3> moveStart;
    std::array<int, 3> moveEnd;

    for (auto const &stack: tableau) {

        if (stack.empty()) {

            for (auto const &card: tableauMovableCards) {
                if (std::get<0>(card).getRank() == HIGH_CARD_RANK && std::get<2>(card) != 0) {
                    moveStart = {static_cast<int>(CardLocation::TABLEAU), std::get<1>(card), std::get<2>(card)};
                    moveEnd = {static_cast<int>(CardLocation::TABLEAU), i, 0};
                    moves.push_back(Move(moveStart, moveEnd, std::get<0>(card)));
                }
            }

            if (allLegalMoves) {
                for (int j = 0; j < STACKS; j++) {
                    if (tableau[j].back().getRank() == HIGH_CARD_RANK) {
                        moveStart = {static_cast<int>(CardLocation::TABLEAU), j, (int)tableau[j].size() - 1};
                        moveEnd = {static_cast<int>(CardLocation::TABLEAU), i, 0};
                        moves.push_back(Move(moveStart, moveEnd, tableau[j].back()));
                    }
                }
            }

            for (size_t j = 0; j < availableStock.size(); j++) {
                int stockIndex = availableStock[j];
                if (stock[stockIndex].getRank() == HIGH_CARD_RANK) {
                    moveStart = {static_cast<int>(CardLocation::STOCK), 0, stockIndex};
                    moveEnd = {static_cast<int>(CardLocation::TABLEAU), i, 0};
                    moves.push_back(Move(moveStart, moveEnd, stock[stockIndex]));
                }
            }

            if (allLegalMoves) {
                for (int j = 0; j < 4; j++) {
                    if (!foundation[j].empty() && foundation[j].top().getRank() == HIGH_CARD_RANK) {
                        moveStart = {static_cast<int>(CardLocation::FOUNDATION), 0, j};
                        moveEnd = {static_cast<int>(CardLocation::TABLEAU), i, 0};
                        moves.push_back(Move(moveStart, moveEnd, foundation[j].top()));
                    }
                }
            }
        }
        else {
            dest = stack.back();

            for (auto const &card: tableauMovableCards) {
                if (evalMove(dest, std::get<0>(card))) {
                    moveStart = {static_cast<int>(CardLocation::TABLEAU), std::get<1>(card), std::get<2>(card)};
                    moveEnd = {static_cast<int>(CardLocation::TABLEAU), i, (int)stack.size() - 1};
                    moves.push_back(Move(moveStart, moveEnd, std::get<0>(card)));
                }
            }

            // for (int j = 0; j < STACKS; j++) {
            //     if (evalMove(dest, tableau[j].back())) {
            //         tempMove = {static_cast<int>(CardLocation::TABLEAU) + j, tableau[j].size() - 1, static_cast<int>(CardLocation::TABLEAU) + i, stack.size() - 1};
            //         moves.push_back(tempMove);
            //     }
            // }

            for (size_t j = 0; j < availableStock.size(); j++) {
                int stockIndex = availableStock[j];
                if (evalMove(dest, stock[stockIndex])) {
                    //tempMove = {static_cast<int>(CardLocation::STOCK), j, static_cast<int>(CardLocation::TABLEAU) + i, stack.size() - 1};
                    moveStart = {static_cast<int>(CardLocation::STOCK), 0, stockIndex};
                    moveEnd = {static_cast<int>(CardLocation::TABLEAU), i, (int)stack.size() - 1};
                    moves.push_back(Move(moveStart, moveEnd, stock[stockIndex]));
                }
            }

            if (allLegalMoves) {
                for (int j = 0; j < 4; j++) {
                    if (!foundation[j].empty() && evalMove(dest, foundation[j].top())) {
                        //tempMove = {static_cast<int>(CardLocation::FOUNDATION), j, static_cast<int>(CardLocation::TABLEAU) + i, 0};
                        moveStart = {static_cast<int>(CardLocation::FOUNDATION), 0, j};
                        moveEnd = {static_cast<int>(CardLocation::TABLEAU), i, (int)stack.size() - 1};
                        moves.push_back(Move(moveStart, moveEnd, foundation[j].top()));
                    }
                }
            }
        }
        i++;
    }

    for (int j = 0; j < 4; j++) {

        int target = 1;
        if (!foundation[j].empty()) {
            target = foundation[j].top().getRank() + 1;
        }
        for (int k = 0; k < STACKS; k++) {
            if (tableau[k].size() > 0 && static_cast<int>(tableau[k].back().getSuit()) == j && tableau[k].back().getRank() == target) {

                //tempMove = {static_cast<int>(CardLocation::TABLEAU) + k, tableau[k].size() - 1, static_cast<int>(CardLocation::FOUNDATION), j};
                moveStart = {static_cast<int>(CardLocation::TABLEAU), k, (int)tableau[k].size() - 1};
                moveEnd = {static_cast<int>(CardLocation::FOUNDATION), 0, j};
                moves.push_back(Move(moveStart, moveEnd, tableau[k].back()));
            }
        }

        for (size_t k = 0; k < availableStock.size(); k++) {
            int stockIndex = availableStock[k];
            if (static_cast<int>(stock[stockIndex].getSuit()) == j && stock[stockIndex].getRank() == target) {
                //tempMove = {static_cast<int>(CardLocation::STOCK), k, static_cast<int>(CardLocation::FOUNDATION), j};
                moveStart = {static_cast<int>(CardLocation::STOCK), 0, stockIndex};
                moveEnd = {static_cast<int>(CardLocation::FOUNDATION), 0, j};
                moves.push_back(Move(moveStart, moveEnd, stock[stockIndex]));
            }
        }

        i++;
    }

    if (moves.size() == 0) {
        won = true;
        for (auto &topCard: foundation) {
            if (topCard.empty() || topCard.top().getRank() != 13) {
                won = false;
            }
        }
    }

    return moves;
}

bool Klondike::evalMove(Card dest, Card pot) {
    if (dest.getColour() != pot.getColour() && dest.getRank() == (pot.getRank() + 1)) return true;
    return false;
}

void Klondike::makeMove(Move move) {
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
            //stock.erase(stock.begin() + move.getStart()[1]);
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
    availableCards.push_back(i);
    if (i == n - 1) {
        i = -1;
    }
    while (!foundDup) {
        if (atEnd) {
            if (n >= deal) {
                i = deal - 1;
            } else {
                i = n - 1;
            }
            atEnd = false;
        } else {
            i += deal;
            if (i >= n - 1) {
                i = n - 1;
                atEnd = true;
                seenEnd++;
            }
        }
        if (seenEnd > 1) {
            foundDup = true;
        } else {
            availableCards.push_back(i);
        }
    }
    return availableCards;
}
