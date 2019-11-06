#include "heuristic.h"

std::function<bool(Klondike, Move)> Heuristic::getFcn() {
    return fcn;
}

int Heuristic::getScore() {
    return score;
}

Heuristic::Heuristic(std::function<bool(Klondike, Move)> fcn, int score) : fcn(fcn), score(score) {
}

bool operator> (Heuristic h1, Heuristic h2) {
    return h1.score > h2.score;
}

bool safeFoundationHeur(Klondike game, Move move) {
    std::array<int, 2> minFoundation = getFoudationMin(game);
    return move.getEnd()[0] == static_cast<int>(CardLocation::FOUNDATION) && ((move.getCard().getColour() == Colour::RED && move.getCard().getRank() <= minFoundation[1] + 2) || (move.getCard().getColour() == Colour::BLACK && move.getCard().getRank() <= minFoundation[0] + 2));
}

bool revealHiddenHeur(Klondike game, Move move) {
    return move.getStart()[0] == static_cast<int>(CardLocation::TABLEAU) && move.getStart()[2] > 0 && game.getTableau()[move.getStart()[1]][move.getStart()[2] - 1].isFaceDown();
}

bool planRevealHiddenHeur(Klondike game, Move move) {
    moves[j].getStart()[0] != static_cast<int>(CardLocation::TABLEAU) && moves[j].getEnd()[0] == static_cast<int>(CardLocation::TABLEAU)) {
    int moveScore = checkFutureHidden(game, moves[j]);
}

bool emptyNoKingHeur(Klondike game, Move move) {

}

std::array<int, 2> getFoudationMin(Klondike game) {
    std::array<std::stack<Card>, 4> foundation = game.getFoundation();
    std::array<int, 2> min = {0,0};
    int i = 0;
    while (i < 2) {
        if (!foundation[i].empty() && foundation[i].top().getRank() < min[0]) {
            min[0] = foundation[i].top().getRank();
        }
        i++;
    }

    while (i < 4) {
        if (!foundation[i].empty() && foundation[i].top().getRank() < min[1]) {
            min[1] = foundation[i].top().getRank();
        }
        i++;
    }

    return min;
}

int checkFutureHidden(Klondike game, Move move) {
    for (int i = 0; i < STACKS; i++) {
        if (game.getTableau()[i].size() >= 2 && game.getTableau()[i].back().getRank() == move.getCard().getRank() - 1 && game.getTableau()[i].back().getColour() != move.getCard().getColour() && game.getTableau()[i][game.getTableau()[i].size() - 2].isFaceDown()) {
            return game.getTableau()[i].size();
        }
    }
    return 0;
}

bool checkNothingMove(Klondike game, Move move) {

    if (move.getStart()[0] == static_cast<int>(CardLocation::TABLEAU) && move.getStart()[2] == 0 && move.getCard().getRank() != 13) {
        int n = game.getStock().size();
        for (int i = 0; i < n; i++) {
            if (game.getStock()[i].getRank() == 13) {
                return false;
            }
        }
        for (int i = 0; i < STACKS; i++) {
            int j = game.getTableau()[i].size() - 1;
            while (j >= 0 && !game.getTableau()[i][j].isFaceDown()) {
                j--;
            }
            j++;
            if (j > 0 && game.getTableau()[i][j].getRank() == 13) {
                return false;
            }
        }
        return true;
    }
    return false;
}
