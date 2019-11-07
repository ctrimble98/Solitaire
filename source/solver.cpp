#include "solver.h"

Solver::Solver(std::string name, std::vector<Heuristic> heuristics) : name(name), heuristics(heuristics) {
    sort(heuristics.begin(), heuristics.end(), std::greater <>());
}

bool Solver::run(Klondike game) {
    bool allLegalMoves = false;
    int maxMoves = 200;
    std::vector<Move> moves = game.findMoves(allLegalMoves);
    int i = 0;
    srand(time(NULL));

    while (!moves.empty() && i < maxMoves) {
        int n = moves.size();
        std::vector<Move> bestMoves = moves;
        std::vector<int> scores(n);
        // std::array<int, 2> minFoundation = getFoudationMin(game);
        // for (int j = 0; j < n; j++) {
        //     for (auto &h: heuristics) {
        //         scores[j] = h.getFcn()(game, moves[j], h.getScore());
        //         if (scores[j] != NOT_SATISFIED_SCORE) {
        //             break;
        //         }
        //     }
        // }

        int bestScore = NOT_SATISFIED_SCORE + 1;
        for (int j = 0; j < n; j++) {
            for (auto &h: heuristics) {
                if (h.getScore() >= bestScore) {
                    int score = h.getFcn()(game, moves[j]);
                    if (score >= bestScore) {
                        if (score > bestScore) {
                            bestScore = score;
                            bestMoves = std::vector<Move>();
                        }
                        bestMoves.push_back(moves[j]);
                        break;
                    }
                } else {
                    break;
                }
            }
        }
        game.makeMove(bestMoves[rand() % bestMoves.size()]);
        moves = game.findMoves(allLegalMoves);
        i++;
    }
    return game.isWon();
}

std::string Solver::getName() {
    return name;
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
//
// bool weightedSolveAll(Klondike game) {
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
//             } else if (bestScore <= 0 && !checkNothingMove(game, moves[j])) {
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
