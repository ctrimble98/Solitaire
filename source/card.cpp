#include "card.h"

Card::Card() {
    Card(Suit::HEARTS, 0);
}

Card::Card(uint8_t value) {

    std::cout << value << std::endl;

    switch (value / CARDS_PER_SUIT) {
        case 0:
            Card(Suit::HEARTS, value % 13);
        case 1:
            Card(Suit::DIAMONDS, value % 13);
        case 2:
            Card(Suit::SPADES, value % 13);
        case 3:
            Card(Suit::CLUBS, value % 13);
    }
}

Card::Card(Suit suit, uint8_t rank) {
    m_suit = suit;
    m_rank = rank;
    m_colour = (suit == Suit::HEARTS || suit == Suit::DIAMONDS) ? Colour::RED : Colour::BLACK;
}

uint8_t Card::getRank() const {
    //std::cout << m_rank << std::endl;
    return m_rank;
}

Suit Card::getSuit() const {
    return m_suit;
}

Colour Card::getColour() const {
    return m_colour;
}

bool Card::isFaceDown() const {
    return m_faceDown;
}

void Card::turnFaceUp() {
    m_faceDown = false;
}
