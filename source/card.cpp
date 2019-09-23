#include "card.h"

Card::Card() {
    m_suit = Suit::HEARTS;
    m_rank = 1;
    m_faceDown = false;
    m_colour = (m_suit == Suit::HEARTS || m_suit == Suit::DIAMONDS) ? Colour::RED : Colour::BLACK;
}

Card::Card(int value) {

    switch (value / CARDS_PER_SUIT) {
        case 0:
            // Card(Suit::HEARTS, value % 13);
            m_suit = Suit::HEARTS;
            break;
        case 1:
            // Card(Suit::DIAMONDS, value % 13);
            m_suit = Suit::DIAMONDS;
            break;
        case 2:
            // Card(Suit::SPADES, value % 13);
            m_suit = Suit::SPADES;
            break;
        case 3:
            // Card(Suit::CLUBS, value % 13);
            m_suit = Suit::CLUBS;
            break;
    }
    m_rank = (value % 13) + 1;
    m_faceDown = false;
    m_colour = (m_suit == Suit::HEARTS || m_suit == Suit::DIAMONDS) ? Colour::RED : Colour::BLACK;
}

Card::Card(Suit suit, int rank) : m_suit(suit), m_rank(rank), m_colour((suit == Suit::HEARTS || suit == Suit::DIAMONDS) ? Colour::RED : Colour::BLACK) {
    m_suit = suit;
    m_rank = rank;
    m_faceDown = false;
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

std::string Card::toString() const {

    if (m_faceDown) {
        return "?";
    }

    std::string cardStr;

    switch (m_rank) {
        case 1:
            cardStr.push_back('A');
            break;
        case 11:
            cardStr.push_back('J');
            break;
        case 12:
            cardStr.push_back('Q');
            break;
        case 13:
            cardStr.push_back('K');
            break;
        default:
            cardStr += std::to_string(m_rank);
    }

    switch (m_suit) {
        case Suit::HEARTS:
            cardStr.push_back('H');
            break;
        case Suit::DIAMONDS:
            cardStr.push_back('D');
            break;
        case Suit::SPADES:
            cardStr.push_back('S');
            break;
        case Suit::CLUBS:
            cardStr.push_back('C');
            break;
    }

    return cardStr;
}
