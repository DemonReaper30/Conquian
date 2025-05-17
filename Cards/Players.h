#pragma once
#include <vector>
#include "Card.h"

class Deck;

class Player {
private:
	std::vector<Card> hand;
	std::vector<std::vector<Card>> melds;

public:
	void addCard(const Card& card) {
		hand.push_back(card);
	}

	const std::vector<Card>& getHand() const {
		return hand;
	}

	void displayHand() const {
		std::cout << "\nHand: \n";
		for (size_t i = 0; i < hand.size(); ++i) {
			std::cout << "[" << i + 1 << "] "; // Display the index
			hand[i].display(); // Display the card
		}
		std::cout << "\n";
	}

	Card giveCard(int index) {
		if (index >= 0 && index < hand.size()) {
			Card chosenCard = hand[index];
			hand.erase(hand.begin() + index);
			return chosenCard;
		}
		else {
			std::cerr << "Invalid index. No card given" << "\n";
			return Card();
		}
	}

	void organizeHand() {
		std::sort(hand.begin(), hand.end(), [](const Card& a, const Card& b) {
			if (a.getSuit() == b.getSuit()) {
				return a.getRank() < b.getRank(); // Sort by rank if suits are the same
			}
			return a.getSuit() < b.getSuit(); // Sort by suit first
			});
	}

	void addMeld(const std::vector<Card>& newMeld) {
		melds.push_back(newMeld);
	}

	// Method to get the player's melds
	std::vector<std::vector<Card>>& getMelds() {
		return melds;
	}

	bool addCardToMeld(int meldIndex, int cardIndex) {
		if (meldIndex >= 0 && meldIndex < static_cast<int>(melds.size()) &&
			cardIndex >= 0 && cardIndex < static_cast<int>(hand.size())) {
			melds[meldIndex].push_back(hand[cardIndex]);
			hand.erase(hand.begin() + cardIndex);
			return true;
		}
		else {
			std::cerr << "Invalid meld or card index." << std::endl;
			return false;
		}
	}

	std::vector<Card> removeCardsForMeld(const std::vector<int>& indices) {
		std::vector<Card> selectedCards;
		std::vector<Card> newHand;
		for (size_t i = 0; i < hand.size(); ++i) {
			if (std::find(indices.begin(), indices.end(), i) != indices.end()) {
				selectedCards.push_back(hand[i]);
			}
			else {
				newHand.push_back(hand[i]);
			}
		}
		hand = newHand;
		return selectedCards;
	}

	void removeCard(int index) {
		if (index >= 0 && index < static_cast<int>(hand.size())) {
			hand.erase(hand.begin() + index);
		}
		else {
			std::cerr << "Invalid card index.\n";
		}
	}


	// Method to display the player's current melds
	void displayMelds() const {
		std::cout << "Player's melds:" << std::endl;
		for (size_t i = 0; i < melds.size(); ++i) {
			std::cout << "Meld [" << i + 1 << "]: \n";
			for (const auto& card : melds[i]) {
				card.display();
			}
			std::cout << std::endl;
		}
	}

	bool isValidMeld(const std::vector<Card>& meld) const {
		if (meld.size() < 3) {
			return false; // A valid meld must have at least 3 cards
		}

		// Check if all cards have the same rank (e.g., three Kings, four Kings, etc.)
		bool sameRank = std::all_of(meld.begin(), meld.end(), [&](const Card& card) {
			return card.getRank() == meld[0].getRank();
			});

		if (sameRank) {
			return true;
		}

		// Collect and sort the ranks to check for consecutive sequences
		std::vector<int> meldRanks;
		for (const auto& card : meld) {
			meldRanks.push_back(card.getRank());
		}

		std::sort(meldRanks.begin(), meldRanks.end());

		// Check for a consecutive sequence
		for (size_t i = 1; i < meldRanks.size(); ++i) {
			if (meldRanks[i] != meldRanks[i - 1] + 1) {
				return false; // Not a valid consecutive sequence
			}
		}

		return true; // Valid consecutive sequence or same rank meld
	}

	bool winCondition() const {
		if (!hand.empty()) {
			return false;
		}

		int totalMeldCards = 0;
		for (const auto& meld : melds) {
			totalMeldCards += meld.size();
		}

		return totalMeldCards == 9;
	}
};