#pragma once
#include <iostream>
#include <string>
#include <vector>
#include <algorithm>
#include <random>


enum Suit {Spades, Hearts, Clubs, Diamonds };
enum Rank {Ace, Two, Three, Four, Five, Six, Seven, Jack, Queen, King };



class Card {
private:
	Suit suit;
	Rank rank;

	static const std::string suitStrings[];
	static const std::string rankStrings[];

public:
	Card(Suit s, Rank r): suit(s), rank(r) {}

	Suit getSuit() const {
		return suit;
	}

	Rank getRank() const {
		return rank;
	}

	void display() const{
		std::cout << rankStrings[rank] << " of " << suitStrings[suit] << "\n";
	}

	bool isValid() const {
		return (suit >= Spades && suit <= Diamonds) && (rank >= Ace && rank <= King);
	}
};

const std::string Card::suitStrings[] = { "Spades", "Hearts", "Clubs", "Diamonds" };
const std::string Card::rankStrings[] = { "Ace", "Two", "Three", "Four", "Five", "Six", "Seven", "Jack", "Queen", "King" };

class Deck {
private:
	std::vector<Card> cards;

public:
	Deck() {
		for (int s = Spades; s <= Diamonds; ++s) {
			for (int r = Ace; r <= King; ++r) {
				cards.push_back(Card(static_cast<Suit>(s), static_cast<Rank>(r)));
			}
		}
	}

	void displayAllCards() {
		for (const auto& card : cards) {
			card.display();
		}
	}

	void shuffleDeck() {
		std::random_device rd;
		std::mt19937 gen(rd());

		shuffle(cards.begin(), cards.end(), gen);
	}

	std::vector<Card> getCards() const {
		 return cards;
	}

	// this should be in Dealer.h
	std::vector<std::vector<Card>> dealToPlayers(int maxPlayers) {
		std::vector<std::vector<Card>> players(maxPlayers);

		int cardsPerPlayer = 9;
		for (int i = 0; i < cardsPerPlayer; ++i) {
			for (int j = 0; j < maxPlayers; ++j) {
				if (!cards.empty()) {
					players[j].push_back(cards.back());
					cards.pop_back();
				}
			}
		}
		return players;
	}
};