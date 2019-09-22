#include <cstdint>
#include <iostream>
const uint8_t CARDS_PER_SUIT = 13;

enum class Suit {
    HEARTS,
    DIAMONDS,
    SPADES,
    CLUBS
};

enum class Colour {
    RED,
    BLACK
};

class Card {
public:

    uint8_t getRank() const;
    Suit getSuit() const;
    Colour getColour() const;
    bool isFaceDown() const;

    Card();
    Card(uint8_t value);
    Card(Suit suit, uint8_t rank);

    void turnFaceUp();

private:

    uint8_t m_rank;
    Suit m_suit;
    Colour m_colour;
    bool m_faceDown;
};
