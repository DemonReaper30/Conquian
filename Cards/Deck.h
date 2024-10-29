#pragma once
#include <iostream>
#include <string>
#include <vector>
#include <algorithm>
#include <random>
#include "Card.h"
#include "Players.h"

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
		shuffleDeck();
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

	Card drawCard() {
		if (cards.empty()) {
			throw std::out_of_range("The deck is empty!");
		}
		Card drawnCard = cards.back();
		cards.pop_back();
		return drawnCard;
	}

	std::vector<Card> getCards() const {
		return cards;
	}

	std::vector<std::vector<Card>> dealToPlayers(int maxPlayers) {
		std::vector<std::vector<Card>> players(maxPlayers);
		int cardsPerPlayer = 9;
		for (int j = 0; j < maxPlayers; ++j) {
			for (int i = 0; i < cardsPerPlayer; ++i) {
				if (!cards.empty()) {
					players[j].push_back(drawCard());
				}
			}
		}
		return players;
	}

	void dumpCard(Player& currentPlayer, Card& drawnCard, std::vector<Player>& players, int& currentPlayerIndex) {
		// Display the player's hand
		std::cout << "Choose a card from your hand to dump:\n";
		currentPlayer.displayHand();

		// Get input from the player for which card to dump
		int cardIndex;
		std::cin >> cardIndex;

		if (cardIndex < 0 || cardIndex >= static_cast<int>(currentPlayer.getHand().size())) {
			std::cerr << "Invalid card index.\n";
			return;
		}

		// Remove the chosen card from the player's hand and set it as the new drawn card
		drawnCard = currentPlayer.giveCard(cardIndex);
		std::cout << "You dumped: ";
		drawnCard.display(); // Assuming Card has a display method

		//// Move to the next player
		//currentPlayerIndex = (currentPlayerIndex + 1) % players.size(); // Cycle through players
		//std::cout << "It's now Player " << currentPlayerIndex + 1 << "'s turn.\n";
	}
};