#ifndef VERIFIER_H
#define VERIFIER_H

#include <iostream>
#include <ios>
#include <sstream>
#include <iterator>
#include "klondike.h"

class Verifier {

public:

    Verifier(std::string solvInput, std::string solvOutFile);

    void clearOutFile();
    void checkGame(Klondike game);

private:

    std::string solvCommand;
    std::string solvInput;
    std::string solvOutFile;
};

#endif
