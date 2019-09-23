#ifndef CARD_H
#define CARD_H

#include <iostream>
#include <string>

const int CARDS_PER_SUIT = 13;

enum class Suit {
    HEARTS = 0,
    DIAMONDS = 1,
    SPADES = 2,
    CLUBS = 3
};

enum class Colour {
    RED,
    BLACK
};

class Card {
public:

    int getRank() const;
    Suit getSuit() const;
    Colour getColour() const;
    bool isFaceDown() const;

    Card();
    Card(int value);
    Card(Suit suit, int rank);

    void turnFaceDown();
    void turnFaceUp();

    std::string toString() const;

private:

    int m_rank;
    Suit m_suit;
    Colour m_colour;
    bool m_faceDown;
};

#endif
