#include "server.h"

int main(int argc, char const *argv[]) {

    int seed;
    if (argc < 2) {
        seed = time(NULL);
    }
    else {
        seed = *argv[1];
    }

    auto start = std::chrono::high_resolution_clock::now();
    int wins = 0;
    int games = 10000;

    std::string solvCommand = "../solvitaireHome --type klondike-deal-1 --classify ";
    std::string solvInput = "klondike.json";
    std::string solvOutFile = "solvOut.txt";

    std::ofstream ofs;
    std::ifstream infile;
    infile.open(solvOutFile);
    ofs.open(solvOutFile, std::ofstream::out | std::ofstream::trunc);
    ofs.close();

    bool solvitaire = false;

    Heuristic h1 = Heuristic(HeuristicType::FOUNDATION, FOUNDATION_SCORE);
    Heuristic h2 = Heuristic(HeuristicType::REVEAL_HIDDEN, REVEAL_HIDDEN_SCORE);
    Heuristic h3 = Heuristic(HeuristicType::PLAN_REVEAL_HIDDEN, PLAN_REVEAL_HIDDEN_SCORE);
    Heuristic h4 = Heuristic(HeuristicType::EMPTY_SPACE_NO_KING, EMPTY_SPACE_NO_KING_SCORE);

    std::vector<Solver> solvers;
    solvers.push_back(Solver("Random", std::vector<Heuristic>()));

    std::vector<Heuristic> heuristics;
    heuristics.push_back(h3);
    heuristics.push_back(h4);
    heuristics.push_back(h1);
    heuristics.push_back(h2);
    solvers.push_back(Solver("All", heuristics));

    heuristics = std::vector<Heuristic>();
    heuristics.push_back(h1);
    heuristics.push_back(h2);
    heuristics.push_back(h3);
    solvers.push_back(Solver("No empty unless king", heuristics));

    SolverCompare comp(solvers);

    for (int i = 0; i < games; i++) {
        float percent = static_cast<float>(i*100)/games;
        std::cout << percent << "%   \r";

        seed++;
        Klondike game = Klondike(seed, 1);
        // game.printGame(true);
        comp.runSolvers(game);

        if (solvitaire) {
            game.printJsonToFile(false, solvInput);
            if (system((solvCommand + solvInput + " >> " + solvOutFile).c_str()) == -1) {
                std::cerr << "Solvitaire error" << '\n';
            }
        }
    }

    if (solvitaire) {
        bool gameComp[games];
        std::string gameCompText[games];
        std::string line;
        std::string result;

        infile.seekg(0, std::ios::beg);
        if (infile.is_open()) {
            int i = 0;
            while (getline(infile, line)) {
                std::istringstream iss(line);
                std::vector<std::string> results((std::istream_iterator<std::string>(iss)), std::istream_iterator<std::string>());
                if (results.size() > 0) {
                    gameCompText[i] = results.back();
                    i++;
                }
            }
            infile.close();
        }

        int impGamesOne = 0;
        for (int i = 0; i < games; i++) {
            std::cout << gameComp[i] << " " << gameCompText[i] << std::endl;
        }
        for (int i = 0; i < games; i++) {
            if (gameComp[i] == true && !gameCompText[i].compare("unsolvable")) {
                impGamesOne++;
            }
        }
        std::cout << impGamesOne << " unsolvable games were won" << std::endl;
    }

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
