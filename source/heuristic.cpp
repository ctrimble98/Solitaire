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
        case STOCK_SAFE:
            fcn = scoreSafeStockMove;
            break;
        case STOCK_DISTANCE:
            fcn = scoreStockDistanceMove;
            break;
        case SMOOTH:
            fcn = scoreSmoothMove;
            break;
        case TABLEAU:
            fcn = scoreTableauMove;
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
    if (move.getStart()[0] == static_cast<int>(CardLocation::STOCK) && move.getEnd()[0] == static_cast<int>(CardLocation::TABLEAU) && move.getEnd()[2] > 0) {
        int tempScore = checkFutureStock(game, move);
        if (tempScore > 0) {
            return score - tempScore;
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

int scoreTableauMove(Klondike game, Move move, int score) {
    if (move.getStart()[0] == static_cast<int>(CardLocation::TABLEAU)) {
        return score;
    }
    return 0;
}

int scoreSafeStockMove(Klondike game, Move move, int score) {

    if (move.getStart()[0] == static_cast<int>(CardLocation::STOCK) && game.getStockPointer() + 1 % game.getDeal() == 0 && move.getStart()[2] == (int)game.getStock().size() - 1) {
        return score;
    }
    return 0;
}

int scoreStockDistanceMove(Klondike game, Move move, int score) {
    if (move.getStart()[0] == static_cast<int>(CardLocation::STOCK) && game.getStockPointer() + 1 % game.getDeal() == 0 && move.getStart()[2] != (int)game.getStock().size() - 1) {
        return score * move.getStart()[2]/(int)game.getStock().size();
    }
    return 0;
}

int scoreSmoothMove(Klondike game, Move move, int score) {
    if (move.getEnd()[0] == static_cast<int>(CardLocation::TABLEAU) && move.getEnd()[2] >= 2 && game.getTableau()[move.getEnd()[1]][move.getEnd()[2] - 2].getSuit() == move.getCard().getSuit()) {
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

int checkFutureStock(Klondike game, Move move) {
    if (move.getStart()[0] == static_cast<int>(CardLocation::STOCK) && move.getEnd()[0] == static_cast<int>(CardLocation::TABLEAU)) {
        int minDiff = 13;
        Card destCard = game.getTableau()[move.getEnd()[1]][move.getEnd()[2] - 1];
        for (int i = 0; i < STACKS; i++) {
            std::vector<Card> stack = game.getTableau()[i];
            if (stack.empty()) {
                continue;
            }
            int stackSize = stack.size();
            Card card = stack.back();
            std::vector<Card> v;
            if (stackSize >= 2 && stack[stackSize - 2].isFaceDown() && card.getRank() <= 11) {
                if (destCard.getRank() >= card.getRank() + 2) {
                    int diff = destCard.getRank() - card.getRank();
                    if (diff < minDiff) {
                        bool evenSplit = diff% 2 == 0;
                        bool sameColour = card.getColour() == destCard.getColour();
                        if (evenSplit == sameColour) {
                            minDiff = diff;
                        }
                    }
                }
            }
        }
        if (minDiff < 13) {
            bool foundAll = true;
            int target = 2;
            while (target < minDiff) {
                Colour targetColour = (target % 2 == 0) == (destCard.getColour() == Colour::RED) ? Colour::RED : Colour::BLACK;
                if (!findCardInStock(game, destCard.getRank() - target, targetColour)) {
                    foundAll = false;
                    break;
                }
                target++;
            }
            if (foundAll) {
                return minDiff;
            }
        }
    }
    return 0;
}

bool findCardInStock(Klondike game, int rank, Colour colour) {
    for (auto &card: game.getStock()) {
        if (card.getRank() == rank && card.getColour() == colour) {
            return true;
        }
    }
    return false;
}

std::array<int, 2> getFoudationMin(Klondike game) {
    std::array<std::stack<Card>, 4> foundation = game.getFoundation();
    std::array<int, 2> min = {CARDS_PER_SUIT, CARDS_PER_SUIT};
    int i = 0;
    while (i < 2) {
        if (foundation[i].empty()) {
            min[0] = 0;
        } else if (foundation[i].top().getRank() < min[0]) {
            min[0] = foundation[i].top().getRank();
        }
        i++;
    }

    while (i < 4) {
        if (foundation[i].empty()) {
            min[1] = 0;
        } else if (foundation[i].top().getRank() < min[1]) {
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

    if (move.getStart()[0] == static_cast<int>(CardLocation::TABLEAU) && move.getStart()[2] == 0) {

        //If the move is just shifting a king then it is a nothing move
        if (move.getCard().getRank() == 13) {
            if (move.getEnd()[0] == static_cast<int>(CardLocation::TABLEAU) && move.getEnd()[2] == 0) {
                return true;
            }
            return false;
        }

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
