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
        int bestScore = 0;;
        std::vector<Move> bestMoves = moves;
        //game.printGame(true);
        std::array<int, 2> minFoundation = getFoudationMin(game);
        for (size_t j = 0; j < n; j++) {
            if (bestScore <= 10 && moves[j].getEnd()[0] == static_cast<int>(CardLocation::FOUNDATION) && moves[j].getCard().getRank() <= minFoundation[0] + 2) {
                if (bestScore < 10) {
                    bestScore = 10;
                    bestMoves = std::vector<Move>();
                }
                bestMoves.push_back(moves[j]);
            } else if (bestScore <= 1 && moves[j].getStart()[0] > static_cast<int>(CardLocation::STOCK) && moves[j].getStart()[0] < static_cast<int>(CardLocation::FOUNDATION) && moves[j].getStart()[1] > 0 && game.getTableau()[moves[j].getStart()[0] - static_cast<int>(CardLocation::TABLEAU_START)][moves[j].getStart()[1] - 1].isFaceDown()) {
                if (bestScore < 1) {
                    bestScore = 1;
                    bestMoves = std::vector<Move>();
                }
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
    for (size_t i = 0; i < 4; i++) {
        if (!foundation[i].empty() && foundation[i].top().getRank() < min[0]) {
            min[0] = foundation[i].top().getRank();
        }
    }
    //std::cout << min[0] << std::endl;
    return min;
}
