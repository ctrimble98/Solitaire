#include "server.h"

int main(int argc, char *argv[]) {

    int games = 1000;
    int seed = time(NULL);
    int deal = 3;
    std::vector<std::string> hFiles;
    bool verify = false;
    Verifier verifier = Verifier("klondike.json", "solvOut.txt");

    int opt;
    while((opt = getopt(argc, argv, "d:g:s:h:v")) != -1) {
        switch(opt) {
            case 'd':
                deal = std::stoi(optarg);
                break;
            case 'g':
                games = std::stoi(optarg);
                break;
            case 's':
                seed = std::stoi(optarg);
                break;
            case 'h':
                hFiles.push_back(optarg);
                break;
            case 'v':
                verify = true;
                break;
            case '?':
                std::cout << "Unknown option: " << optopt << std::endl;
                break;
        }
    }

    std::cout << "Running solvers with " << deal << " deal klondike on " <<games << " games with seed " << seed;

    if (verify) {
        std::cout << " with solvitaire verification";
    }
    std::cout << std::endl;

    std::vector<Solver> solvers = setSolvers(hFiles);

    SolverCompare comp(solvers);

    auto start = std::chrono::high_resolution_clock::now();
    comp = runGames(comp, deal, seed, games, verify, verifier);
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

std::vector<Solver> setSolvers(std::vector<std::string> hFiles) {

    int foundScore = FOUNDATION_SCORE;
    int revealHiddenScore = REVEAL_HIDDEN_SCORE;
    int planRevealHiddenScore = PLAN_REVEAL_HIDDEN_SCORE;
    int emptyNoKingScore = EMPTY_SPACE_NO_KING_SCORE;

    std::vector<Solver> solvers;
    std::vector<Heuristic> heuristics;

    if (!hFiles.empty()) {

        std::cout << "Heuristics from " << hFiles.size() << " files" << std::endl;
        for (auto &filename: hFiles) {

            std::cout << "Processing " << filename << std::endl;

            std::ifstream ifs(filename);
            rapidjson::IStreamWrapper isw(ifs);
            rapidjson::Document document;
            document.ParseStream(isw);

            foundScore = document["FOUNDATION_SCORE"].GetInt();
            revealHiddenScore = document["REVEAL_HIDDEN_SCORE"].GetInt();
            planRevealHiddenScore = document["PLAN_REVEAL_HIDDEN_SCORE"].GetInt();
            emptyNoKingScore = document["EMPTY_SPACE_NO_KING_SCORE"].GetInt();

            Heuristic h1 = Heuristic(HeuristicType::FOUNDATION, foundScore);
            Heuristic h2 = Heuristic(HeuristicType::REVEAL_HIDDEN, revealHiddenScore);
            Heuristic h3 = Heuristic(HeuristicType::PLAN_REVEAL_HIDDEN, planRevealHiddenScore);
            Heuristic h4 = Heuristic(HeuristicType::EMPTY_SPACE_NO_KING, emptyNoKingScore);

            heuristics = std::vector<Heuristic>();
            heuristics.push_back(h1);
            heuristics.push_back(h2);
            heuristics.push_back(h3);
            heuristics.push_back(h4);
            solvers.push_back(Solver(filename, heuristics));
        }

        return solvers;
    }

    Heuristic h1 = Heuristic(HeuristicType::FOUNDATION, foundScore);
    Heuristic h2 = Heuristic(HeuristicType::REVEAL_HIDDEN, revealHiddenScore);
    Heuristic h3 = Heuristic(HeuristicType::PLAN_REVEAL_HIDDEN, planRevealHiddenScore);
    Heuristic h4 = Heuristic(HeuristicType::EMPTY_SPACE_NO_KING, emptyNoKingScore);

    //solvers.push_back(Solver("Random", std::vector<Heuristic>()));

    // heuristics.push_back(h3);
    // heuristics.push_back(h4);
    // heuristics.push_back(h1);
    // heuristics.push_back(h2);
    // solvers.push_back(Solver("All", heuristics));

    heuristics = std::vector<Heuristic>();
    heuristics.push_back(h1);
    heuristics.push_back(h2);
    heuristics.push_back(h3);
    heuristics.push_back(h4);
    solvers.push_back(Solver("All", heuristics));

    return solvers;
}

SolverCompare runGames(SolverCompare comp, int deal, int seed, int games, bool verify, Verifier verifier) {

    for (int i = 0; i < games; i++) {
        float percent = static_cast<float>(i*100)/games;
        std::cout << "Game " << i << '\n';
        std::cout << percent << "%   \r" << std::flush;

        seed++;
        Klondike game = Klondike(seed, deal);
        bool anyWins = comp.runSolvers(game, seed);

        if (anyWins && verify == true) {
            verifier.checkGame(game);
        }
    }
    return comp;
}
