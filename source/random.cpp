#include "server.h"

bool randomSolve(Klondike game) {

    bool allLegalMoves = false;
    int maxMoves = 200;
    std::vector<Move> moves = game.findMoves(allLegalMoves);
    int i = 0;
    srand(time(NULL));
    while (!moves.empty() && i < maxMoves) {
        game.makeMove(moves[rand() % moves.size()]);
        moves = game.findMoves(allLegalMoves);
        i++;
    }
    return game.isWon();
}

bool weightedSolve(Klondike game) {

    bool allLegalMoves = false;
    int maxMoves = 200;
    std::vector<Move> moves = game.findMoves(allLegalMoves);
    int i = 0;
    srand(time(NULL));
    while (!moves.empty() && i < maxMoves) {

        int n = moves.size();
        int bestScore = -1;
        std::vector<Move> bestMoves = moves;
        std::array<int, 2> minFoundation = getFoudationMin(game);
        for (size_t j = 0; j < n; j++) {

            if (bestScore <= 100 && moves[j].getEnd()[0] == static_cast<int>(CardLocation::FOUNDATION) && ((moves[j].getCard().getColour() == Colour::RED && moves[j].getCard().getRank() <= minFoundation[1] + 2) || (moves[j].getCard().getColour() == Colour::BLACK && moves[j].getCard().getRank() <= minFoundation[0] + 2))) {
                //Check for safe moves to foundation
                if (bestScore < 100) {
                    bestScore = 100;
                    bestMoves = std::vector<Move>();
                }
                bestMoves.push_back(moves[j]);
            } else if (bestScore <= 20 + moves[j].getStart()[2] && moves[j].getStart()[0] == static_cast<int>(CardLocation::TABLEAU) && moves[j].getStart()[2] > 0 && game.getTableau()[moves[j].getStart()[1]][moves[j].getStart()[2] - 1].isFaceDown()) {
                //Check for moves that reveal hidden cards
                if (bestScore < 20 + moves[j].getStart()[2]) {
                    bestScore = 20 + moves[j].getStart()[2];
                    bestMoves = std::vector<Move>();
                }
                bestMoves.push_back(moves[j]);
            } else if (bestScore < 20 && moves[j].getStart()[0] != static_cast<int>(CardLocation::TABLEAU) && moves[j].getEnd()[0] == static_cast<int>(CardLocation::TABLEAU)) {
                //Check for moves that open up moves to reveal hidden cards
                int moveScore = checkFutureHidden(game, moves[j]);
                if (moveScore > 0) {
                    if (bestScore < 10 + moveScore) {
                        bestScore = 10 + moveScore;
                        bestMoves = std::vector<Move>();
                    }
                    bestMoves.push_back(moves[j]);
                }
            } else if (bestScore <= 0 && !checkNothingMove(game, moves[j])) {
                bestMoves.push_back(moves[j]);
            }
        }

        game.makeMove(bestMoves[rand() % bestMoves.size()]);
        moves = game.findMoves(allLegalMoves);
        i++;
    }
    return game.isWon();
}

std::array<int, 2> getFoudationMin(Klondike game) {
    std::array<std::stack<Card>, 4> foundation = game.getFoundation();
    std::array<int, 2> min = {0,0};
    size_t i = 0;
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
    for (size_t i = 0; i < STACKS; i++) {
        if (game.getTableau()[i].size() >= 2 && game.getTableau()[i].back().getRank() == move.getCard().getRank() - 1 && game.getTableau()[i].back().getColour() != move.getCard().getColour() && game.getTableau()[i][game.getTableau()[i].size() - 2].isFaceDown()) {
            return game.getTableau()[i].size();
        }
    }
    return 0;
}

bool checkNothingMove(Klondike game, Move move) {

    if (move.getStart()[0] == static_cast<int>(CardLocation::TABLEAU) && move.getStart()[2] == 0 && move.getCard().getRank() != 13) {
        int n = game.getStock().size();
        for (size_t i = 0; i < n; i++) {
            if (game.getStock()[i].getRank() == 13) {
                return false;
            }
        }
        for (size_t i = 0; i < STACKS; i++) {
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
