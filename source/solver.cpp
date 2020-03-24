#include "solver.h"

Solver::Solver(std::string name, std::vector<Heuristic> heuristics, std::function<bool(Solver*, Klondike, std::vector<Move>)> searchFcn, std::function<int(int, int)> limitFcn) : name(name), heuristics(heuristics), searchFcn(searchFcn), limitFcn(limitFcn) {
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

int Solver::getNodes() {
    return nodes;
}

void Solver::addNode() {
    nodes++;
}

std::function<int(int, int)> Solver::getLimitFcn() {
    return limitFcn;
}

bool checkSafeMove(Klondike game, Move move, bool print) {
    bool safeMoveStart = true;
    if (game.getDeal() > 1) {
        safeMoveStart = (move.getStart()[0] == static_cast<int>(CardLocation::TABLEAU) || (move.getStart()[0] == static_cast<int>(CardLocation::STOCK) && (game.getStockPointer() + 1) % game.getDeal() == 0 && move.getStart()[2] == (signed)game.getStock().size() - 1));
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

bool Solver::run(Klondike game, int seed, bool verbose) {
    bool allLegalMoves = false;
    int maxMoves = 150;
    std::vector<Move> moves = game.getMoves(allLegalMoves);
    int movesMade = 0;
    srand(seed);

    while (!moves.empty() && movesMade < maxMoves) {

        int chosenMove = rand() % moves.size();

        if (verbose) {
            std::cout << "Solver : " << name << " Game State " << std::endl;
            std::cout << "Moves made: " << movesMade << std::endl;
            game.printGame(true);
            std::cout << "Moves Available:" << std::endl;
            for (auto &move: moves) {
                move.printMove();
            }
        }

        bool madeMove = false;
        for (int i = 0; i < moves.size(); i++) {
            if (checkSafeMove(game, moves[i], true)) {
                chosenMove = i;
                madeMove = true;
                break;
            }
        }
        if (!madeMove) {
            for (int i = 0; i < moves.size(); i++) {
                Move move = moves[i];
                if ((move.getStart()[0] == static_cast<int>(CardLocation::TABLEAU) && move.getStart()[2] > 0 && game.getTableau()[move.getStart()[1]][move.getStart()[2] - 1].isFaceDown())) {
                    chosenMove = i;
                    madeMove = true;
                    break;
                }
            }
        }

        if (!madeMove && game.hasHiddenCards()) {
            nodes = 0;
            int chosenIndex = searchFcn(this, game, moves);
            std::cout << movesMade << " " << chosenIndex << " " << moves.size() << '\n';
            if (chosenIndex > -1) {
                std::cout << chosenIndex << " " << searchFcn(this, game, moves) << '\n';
                chosenMove = chosenIndex;
                madeMove = true;
            }
        }
        if (!madeMove) {
            int n = moves.size();
            std::vector<int> bestMoves(moves.size());
            std::iota(bestMoves.begin(), bestMoves.end(), 0);
            int bestScore = MIN_SCORE + 1;
            for (int i = 0; i < moves.size(); i++) {
                if (checkSafeMove(game, moves[i], false)) {
                    bestMoves = std::vector<int>();
                    bestMoves.push_back(i);
                    break;
                }
                int score = 0;
                for (auto &h: heuristics) {
                    score += h.run(game, moves[i]);
                }
                if (score >= bestScore) {
                    if (score > bestScore) {
                        bestScore = score;
                        bestMoves = std::vector<int>();
                    }
                    bestMoves.push_back(i);
                }
            }
            chosenMove = bestMoves[rand() % bestMoves.size()];
        }

        game.makeMove(chosenMove);

        if (verbose) {
            std::cout << "Chosen Move" << std::endl;
            moves[chosenMove].printMove();
            std::cout << std::endl;
        }

        moves = game.getMoves(allLegalMoves);
        movesMade++;
    }
    return game.isWon();
}

int runSearchCheckStock(Solver* solver, Klondike game, std::vector<Move> moves) {
    int bestScore = -1;
    int chosenMove = -1;
    for (int i = 0; i < 8; i++) {
        for (int j = 0; j < moves.size(); j++) {
            Klondike temp = Klondike(game);
            temp.makeMove(j);
            int score = dfs(solver, temp, moves[j], 0, i, false, 0);
            if (score == 20 || score == 10) {
                bestScore = score;
                chosenMove = j;
                break;
            } else if (score > bestScore) {
                chosenMove = j;
                bestScore = score;
            }
        }
        if (chosenMove > -1) {
            break;
        }
    }
    if (chosenMove == -1) {
        std::cout << "/* message */" << '\n';
    }
    return chosenMove;
}

int runSearchNoCheckStock(Solver* solver, Klondike game, std::vector<Move> moves) {
    for (int i = 0; i < 8; i++) {
        for (size_t j = 0; j < moves.size(); j++) {
            Klondike temp = Klondike(game);
            temp.makeMove(j);
            if (dfs(solver, temp, moves[j], 0, i, false, 0) > -1) {
                // Move move = moves[j];
                return j;
            }
        }
    }
    return -1;
}

int runSearchDFS(Solver* solver, Klondike game, std::vector<Move> moves) {
    for (size_t i = 0; i < moves.size(); i++) {
        Klondike temp = Klondike(game);
        temp.makeMove(i);
        if (dfs(solver, temp, moves[i], 0, 8, false, 0) > -1) {
            // Move move = moves[i];
            return i;
        }
    }
    return -1;
}

// bool dfs(Klondike game, Move move, int depth, int maxDepth, bool performedStockMove) {
//
//
//     if (game.makeMove(move)) {
//         return false;
//     }
//
//     std::vector<Move> moves = game.findMoves(false);
//
//     for (auto &move: moves) {
//         if (!performedStockMove && checkSafeMove(game, move, false)) {
//             return true;
//         } else if (move.getStart()[0] == static_cast<int>(CardLocation::TABLEAU) && move.getStart()[2] > 0 && game.getTableau()[move.getStart()[1]][move.getStart()[2] - 1].isFaceDown()) {
//             return true;
//         }
//     }
//
//     if (depth < maxDepth) {
//         for (auto &move: moves) {
//             if (move.getStart()[0] == static_cast<int>(CardLocation::STOCK)) {
//                 performedStockMove = true;
//             }
//             if (dfs(Klondike(game), move, depth + 1, maxDepth, performedStockMove)) {
//                 return true;
//             }
//         }
//     }
//
//     return false;
// }

int dfs(Solver* solver, Klondike game, Move move, int depth, int maxDepth, bool performedStockMove, int currentScore) {

    solver->addNode();
    if (move.getStart()[0] == static_cast<int>(CardLocation::STOCK)) {
        performedStockMove = true;
    }

    // game.makeMove(move));

    std::vector<Move> moves = game.getMoves(false);

    for (auto &move: moves) {
        if (!performedStockMove && checkSafeMove(game, move, false)) {
            return 10 + currentScore;
        } else if (move.getStart()[0] == static_cast<int>(CardLocation::TABLEAU) && move.getStart()[2] > 0 && game.getTableau()[move.getStart()[1]][move.getStart()[2] - 1].isFaceDown()) {
            return 20 + currentScore;
        }
    }

    if (solver->getLimitFcn()(depth, solver->getNodes())) {
        int score = -1;
        for (int i = 0; i < moves.size(); i++) {
            int modifier = moves[i].getStart()[0] == static_cast<int>(CardLocation::STOCK) ? -1 : 0;
            Klondike temp = Klondike(game);
            temp.makeMove(i);
            int moveScore = dfs(solver, temp, moves[i], depth + 1, maxDepth, performedStockMove, currentScore + modifier);
            if (moveScore > score) {
                score = moveScore;
            }
        }

        return score;
    }
    return currentScore;
}

// int dfs(Klondike game, Move move, int depth, int maxDepth, bool performedStockMove, int currentScore) {
//
//     if (move.getStart()[0] == static_cast<int>(CardLocation::STOCK)) {
//         performedStockMove = true;
//     }
//
//     if (!game.makeMove(move)) {
//         return -1;
//     }
//
//     std::vector<Move> moves = game.findMoves(false);
//
//     for (auto &move: moves) {
//         if (!performedStockMove && checkSafeMove(game, move, false)) {
//             return 10 + currentScore;
//         } else if (move.getStart()[0] == static_cast<int>(CardLocation::TABLEAU) && move.getStart()[2] > 0 && game.getTableau()[move.getStart()[1]][move.getStart()[2] - 1].isFaceDown()) {
//             return 10 + currentScore;
//         }
//     }
//
//     if (depth < maxDepth) {
//         for (auto &move: moves) {
//             if (dfs(Klondike(game), move, depth + 1, maxDepth, performedStockMove, currentScore) == 0) {
//                 return 0;
//             }
//         }
//     }
//
//     return -1;
// }

bool depthLimit(int depth, int nodes) {
    if (depth < 6) {
        return true;
    }
    return false;
}

bool nodeLimit(int depth, int nodes) {
    if (nodes < 150) {
        return true;
    }
    return false;
}
