#include "move.h"

std::array<int, 2> Move::getStart() {
    return start;
}

std::array<int, 2> Move::getEnd() {
    return end;
}

Card Move::getCard() {
    return card;
}

Move::Move(std::array<int, 2> start, std::array<int, 2> end, Card card) : start(start), end(end), card(card) {
}
