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
    int games = 10000;

    for (size_t k = 0; k < games; k++) {

        seed++;
        Klondike game = Klondike(seed);
        if (weightedSolve(game)) {
            winsW++;
        }
        game = Klondike(seed);
        if (randomSolve(game)) {
            winsR++;
        }
    }
    auto end = std::chrono::high_resolution_clock::now();
    std::cout << winsR << " out of " << games << " were won by randomSolve." << std::endl;
    std::cout << winsW << " out of " << games << " were won by weightedSolve." << std::endl;
    std::cout << "(" << std::chrono::duration_cast<std::chrono::milliseconds>(end - start).count() << "ms total time)"<< '\n';
    return 0;
}
