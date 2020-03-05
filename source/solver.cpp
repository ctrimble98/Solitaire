#include "solver.h"

Solver::Solver(std::string name, std::vector<Heuristic> heuristics) : name(name), heuristics(heuristics) {
    sort(heuristics.begin(), heuristics.end(), std::greater <>());
}

// bool Solver::run(Klondike game, int seed) {
//     bool allLegalMoves = false;
//     int maxMoves = 200;
//     std::vector<Move> moves = game.findMoves(allLegalMoves);
//     int movesMade = 0;
//     srand(seed);
//
//     while (!moves.empty() && movesMade < maxMoves) {
//         int n = moves.size();
//         std::vector<Move> bestMoves = moves;
//         int bestScore = MIN_SCORE + 1;
//         for (auto const &move: moves) {
//             if (checkSafeMove(game, move)) {
//                 bestMoves = std::vector<Move>();
//                 bestMoves.push_back(move);
//                 break;
//             }
//             int score = 0;
//             for (auto &h: heuristics) {
//                 //Need to default score to zero and make additive. Also remove bad moves
//                 score += h.run(game, move);
//             }
//             if (score >= bestScore) {
//                 if (score > bestScore) {
//                     bestScore = score;
//                     bestMoves = std::vector<Move>();
//                 }
//                 bestMoves.push_back(move);
//             }
//         }
//         Move chosenMove = bestMoves[rand() % bestMoves.size()];
//         game.makeMove(chosenMove);
//
//         // for (auto &move: moves) {
//         //     move.printMove();
//         // }
//         // std::cout << "Chosen Move" << std::endl;
//         // chosenMove.printMove();
//         // game.printGame(true);
//         moves = game.findMoves(allLegalMoves);
//         movesMade++;
//     }
//     return game.isWon();
// }

std::string Solver::getName() {
    return name;
}

bool checkSafeMove(Klondike game, Move move, bool print) {
    bool safeMoveStart = true;
    if (game.getDeal() > 1) {
        safeMoveStart = (move.getStart()[0] == static_cast<int>(CardLocation::TABLEAU) || (move.getStart()[0] == static_cast<int>(CardLocation::STOCK) && (game.getStockPointer() + 1) % game.getDeal() == 0 && move.getEnd()[2] == (signed)game.getStock().size() - 1));
    }
    bool safeMoveEnd = getSafeFoundation(game, move);
    // if (print) {
    //     std::cout << '\n';
    //     move.printMove();
    //     std::cout << " " << safeMoveStart << " " << safeMoveEnd << '\n';
    //     std::cout << '\n';
    // }
    if (safeMoveStart && safeMoveEnd) {

        return true;
    }
    return false;
}

// bool Solver::runSearch(Klondike game, int seed) {
//     bool allLegalMoves = false;
//     int maxMoves = 200;
//     std::vector<Move> moves = game.findMoves(allLegalMoves);
//     int movesMade = 0;
//     srand(seed);
//
//     while (!moves.empty() && movesMade < maxMoves) {
//
//
//         for (std::vector<Card> &stack: game.getTableau()) {
//             int n = stack.size();
//             if (n > 0 && stack[0].isFaceDown()) {
//                 Card card = stack[0];
//                 for (int i = 1; i < n; i++) {
//                     card = stack[i];
//                     if (!card.isFaceDown()) {
//                         break;
//                     }
//                 }
//
//             }
//             move.getStart()[0] == static_cast<int>(CardLocation::TABLEAU) && move.getStart()[2] > 0 && game.getTableau()[move.getStart()[1]][move.getStart()[2] - 1].isFaceDown())
//         }
//
//
//         int n = moves.size();
//         std::vector<Move> bestMoves = moves;
//         int bestScore = MIN_SCORE + 1;
//         for (auto const &move: moves) {
//             if (checkSafeMove(game, move)) {
//                 bestMoves = std::vector<Move>();
//                 bestMoves.push_back(move);
//                 break;
//             }
//             int score = 0;
//             for (auto &h: heuristics) {
//                 //Need to default score to zero and make additive. Also remove bad moves
//                 score += h.run(game, move);
//             }
//             if (score >= bestScore) {
//                 if (score > bestScore) {
//                     bestScore = score;
//                     bestMoves = std::vector<Move>();
//                 }
//                 bestMoves.push_back(move);
//             }
//         }
//         Move chosenMove = bestMoves[rand() % bestMoves.size()];
//         game.makeMove(chosenMove);
//
//         // for (auto &move: moves) {
//         //     move.printMove();
//         // }
//         // std::cout << "Chosen Move" << std::endl;
//         // chosenMove.printMove();
//         // game.printGame(true);
//         moves = game.findMoves(allLegalMoves);
//         movesMade++;
//     }
//     return game.isWon();
// }

bool Solver::run(Klondike game, int seed) {
    bool allLegalMoves = false;
    int maxMoves = 200;
    std::vector<Move> moves = game.findMoves(allLegalMoves);
    int movesMade = 0;
    srand(seed);

    while (!moves.empty() && movesMade < maxMoves) {

        // for (auto &move: moves) {
        //     move.printMove();
        // }
        // game.printGame(true);

        bool madeMove = false;
        for (auto &move: moves) {
            if (checkSafeMove(game, move, true) || (move.getStart()[0] == static_cast<int>(CardLocation::TABLEAU) && move.getStart()[2] > 0 && game.getTableau()[move.getStart()[1]][move.getStart()[2] - 1].isFaceDown())) {
                game.makeMove(move);
                madeMove = true;

                // std::cout << "Chosen Move" << std::endl;
                // move.printMove();
                // std::cout << std::endl;
                break;
            }
        }

        if (!madeMove) {
            for (int i = 0; i < 10; i++) {
                for (auto &move: moves) {
                    if (dfs(Klondike(game), move, 0, i)) {
                        game.makeMove(move);
                        madeMove = true;
                        // std::cout << "Chosen Move" << std::endl;
                        // move.printMove();
                        // std::cout << std::endl;
                        break;
                    }
                }
                if (madeMove) {
                    break;
                }
            }
        }

        if (!madeMove) {
            // Move move = moves[rand() % moves.size()];
            // game.makeMove(move);
            // std::cout << "Chosen Move" << std::endl;
            // move.printMove();
            // std::cout << std::endl;
            int n = moves.size();
            std::vector<Move> bestMoves = moves;
            int bestScore = MIN_SCORE + 1;
            for (auto const &move: moves) {
                if (checkSafeMove(game, move, false)) {
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

        moves = game.findMoves(allLegalMoves);
        movesMade++;
    }
    return game.isWon();
}

bool dfs(Klondike game, Move move, int depth, int maxDepth) {


    if (game.makeMove(move)) {
        return false;
    }

    std::vector<Move> moves = game.findMoves(false);

    for (auto &move: moves) {
        if (/*checkSafeMove(game, move, false) || */(move.getStart()[0] == static_cast<int>(CardLocation::TABLEAU) && move.getStart()[2] > 0 && game.getTableau()[move.getStart()[1]][move.getStart()[2] - 1].isFaceDown())) {
            return true;
        }
    }

    if (depth < maxDepth) {
        for (auto &move: moves) {
            if (dfs(Klondike(game), move, depth + 1, maxDepth)) {
                return true;
            }
        }
    }

    return false;
}

// bool dfs(Klodike game, std::vector<Move> moves) {
//
//     std::stack<Klondike> currentStack;
//     Klondike currentNode;
//
//     for (auto& move: moves) {
//
//         currentNode = Klondike(game);
//
//         if (currentNode.makeMove(move)) {
//              ;
//         }
//
//         currentStack.push()
//
//         moves = game.findMoves(false);
//     }
// }
