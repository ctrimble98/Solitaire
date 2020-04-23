 #include "solver.h"

Solver::Solver(std::string name, std::vector<Heuristic> heuristics, std::function<int(Solver*, Klondike, std::vector<Move>)> searchFcn, std::function<int(Solver*, int)> searchValueFcn, int searchLimit) : name(name), heuristics(heuristics), searchFcn(searchFcn), searchValueFcn(searchValueFcn), limit(searchLimit) {
    sort(heuristics.begin(), heuristics.end(), std::greater <>());
}

std::string Solver::getName() {
    return name;
}

int Solver::getNodes() {
    return nodes;
}

void Solver::addNode() {
    nodes++;
}

int Solver::getLimit() {
    return limit;
}

std::function<int(int, int)> Solver::getLimitFcn() {
    return limitFcn;
}

std::function<int(Solver*, int)> Solver::getSearchValueFcn() {
    return searchValueFcn;
}

bool checkSafeMove(Klondike game, Move move) {
    bool safeMoveStart = true;
    if (game.getDeal() > 1) {
        safeMoveStart = (move.getStart()[0] == static_cast<int>(CardLocation::TABLEAU) || (move.getStart()[0] == static_cast<int>(CardLocation::STOCK) && (game.getStockPointer() + 1) % game.getDeal() == 0 && move.getStart()[2] == (signed)game.getStock().size() - 1));
    }
    bool safeMoveEnd = getSafeFoundation(game, move);
    if (safeMoveStart && safeMoveEnd) {
        return true;
    }
    return false;
}

bool Solver::run(Klondike game, int seed, bool verbose) {
    bool allLegalMoves = false;
    int maxMoves = 300;
    std::vector<Move> moves = game.getMoves(allLegalMoves);
    int movesMade = 0;
    srand(seed);

    while (!moves.empty() && movesMade < maxMoves && !game.isWon()) {

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
            if (checkSafeMove(game, moves[i])) {
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
            if (chosenIndex > -1) {
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

int runNoSearch(Solver* solver, Klondike game, std::vector<Move> moves) {
    return -1;
}

int runSearchIDDFSCheckStock(Solver* solver, Klondike game, std::vector<Move> moves) {
    int bestScore = -1;
    int chosenMove = -1;
    for (int i = 0; i < solver->getLimit(); i++) {
        for (int j = 0; j < moves.size(); j++) {
            Klondike temp = Klondike(game);
            temp.makeMove(j);
            int score = dfs(solver, temp, moves[j], 0, i, false, 0);
            if (score == 250 || score == 240) {
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
    return chosenMove;
}

int runSearchIDDFSNoCheckStock(Solver* solver, Klondike game, std::vector<Move> moves) {
    for (int i = 0; i < solver->getLimit(); i++) {
        for (size_t j = 0; j < moves.size(); j++) {
            Klondike temp = Klondike(game);
            temp.makeMove(j);
            if (dfs(solver, temp, moves[j], 0, i, false, 0) > -1) {
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
        if (dfs(solver, temp, moves[i], 0, 10, false, 0) > -1) {
            return i;
        }
    }
    return -1;
}

int dfs(Solver* solver, Klondike game, Move move, int depth, int maxDepth, bool performedStockMove, int currentScore) {

    solver->addNode();
    if (move.getStart()[0] == static_cast<int>(CardLocation::STOCK)) {
        performedStockMove = true;
    }

    std::vector<Move> moves = game.getMoves(false);

    for (auto &nextMove: moves) {
        if (!performedStockMove && checkSafeMove(game, nextMove)) {
            return 250;
        }
    }

    for (auto &nextMove: moves) {
        if (nextMove.getStart()[0] == static_cast<int>(CardLocation::TABLEAU) && nextMove.getStart()[2] > 0 && game.getTableau()[nextMove.getStart()[1]][nextMove.getStart()[2] - 1].isFaceDown()) {
            return 240 + currentScore;
        }
    }

    if (checkLimit(solver->getSearchValueFcn()(solver, depth), solver->getLimit())) {
        int score = -1;
        for (int i = 0; i < moves.size(); i++) {
            int moveScore = 0;

            int modifier = moves[i].getStart()[0] == static_cast<int>(CardLocation::STOCK) ? -1*(game.getStock().size() - 1 - moves[i].getStart()[2]) : 0;
            Klondike temp = Klondike(game);
            temp.makeMove(i);
            moveScore = dfs(solver, temp, moves[i], depth + 1, maxDepth, performedStockMove, currentScore + modifier);

            if (moveScore > score) {
                score = moveScore;
            }
        }

        return score;
    }
    return currentScore;
}

int getDepth(Solver* solver, int depth) {
    return depth;
}

int getNodes(Solver* solver, int depth) {
    return solver->getNodes();
}

bool checkLimit(int value, int limit) {
    if (value < limit) {
        return true;
    }
    return false;
}

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
