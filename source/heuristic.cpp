#include "heuristic.h"

int Heuristic::run(Klondike game, Move move) {
    return fcn(game, move, score);
}

int Heuristic::getScore() {
    return score;
}

Heuristic::Heuristic(HeuristicType type, int score) : score(score) {
    switch (type) {
        case FOUNDATION:
            fcn = foundationHeur;
            break;
        case REVEAL_HIDDEN:
            fcn = revealHiddenHeur;
            break;
        case PLAN_REVEAL_HIDDEN:
            fcn = planRevealHiddenHeur;
            break;
        case EMPTY_SPACE_NO_KING:
            fcn = emptyNoKingHeur;
            break;
    }
}

bool operator> (Heuristic h1, Heuristic h2) {
    return h1.score > h2.score;
}

int foundationHeur(Klondike game, Move move, int score) {
    if (move.getEnd()[0] == static_cast<int>(CardLocation::FOUNDATION)) {
        if (getSafeFoundation(game, move)) {
            return SAFE_SCORE;
        }
        return score;
    } else {
        return 0;
    }
}

int revealHiddenHeur(Klondike game, Move move, int score) {
    if (move.getStart()[0] == static_cast<int>(CardLocation::TABLEAU) && move.getStart()[2] > 0 && game.getTableau()[move.getStart()[1]][move.getStart()[2] - 1].isFaceDown()) {
        return score + move.getStart()[2];
    } else {
        return 0;
    }
}

int planRevealHiddenHeur(Klondike game, Move move, int score) {
    if (move.getStart()[0] == static_cast<int>(CardLocation::STOCK) && move.getEnd()[0] == static_cast<int>(CardLocation::TABLEAU)) {
        int tempScore = checkFutureHidden(game, move);
        if (tempScore > 0) {
            return score + tempScore;
        }
    }
    return 0;
}

int emptyNoKingHeur(Klondike game, Move move, int score) {
    if (checkNothingMove(game, move)) {
        return score;
    } else {
        return 0;
    }
}

int scoreStockMove(Klondike game, Move move, int score) {

    if (game.getStockPointer() + 1 % game.getDeal() == 0 && move.getStart()[0] == static_cast<int>(CardLocation::STOCK) && move.getStart()[2] == game.getStock()[(int)game.getStock().size() - 1]) {
        return score;
    }
    return 0;
}

bool getSafeFoundation(Klondike game, Move move) {
    std::array<int, 2> minFoundation = getFoudationMin(game);
    if (move.getEnd()[0] == static_cast<int>(CardLocation::FOUNDATION) && ((move.getCard().getColour() == Colour::RED && move.getCard().getRank() <= minFoundation[1] + 2) || (move.getCard().getColour() == Colour::BLACK && move.getCard().getRank() <= minFoundation[0] + 2))) {
        return true;
    }
    return false;
}

int checkFutureStock(Klondike game, Move move, int score) {
    if (move.getStart()[0] == static_cast<int>(CardLocation::STOCK) && move.getEnd()[0] == static_cast<int>(CardLocation::TABLEAU)) {
        for (int i = 0; i < STACKS; i++) {
            std::vector<Card> stack = game.getTableau()[i];
            int stackSize = stack.getSize();
            Card card = stack.back();
            std::vector<card> v;
            if (stackSize >= 2 && stack[stackSize - 2].isFaceDown() && card.getRank() <= 11) {
                Card targetCard = game.getTableau()[move.getEnd()[1]][move.getEnd()[2]];
                if (targetCard.getRank() >= card.getRank() + 2) {
                    bool evenSplit = (card.getRank() - targetCard.getRank())% 2 == 0;
                    bool sameColour = card.getColour() == targetcard.getColour();
                    if (evenSplit == sameColour) {
                        //check stock for all cards in gap between hidden card and move end
                    }
                }
            }
        }
        return score;
    } else {
        return NOT_SATISFIED_SCORE;
    }
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

// bool checkStockMoves(Klondike game, int stockIndex) {
//     std::vector<int> availableStock = game.getAvailableStock(stockIndex);
//     for (int i = 0; i < availableStock.size(); i++) {
//         int stockIndex = availableStock[i];
//         moveStart = {static_cast<int>(CardLocation::STOCK), stockIndex, 0};
//         moveEnd = {static_cast<int>(CardLocation::TABLEAU), i, 0};
//         Move move(moveStart, moveEnd, stock[stockIndex]);
//         if (safeFoundationHeur(game, move))
//     }
// }
