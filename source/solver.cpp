#include "solver.h"

Solver::Solver(std::string name, std::vector<Heuristic> heuristics) : name(name), heuristics(heuristics) {
    sort(heuristics.begin(), heuristics.end(), std::greater <>());
}

bool Solver::run(Klondike game) {
    bool allLegalMoves = false;
    int maxMoves = 200;
    std::vector<Move> moves = game.findMoves(allLegalMoves);
    int movesMade = 0;
    srand(time(NULL));

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

// bool randomSolve(Klondike game) {
//
//     bool allLegalMoves = false;
//     int maxMoves = 200;
//     std::vector<Move> moves = game.findMoves(allLegalMoves);
//     int i = 0;
//     srand(time(NULL));
//     while (!moves.empty() && i < maxMoves) {
//         game.makeMove(moves[rand() % moves.size()]);
//         moves = game.findMoves(allLegalMoves);
//         i++;
//     }
//     return game.isWon();
// }

// bool Solver::run(Klondike game) {
//
//     bool allLegalMoves = false;
//     int maxMoves = 200;
//     std::vector<Move> moves = game.findMoves(allLegalMoves);
//     int i = 0;
//     srand(time(NULL));
//     while (!moves.empty() && i < maxMoves) {
//
//         int n = moves.size();
//         int bestScore = -1;
//         std::vector<Move> bestMoves = moves;
//         std::array<int, 2> minFoundation = getFoudationMin(game);
//         for (int j = 0; j < n; j++) {
//
//             int moveScore = 0;
//
//             if (checkSafeMove(game, moves[j])/*moves[j].getEnd()[0] == static_cast<int>(CardLocation::FOUNDATION) && ((moves[j].getCard().getColour() == Colour::RED && moves[j].getCard().getRank() <= minFoundation[1] + 2) || (moves[j].getCard().getColour() == Colour::BLACK && moves[j].getCard().getRank() <= minFoundation[0] + 2))*/) {
//                 //Check for safe moves to foundation
//
//                 bestMoves = std::vector<Move>();
//                 bestMoves.push_back(moves[j]);
//                 break;
//             }
//             if (moves[j].getStart()[2] && revealHiddenHeur(game, moves[j], 1) > 0/*moves[j].getStart()[0] == static_cast<int>(CardLocation::TABLEAU) && moves[j].getStart()[2] > 0 && game.getTableau()[moves[j].getStart()[1]][moves[j].getStart()[2] - 1].isFaceDown()*/) {
//                 //Check for moves that reveal hidden cards
//                 //if (bestScore < 20 + moves[j].getStart()[2]) {
//                     moveScore += 20 + moves[j].getStart()[2];
//                     //bestMoves = std::vector<Move>();
//                 //}
//                 // bestMoves.push_back(moves[j]);
//             }
//             if (planRevealHiddenHeur(game, moves[j], 1) > 0/*moves[j].getStart()[0] != static_cast<int>(CardLocation::TABLEAU) && moves[j].getEnd()[0] == static_cast<int>(CardLocation::TABLEAU)*/) {
//                 //Check for moves that open up moves to reveal hidden cards
//                 moveScore += 10 + checkFutureHidden(game, moves[j]);
//                 // if (moveScore > 0) {
//                 //     if (bestScore < 10 + moveScore) {
//                         // bestScore = 10 + moveScore;
//                 //         bestMoves = std::vector<Move>();
//                 //     }
//                 //     bestMoves.push_back(moves[j]);
//                 // }
//             }
//             // if (emptyNoKingHeur(game, moves[j], -1) < 0) {
//             //     // bestMoves.push_back(moves[j]);
//             //     moveScore += -1;
//             // }
//
//             if (bestScore <= moveScore) {
//                 if (bestScore < moveScore) {
//                     bestScore = moveScore;
//                     bestMoves = std::vector<Move>();
//                 }
//                 bestMoves.push_back(moves[j]);
//             }
//         }
//
//         game.makeMove(bestMoves[rand() % bestMoves.size()]);
//         moves = game.findMoves(allLegalMoves);
//         i++;
//     }
//     return game.isWon();
// }
//
// bool weightedSolve1(Klondike game) {
//
//     bool allLegalMoves = false;
//     int maxMoves = 200;
//     std::vector<Move> moves = game.findMoves(allLegalMoves);
//     int i = 0;
//     srand(time(NULL));
//     while (!moves.empty() && i < maxMoves) {
//
//         int n = moves.size();
//         int bestScore = -1;
//         std::vector<Move> bestMoves = moves;
//         std::array<int, 2> minFoundation = getFoudationMin(game);
//         for (int j = 0; j < n; j++) {
//
//             if (bestScore <= 100 && moves[j].getEnd()[0] == static_cast<int>(CardLocation::FOUNDATION) && ((moves[j].getCard().getColour() == Colour::RED && moves[j].getCard().getRank() <= minFoundation[1] + 2) || (moves[j].getCard().getColour() == Colour::BLACK && moves[j].getCard().getRank() <= minFoundation[0] + 2))) {
//                 //Check for safe moves to foundation
//                 if (bestScore < 100) {
//                     bestScore = 100;
//                     bestMoves = std::vector<Move>();
//                 }
//                 bestMoves.push_back(moves[j]);
//             } else if (bestScore <= 20 + moves[j].getStart()[2] && moves[j].getStart()[0] == static_cast<int>(CardLocation::TABLEAU) && moves[j].getStart()[2] > 0 && game.getTableau()[moves[j].getStart()[1]][moves[j].getStart()[2] - 1].isFaceDown()) {
//                 //Check for moves that reveal hidden cards
//                 if (bestScore < 20 + moves[j].getStart()[2]) {
//                     bestScore = 20 + moves[j].getStart()[2];
//                     bestMoves = std::vector<Move>();
//                 }
//                 bestMoves.push_back(moves[j]);
//             }
//         }
//
//         game.makeMove(bestMoves[rand() % bestMoves.size()]);
//         moves = game.findMoves(allLegalMoves);
//         i++;
//     }
//     return game.isWon();
// }
//
// bool weightedSolve2(Klondike game) {
//
//     bool allLegalMoves = false;
//     int maxMoves = 200;
//     std::vector<Move> moves = game.findMoves(allLegalMoves);
//     int i = 0;
//     srand(time(NULL));
//     while (!moves.empty() && i < maxMoves) {
//
//         int n = moves.size();
//         int bestScore = -1;
//         std::vector<Move> bestMoves = moves;
//         std::array<int, 2> minFoundation = getFoudationMin(game);
//         for (int j = 0; j < n; j++) {
//
//             if (bestScore <= 100 && moves[j].getEnd()[0] == static_cast<int>(CardLocation::FOUNDATION) && ((moves[j].getCard().getColour() == Colour::RED && moves[j].getCard().getRank() <= minFoundation[1] + 2) || (moves[j].getCard().getColour() == Colour::BLACK && moves[j].getCard().getRank() <= minFoundation[0] + 2))) {
//                 //Check for safe moves to foundation
//                 if (bestScore < 100) {
//                     bestScore = 100;
//                     bestMoves = std::vector<Move>();
//                 }
//                 bestMoves.push_back(moves[j]);
//             } else if (bestScore <= 20 + moves[j].getStart()[2] && moves[j].getStart()[0] == static_cast<int>(CardLocation::TABLEAU) && moves[j].getStart()[2] > 0 && game.getTableau()[moves[j].getStart()[1]][moves[j].getStart()[2] - 1].isFaceDown()) {
//                 //Check for moves that reveal hidden cards
//                 if (bestScore < 20 + moves[j].getStart()[2]) {
//                     bestScore = 20 + moves[j].getStart()[2];
//                     bestMoves = std::vector<Move>();
//                 }
//                 bestMoves.push_back(moves[j]);
//             } else if (bestScore < 20 && moves[j].getStart()[0] != static_cast<int>(CardLocation::TABLEAU) && moves[j].getEnd()[0] == static_cast<int>(CardLocation::TABLEAU)) {
//                 //Check for moves that open up moves to reveal hidden cards
//                 int moveScore = checkFutureHidden(game, moves[j]);
//                 if (moveScore > 0) {
//                     if (bestScore < 10 + moveScore) {
//                         bestScore = 10 + moveScore;
//                         bestMoves = std::vector<Move>();
//                     }
//                     bestMoves.push_back(moves[j]);
//                 }
//             }
//         }
//
//         game.makeMove(bestMoves[rand() % bestMoves.size()]);
//         moves = game.findMoves(allLegalMoves);
//         i++;
//     }
//     return game.isWon();
// }
//
// bool weightedSolve3(Klondike game) {
//
//     bool allLegalMoves = false;
//     int maxMoves = 200;
//     std::vector<Move> moves = game.findMoves(allLegalMoves);
//     int i = 0;
//     srand(time(NULL));
//     while (!moves.empty() && i < maxMoves) {
//
//         int n = moves.size();
//         int bestScore = -1;
//         std::vector<Move> bestMoves = moves;
//         std::array<int, 2> minFoundation = getFoudationMin(game);
//         for (int j = 0; j < n; j++) {
//
//             if (bestScore <= 100 && moves[j].getEnd()[0] == static_cast<int>(CardLocation::FOUNDATION) && ((moves[j].getCard().getColour() == Colour::RED && moves[j].getCard().getRank() <= minFoundation[1] + 2) || (moves[j].getCard().getColour() == Colour::BLACK && moves[j].getCard().getRank() <= minFoundation[0] + 2))) {
//                 //Check for safe moves to foundation
//                 if (bestScore < 100) {
//                     bestScore = 100;
//                     bestMoves = std::vector<Move>();
//                 }
//                 bestMoves.push_back(moves[j]);
//             }
//         }
//
//         game.makeMove(bestMoves[rand() % bestMoves.size()]);
//         moves = game.findMoves(allLegalMoves);
//         i++;
//     }
//     return game.isWon();
// }
