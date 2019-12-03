#include "verifier.h"

Verifier::Verifier(std::string solvInput, std::string solvOutFile) : solvInput(solvInput), solvOutFile(solvOutFile) {
    std::string solvCommand = "../solvitaireHome --type klondike-deal-1 --classify ";
    clearOutFile();
}

void Verifier::clearOutFile() {
    std::ofstream ofs;
    std::ifstream infile;
    infile.open(solvOutFile);
    ofs.open(solvOutFile, std::ofstream::out | std::ofstream::trunc);
    ofs.close();
}

void Verifier::checkGame(Klondike game) {

    game.printJsonToFile(false, solvInput);
    if (system((solvCommand + solvInput + " >> " + solvOutFile).c_str()) == -1) {
        std::cerr << "Solvitaire error" << '\n';
    }

    std::string gameCompText;
    std::string line;
    std::string result;
    std::ifstream infile;
    infile.open(solvOutFile);

    infile.seekg(0, std::ios::beg);
    if (infile.is_open()) {
        while (getline(infile, line)) {
            std::istringstream iss(line);
            std::vector<std::string> results((std::istream_iterator<std::string>(iss)), std::istream_iterator<std::string>());
            if (results.size() > 0) {
                gameCompText = results.back();
            }
        }
        infile.close();
    }

    if (gameCompText.compare("unsolvable")) {
        std::cout << "Solved game unsolvable" << std::endl;
        game.printGame(false);
    }
}
