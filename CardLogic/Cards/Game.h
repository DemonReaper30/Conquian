#pragma once
#include <unordered_map>;
#include <algorithm>;
#include "Deck.h"
#include "Players.h"
#include "Dealer.h"

class Game {
private:
	Deck deck;
	std::vector<Player> players;
	Dealer dealer;
	const int maxPlayers = 1;

public:
	void playGame() {
		// Create players
		for (int i = 0; i < maxPlayers; ++i) {
			players.push_back(Player());
		}

		// distribute cards
		std::vector<Card> remainingCards = dealer.distributeCardsToPlayers(players);
		for (const auto& card : remainingCards) {
			dealer.addCardToDealer(card);
		}

		players[0].displayHand();

		std::cout << "choose 3 cards to add to a meld: \n";
		std::vector<int> selectedIndices;
		int handIndex;
		
		for (int i = 0; i < 3; ++i) {
			std::cout << "Card " << i + 1 << ": \n";
			std::cin >> handIndex;
			if (handIndex >= 0 && handIndex < static_cast<int>(players[0].getHand().size())) {
				selectedIndices.push_back(handIndex);
			}
			else {
				std::cerr << "Invalid index. Try again.\n";
				--i; // Ask for the same card again
			}
		}

		std::vector<Card> newMeld = players[0].removeCardsForMeld(selectedIndices);
		players[0].addMeld(newMeld);

		players[0].displayMelds();
		players[0].displayHand();

		// pass a card to the next player
		/*for (int i = 0; i < maxPlayers; i++) {
			Player& currentPlayer = players[i];
			Player& nextPlayer = players[(i + 1) % maxPlayers];

			currentPlayer.displayHand();

			int chosenIndex = -1;
			std::cout << "Player " << i << ", choose a card index to give to player " << (i + 1) % maxPlayers << ": \n";
			std::cin >> chosenIndex;

			Card chosenCard = currentPlayer.giveCard(chosenIndex);
			if (chosenCard.isValid()) {
				nextPlayer.addCard(chosenCard);
				std::cout << "Player "<< i <<" Gave a card to player "<< (i + 1) % maxPlayers << ": " << "\n";
			}
		}*/

		// Round starts
	}
};