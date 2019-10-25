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
    int winsW = 0;
    int winsR = 0;
    int games = 100;

    std::string solvCommand = "../solvitaireHome --type klondike --classify ";
    std::string solvInput = "klondike.json";
    std::string solvOutFile = "solvOut.txt";

    std::ofstream ofs;
    std::ifstream infile;
    infile.open(solvOutFile);
    ofs.open(solvOutFile, std::ofstream::out | std::ofstream::trunc);
    ofs.close();

    bool gameComp[games];
    std::string gameCompText[games];

    for (size_t k = 0; k < games; k++) {

        seed++;
        Klondike game = Klondike(seed);
        if (weightedSolve(game)) {
            gameComp[k] = true;
            winsW++;
        } else {
            gameComp[k] = false;
        }
        // game = Klondike(seed);
        // if (randomSolve(game)) {
        //     winsR++;
        // }

        game.printJsonToFile(false, solvInput);
        system((solvCommand + solvInput + " >> " + solvOutFile).c_str());
    }

    std::string line;
    std::string lastLine;
    std::string result;

    int k = 0;
    infile.seekg(0, std::ios::beg);
    if (infile.is_open()) {
        while (getline(infile, line)) {
            std::istringstream iss(line);
            std::vector<std::string> results((std::istream_iterator<std::string>(iss)), std::istream_iterator<std::string>());
            if (results.size() > 0) {
                gameCompText[k] = results.back();
                k++;
            }
        }
        infile.close();
    }

    int impGamesOne = 0;
    for (int i = 0; i < games; i++) {
        std::cout << gameComp[i] << " " << gameCompText[i] << std::endl;
    }
    for (int i = 0; i < games; i++) {
        if (gameComp[i] == false && gameCompText[i].compare("solved")) {
            impGamesOne++;
        }
    }
    std::cout << impGamesOne << " unsolvable games were won" << std::endl;

    auto end = std::chrono::high_resolution_clock::now();
    std::cout << winsR << " out of " << games << " were won by randomSolve." << std::endl;
    std::cout << winsW << " out of " << games << " were won by weightedSolve." << std::endl;
    std::cout << "(" << std::chrono::duration_cast<std::chrono::milliseconds>(end - start).count() << "ms total time)"<< '\n';
    return 0;
}
