#include "card.h"

Card::Card() {
    Card(Suit::HEARTS, 0);
}

Card::Card(int value) {

    switch (value / CARDS_PER_SUIT) {
        case 0:
            // Card(Suit::HEARTS, value % 13);
            m_suit = Suit::HEARTS;
        case 1:
            // Card(Suit::DIAMONDS, value % 13);
            m_suit = Suit::DIAMONDS;
        case 2:
            // Card(Suit::SPADES, value % 13);
            m_suit = Suit::SPADES;
        case 3:
            // Card(Suit::CLUBS, value % 13);
            m_suit = Suit::CLUBS;
    }
    m_rank = value % 13;
    m_colour = (m_suit == Suit::HEARTS || m_suit == Suit::DIAMONDS) ? Colour::RED : Colour::BLACK;
}

Card::Card(Suit suit, int rank) : m_suit(suit), m_rank(rank), m_colour((suit == Suit::HEARTS || suit == Suit::DIAMONDS) ? Colour::RED : Colour::BLACK) {
    m_suit = suit;
    m_rank = rank;
    m_colour = (suit == Suit::HEARTS || suit == Suit::DIAMONDS) ? Colour::RED : Colour::BLACK;
}

int Card::getRank() const {
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

void Card::turnFaceDown() {
    m_faceDown = true;
}


void Card::turnFaceUp() {
    m_faceDown = false;
}
