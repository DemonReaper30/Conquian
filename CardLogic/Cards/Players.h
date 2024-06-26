#pragma once
#include <vector>
#include "Deck.h"

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
		std::cout << "Players hand: \n";
		for (size_t i = 0; i < hand.size(); ++i) {
			std::cout << "[" << i << "] "; // Display the index
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
			return Card(Spades, Ace);
		}
	}

	void addMeld(const std::vector<Card>& newMeld) {
		melds.push_back(newMeld);
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

	// Method to display the player's current melds
	void displayMelds() const {
		std::cout << "Player's melds:" << std::endl;
		for (size_t i = 0; i < melds.size(); ++i) {
			std::cout << "Meld [" << i << "]: \n";
			for (const auto& card : melds[i]) {
				card.display();
			}
			std::cout << std::endl;
		}
	}
};