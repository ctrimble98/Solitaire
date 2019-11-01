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
    int games = 100000;

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

    for (int i = 0; i < games; i++) {

        seed++;
        Klondike game = Klondike(seed);
        if (weightedSolve(game)) {
            gameComp[i] = true;
            wins++;
        } else {
            gameComp[i] = false;
        }

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
    std::cout << wins << " out of " << games << " were won by weightedSolve." << std::endl;
    std::cout << "(" << std::chrono::duration_cast<std::chrono::milliseconds>(end - start).count() << "ms total time)"<< '\n';
    return 0;
}

void runGames(int games, int seed, bool verify) {

}
