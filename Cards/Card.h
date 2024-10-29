#pragma once
#include <iostream>
#include <string>

enum Suit { Spades, Hearts, Clubs, Diamonds };
enum Rank { Ace = 0, Two = 1, Three = 2, Four = 3, Five = 4, Six = 5, Seven = 6, Jack = 7, Queen = 8, King = 9 };

class Card {
private:
	Suit suit;
	Rank rank;

	static const std::string suitStrings[];
	static const std::string rankStrings[];

public:
	Card() : suit(Spades), rank(Ace) {} // Default constructer
	Card(Suit s, Rank r) : suit(s), rank(r) {}

	Suit getSuit() const {
		return suit;
	}

	Rank getRank() const {
		return rank;
	}

	void display() const {
		std::cout << rankStrings[rank] << " of " << suitStrings[suit] << "\n";
	}

	bool isValid() const {
		return (suit >= Spades && suit <= Diamonds) && (rank >= Ace && rank <= King);
	}

	bool operator==(const Card& other) const {
		return suit == other.suit && rank == other.rank;
	}
};