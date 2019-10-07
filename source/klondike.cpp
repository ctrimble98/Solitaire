#include "server.h"

std::vector<Card> Klondike::getStock() {
    return stock;
}

std::array<std::vector<Card>, 7> Klondike::getTableau() {
    return tableau;
}
std::array<std::stack<Card>, 4> Klondike::getFoundation() {
    return foundation;
}

Klondike::Klondike(int seed) {

    std::array<Card, CARD_NO> cards;
    for (size_t i = 0; i < CARD_NO; i++) {
        cards[i] = Card(i);
    }
    cards = shuffle(cards, seed);

    for (size_t i = 0; i < STOCK_SIZE; i++) {
        stock.push_back(cards[i]);
    }

    int cardsPlaced = 0;
    for (size_t i = 0; i < STACKS; i++) {
        for (size_t j = 0; j <= i; j++) {
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

    std::cout << "Stock: [ ";
    for (auto const &card: stock) {
        std::cout << card.toString(hideFaceDown) << ' ';
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

std::vector<std::array<int, 4>> Klondike::findMoves(bool allLegalMoves) {

    std::vector<std::array<int, 4>> moves;
    std::vector<std::tuple<Card, int, int>> tableauMovableCards;

    for (int i = 0; i < STACKS; i++) {
        int j = tableau[i].size() - 1;
        while (j >= 0 && !tableau[i][j].isFaceDown()) {
            tableauMovableCards.push_back(std::tuple<Card, int, int>(tableau[i][j], i, j));
            j--;
        }
    }

    Card dest;
    int i = 0;
    std::array<int, 4> tempMove;
    for (auto const &stack: tableau) {

        if (stack.empty()) {

            for (auto const &card: tableauMovableCards) {
                if (std::get<0>(card).getRank() == HIGH_CARD_RANK && std::get<2>(card) != 0) {
                    tempMove = {static_cast<int>(CardLocation::TABLEAU_START) + std::get<1>(card), std::get<2>(card), static_cast<int>(CardLocation::TABLEAU_START) + i, 0};
                    moves.push_back(tempMove);
                }
            }

            if (allLegalMoves) {
                for (int j = 0; j < STACKS; j++) {
                    if (tableau[j].back().getRank() == HIGH_CARD_RANK) {
                        tempMove = {static_cast<int>(CardLocation::TABLEAU_START) + j, tableau[j].size() - 1, static_cast<int>(CardLocation::TABLEAU_START) + i, 0};
                        moves.push_back(tempMove);
                    }
                }
            }

            for (int j = 0; j < stock.size(); j++) {
                if (stock[j].getRank() == HIGH_CARD_RANK) {
                    tempMove = {static_cast<int>(CardLocation::STOCK), j, static_cast<int>(CardLocation::TABLEAU_START) + i, 0};
                    moves.push_back(tempMove);
                }
            }

            if (allLegalMoves) {
                for (int j = 0; j < 4; j++) {
                    if (!foundation[j].empty() && foundation[j].top().getRank() == HIGH_CARD_RANK) {
                        tempMove = {static_cast<int>(CardLocation::FOUNDATION), j, static_cast<int>(CardLocation::TABLEAU_START) + i, 0};
                        moves.push_back(tempMove);
                    }
                }
            }
        }
        else {
            dest = stack.back();

            for (auto const &card: tableauMovableCards) {
                if (evalMove(dest, std::get<0>(card))) {
                    tempMove = {static_cast<int>(CardLocation::TABLEAU_START) + std::get<1>(card), std::get<2>(card), static_cast<int>(CardLocation::TABLEAU_START) + i, 0};
                    moves.push_back(tempMove);
                }
            }

            // for (int j = 0; j < STACKS; j++) {
            //     if (evalMove(dest, tableau[j].back())) {
            //         tempMove = {static_cast<int>(CardLocation::TABLEAU_START) + j, tableau[j].size() - 1, static_cast<int>(CardLocation::TABLEAU_START) + i, stack.size() - 1};
            //         moves.push_back(tempMove);
            //     }
            // }

            for (int j = 0; j < stock.size(); j++) {
                if (evalMove(dest, stock[j])) {
                    tempMove = {static_cast<int>(CardLocation::STOCK), j, static_cast<int>(CardLocation::TABLEAU_START) + i, stack.size() - 1};
                    moves.push_back(tempMove);
                }
            }

            if (allLegalMoves) {
                for (int j = 0; j < 4; j++) {
                    if (!foundation[j].empty() && evalMove(dest, foundation[j].top())) {
                        tempMove = {static_cast<int>(CardLocation::FOUNDATION), j, static_cast<int>(CardLocation::TABLEAU_START) + i, 0};
                        moves.push_back(tempMove);
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

                tempMove = {static_cast<int>(CardLocation::TABLEAU_START) + k, tableau[k].size() - 1, static_cast<int>(CardLocation::FOUNDATION), j};
                moves.push_back(tempMove);
            }
        }

        for (int k = 0; k < stock.size(); k++) {
            if (static_cast<int>(stock[k].getSuit()) == j && stock[k].getRank() == target) {
                tempMove = {static_cast<int>(CardLocation::STOCK), k, static_cast<int>(CardLocation::FOUNDATION), j};
                moves.push_back(tempMove);
            }
        }

        i++;
    }

    // for (auto const &move: moves) {
    //     std::cout << "(" << move[0] << ", " << move[1] << ")" << " -> " << "(" << move[2] << ", " << move[3] << ")" << std::endl;
    // }

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

void Klondike::makeMove(std::array<int, 4> move) {
    std::vector<Card> cardsToMove;
    switch (move[0]) {
        case static_cast<int>(CardLocation::STOCK):
            cardsToMove.push_back(stock[move[1]]);
            stock.erase(stock.begin() + move[1]);
            break;
        default:
            int tableauIndex = move[0] - static_cast<int>(CardLocation::TABLEAU_START);
            if (move[1] > 0 && tableau[tableauIndex][move[1] - 1].isFaceDown()) {
                tableau[tableauIndex][move[1] - 1].turnFaceUp();
            }
            for (int i = move[1]; i < tableau[tableauIndex].size(); i++) {
                cardsToMove.push_back(tableau[tableauIndex][i]);
            }
            tableau[tableauIndex].erase(tableau[tableauIndex].begin() + move[1], tableau[tableauIndex].end());
    }
    placeCards(move, cardsToMove);
}

void Klondike::placeCards(std::array<int, 4> move, std::vector<Card> cardsToMove) {

    switch (move[2]) {
        case static_cast<int>(CardLocation::FOUNDATION):
            foundation[move[3]].push(cardsToMove[0]);
            break;
        default:
            for (auto const &card: cardsToMove) {
                tableau[move[2] - static_cast<int>(CardLocation::TABLEAU_START)].push_back(card);
            }
    }
}

bool Klondike::isWon() {
    return won;
}
