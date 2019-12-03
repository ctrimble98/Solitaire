#include "server.h"

int main(int argc, char *argv[]) {

    int games = 1000;
    int seed = time(NULL);
    std::string hFileName = "";
    bool verify = false;
    Verifier verifier = Verifier("klondike.json", "solvOut.txt");

    int opt;
    while((opt = getopt(argc, argv, "g:s:h:v")) != -1) {
        switch(opt) {
            case 'g':
                games = std::stoi(optarg);
                break;
            case 's':
                seed = std::stoi(optarg);
                break;
            case 'h':
                hFileName = optarg;
                break;
            case 'v':
                verify = true;
                break;
            case '?':
                std::cout << "Unknown option: " << optopt << std::endl;
                break;
        }
    }

    std::vector<Solver> solvers = setSolvers(hFileName);

    SolverCompare comp(solvers);

    auto start = std::chrono::high_resolution_clock::now();
    comp = runGames(comp, seed, games, verify, verifier);
    auto end = std::chrono::high_resolution_clock::now();

    std::ofstream out("comp.csv");
    std::streambuf *coutbuf = std::cout.rdbuf();
    std::cout.rdbuf(out.rdbuf());
    std::cout << comp.toString();
    std::cout.rdbuf(coutbuf);

    std::cout << comp.toString();
    std::cout << "(" << std::chrono::duration_cast<std::chrono::milliseconds>(end - start).count() << "ms total time)"<< '\n';
    return 0;
}

std::vector<Solver> setSolvers(std::string hFileName) {

    std::vector<Solver> solvers;

    Heuristic h1 = Heuristic(HeuristicType::FOUNDATION, FOUNDATION_SCORE);
    Heuristic h2 = Heuristic(HeuristicType::REVEAL_HIDDEN, REVEAL_HIDDEN_SCORE);
    Heuristic h3 = Heuristic(HeuristicType::PLAN_REVEAL_HIDDEN, PLAN_REVEAL_HIDDEN_SCORE);
    Heuristic h4 = Heuristic(HeuristicType::EMPTY_SPACE_NO_KING, EMPTY_SPACE_NO_KING_SCORE);

    solvers.push_back(Solver("Random", std::vector<Heuristic>()));

    std::vector<Heuristic> heuristics;

    // heuristics.push_back(h3);
    // heuristics.push_back(h4);
    // heuristics.push_back(h1);
    // heuristics.push_back(h2);
    // solvers.push_back(Solver("All", heuristics));

    heuristics = std::vector<Heuristic>();
    heuristics.push_back(h1);
    heuristics.push_back(h2);
    heuristics.push_back(h3);
    solvers.push_back(Solver("No empty unless king", heuristics));

    return solvers;
}

SolverCompare runGames(SolverCompare comp, int seed, int games, bool verify, Verifier verifier) {

    for (int i = 0; i < games; i++) {
        float percent = static_cast<float>(i*100)/games;
        std::cout << percent << "%   \r";

        seed++;
        Klondike game = Klondike(seed, 3);
        bool anyWins = comp.runSolvers(game, seed);

        if (anyWins && verify == true) {
            verifier.checkGame(game);
        }
    }
    return comp;
}
