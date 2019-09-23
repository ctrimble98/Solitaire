#include "klondike.h"


Klondike::Klondike(std::array<Card, CARD_NO> cards) {

    for (size_t i = 0; i < STOCK_SIZE; i++) {
        stock.push_back(cards[i]);
    }

    for (size_t i = 0; i < STACKS; i++) {
        for (size_t j = 0; j <= i; j++) {
            tableau[i].push_back(cards[STOCK_SIZE + i + j]);
            if (j < i) {
                tableau[i][j].turnFaceDown();
            }
        }
    }
}

void Klondike::printGame() {

    std::cout << "Stock: [ ";
    for (auto const &card: stock) {
        std::cout << card.toString() << ' ';
    }
    std::cout << "]" << std::endl;

    std::cout << "Tableau: [ " << std::endl;

    for (auto const &stack: tableau) {
        std::cout << "\t[ ";
        for (auto const &card: stack) {
            std::cout << card.toString() << ' ';
        }
        std::cout << ']' << std::endl;
    }
    std::cout << "]" << std::endl;

    std::cout << "Foundation: [ ";
    for (auto &topCard: foundation) {
        if (!topCard.empty()) {
            std::cout << topCard.top().toString() << ' ';
        }
        else {
            std::cout << "X ";
        }
    }
    std::cout << "]" << std::endl;
}

void Klondike::findMoves() {

    std::vector<std::vector<Card>> moves;
    Card dest;
    int i = 0;
    for (auto const &stack: tableau) {
        moves.push_back(std::vector<Card>());
        dest = stack.back();
        for (auto const &stackMove: tableau) {
            if (evalMove(dest, stackMove.back())) {
                moves[i].push_back(stackMove.back());
            }
        }

        for (auto const &card: stock) {
            if (evalMove(dest, card)) {
                moves[i].push_back(card);
            }
        }

        i++;
    }
    for (int j = 0; j < 4; j++) {

        moves.push_back(std::vector<Card>());
        int target = 1;
        if (!foundation[j].empty()) {
            target = foundation[j].top().getRank() + 1;
        }
        for (auto const &stackMove: tableau) {
            if (static_cast<int>(stackMove.back().getSuit()) == j && stackMove.back().getRank() == target) {
                moves[i].push_back(stackMove.back());
            }
        }

        for (auto const &card: stock) {
            if (static_cast<int>(card.getSuit()) == j && card.getRank() == target) {
                moves[i].push_back(card);
            }
        }

        i++;
    }

    for (auto const &validMoves: moves) {
        for (auto const &card: validMoves) {
            std::cout << card.toString() << ' ';
        }
        std::cout << validMoves.size() << std::endl;
    }
}

bool Klondike::evalMove(Card dest, Card pot) {
    if (dest.getColour() != pot.getColour() && dest.getRank() == (pot.getRank() + 1)) return true;
    return false;
}
