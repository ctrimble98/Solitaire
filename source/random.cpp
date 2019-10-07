#include "server.h"

bool randomSolve(Klondike game) {

    bool allLegalMoves = false;
    int maxMoves = 200;
    std::vector<std::array<int, 4>> moves = game.findMoves(allLegalMoves);
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
    std::vector<std::array<int, 4>> moves = game.findMoves(allLegalMoves);
    int i = 0;
    srand(time(NULL));
    while (!moves.empty() && i < maxMoves) {

        int n = moves.size();
        int bestScore = 0;;
        std::vector<std::array<int, 4>> bestMoves = moves;
        std::array<int, 2> minFoundation = getFoudationMin(game);
        for (size_t j = 0; j < n; j++) {
            if (bestScore <= 1 && moves[j][2] == static_cast<int>(CardLocation::FOUNDATION) && moves[j][3] <= minFoundation + 2) {
                if (bestScore < 1) {
                    bestScore = 1;
                    bestMoves = std::vector<std::array<int, 4>>();
                }
                bestMoves.push_back(moves[j]);
            }
        }

        game.makeMove(moves[rand() % moves.size()]);
        moves = game.findMoves(allLegalMoves);
        i++;
    }
    return game.isWon();
}

std::array<int, 2> getFoudationMin(Klondike game) {
    std::array<std::stack<Card>, 4> foundation = game.getFoundation();
    std::array<int, 2> min = {0,0};
    for (size_t i = 0; i < 4; i++) {
        if (!foundation[i].empty() && foundation[i].top().getRank() > min[i/2]) {
            min[i/2] = foundation[i].top().getRank();
        }
    }
    return min;
}
