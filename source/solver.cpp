#include "solver.h"

Solver::Solver(std::string name, std::vector<Heuristic> heuristics) : name(name), heuristics(heuristics) {
    sort(heuristics.begin(), heuristics.end(), std::greater <>());
}

bool Solver::run(Klondike game, int seed) {
    bool allLegalMoves = false;
    int maxMoves = 200;
    std::vector<Move> moves = game.findMoves(allLegalMoves);
    int movesMade = 0;
    srand(seed);

    while (!moves.empty() && movesMade < maxMoves) {
        int n = moves.size();
        std::vector<Move> bestMoves = moves;
        int bestScore = MIN_SCORE + 1;
        for (auto const &move: moves) {
            if (checkSafeMove(game, move)) {
                bestMoves = std::vector<Move>();
                bestMoves.push_back(move);
                break;
            }
            int score = 0;
            for (auto &h: heuristics) {
                //Need to default score to zero and make additive. Also remove bad moves
                score += h.run(game, move);
            }
            score *= scoreStockMove(game, move);
            if (score >= bestScore) {
                if (score > bestScore) {
                    bestScore = score;
                    bestMoves = std::vector<Move>();
                }
                bestMoves.push_back(move);
            }
        }
        Move chosenMove = bestMoves[rand() % bestMoves.size()];
        game.makeMove(chosenMove);

        // for (auto &move: moves) {
        //     move.printMove();
        // }
        // std::cout << "Chosen Move" << std::endl;
        // chosenMove.printMove();
        // game.printGame(true);
        moves = game.findMoves(allLegalMoves);
        movesMade++;
    }
    return game.isWon();
}

std::string Solver::getName() {
    return name;
}

bool checkSafeMove(Klondike game, Move move) {
    bool safeMoveStart = true;
    if (game.getDeal() > 1) {
        safeMoveStart = (move.getStart()[0] == static_cast<int>(CardLocation::TABLEAU) || (move.getStart()[0] == static_cast<int>(CardLocation::STOCK) && (game.getStockPointer() + 1) % game.getDeal() == 0 && move.getEnd()[2] == (signed)game.getStock().size() - 1));
    }
    bool safeMoveEnd = getSafeFoundation(game, move);
    if (safeMoveStart && safeMoveEnd) {

        return true;
    }
    return false;
}
