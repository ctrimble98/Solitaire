#include "server.h"

int main(int argc, char const *argv[]) {

    int seed;
    if (argc < 2) {
        seed = time(NULL);
    }
    else {
        seed = *argv[1];
    }

    std::array<Card, CARD_NO> cards;
    auto start = std::chrono::high_resolution_clock::now();
    int wins = 0;
    int games = 10000;

    for (size_t k = 0; k < games; k++) {

        seed++;
        Klondike game = Klondike(seed);
        if (weightedSolve(game)) {
            wins++;
        }
    }
    auto end = std::chrono::high_resolution_clock::now();
    std::cout << wins << " out of " << games << " were won." << std::endl;
    std::cout << "(" << std::chrono::duration_cast<std::chrono::milliseconds>(end - start).count() << "ms total time)"<< '\n';
    return 0;
}
