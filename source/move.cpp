#include "move.h"

std::array<int, 3> Move::getStart() {
    return start;
}

std::array<int, 3> Move::getEnd() {
    return end;
}

Card Move::getCard() {
    return card;
}

Move::Move(std::array<int, 3> start, std::array<int, 3> end, Card card) : start(start), end(end), card(card) {
}

void Move::printMove() {
    std::cout << card.toString(false) << ": (" << start[0] << ", " << start[1] << ", " << start[2] << ")" << " -> " << "(" << end[0] << ", " << end[1] << ", " << end[2] << ")" << std::endl;
}
