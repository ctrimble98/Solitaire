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
