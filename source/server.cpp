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
    int games = 1000;

    std::string solvCommand = "../solvitaireHome --type klondike-deal-1 --classify ";
    std::string solvInput = "klondike.json";
    std::string solvOutFile = "solvOut.txt";

    std::ofstream ofs;
    std::ifstream infile;
    infile.open(solvOutFile);
    ofs.open(solvOutFile, std::ofstream::out | std::ofstream::trunc);
    ofs.close();

    bool gameComp[games];
    std::string gameCompText[games];
    bool solvitaire = false;

    std::vector<Heuristic> heuristics;
    heuristics.push_back(Heuristic(safeFoundationHeur, SAFE_FOUNDATION_SCORE));
    heuristics.push_back(Heuristic(revealHiddenHeur, REVEAL_HIDDEN_SCORE));
    heuristics.push_back(Heuristic(planRevealHiddenHeur, PLAN_REVEAL_HIDDEN_SCORE));
    heuristics.push_back(Heuristic(emptyNoKingHeur, EMPTY_SPACE_NO_KING_SCORE));

    std::vector<Solver> solvers;
    solvers.push_back(Solver("Random", std::vector<Heuristic>()));
    solvers.push_back(Solver("All", heuristics));
    SolverCompare comp(solvers);

    for (int i = 0; i < games; i++) {

        seed++;
        Klondike game = Klondike(seed);
        comp.runSolvers(game);

        if (solvitaire) {
            game.printJsonToFile(false, solvInput);
            system((solvCommand + solvInput + " >> " + solvOutFile).c_str());
        }
    }

    if (solvitaire) {
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
