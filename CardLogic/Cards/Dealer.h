#pragma once
#include "Deck.h"
#include "Players.h"

// this class is unnecessary
// most methods can go to Deck.h

class Dealer {
private:
    Deck deck;
    std::vector<Card> DealerCards;
    std::vector<Card> Stockpile;

public:
    Dealer() {
        deck.shuffleDeck(); // Shuffle the deck when initializing dealer
    }

    // Distribute cards to players and return remaining cards on the dealer
    std::vector<Card> distributeCardsToPlayers(std::vector<Player>& players) {
        const int maxPlayers = players.size();
        auto dealtCards = deck.dealToPlayers(maxPlayers);

        for (int i = 0; i < maxPlayers; ++i) {
            for (const auto& card : dealtCards[i]) {
                players[i].addCard(card);
            }
        }
        
        return deck.getCards();
    }

    void addCardToDealer(const Card& card) {
        DealerCards.push_back(card);
    }

    Card layCardToStockpile() {
        if (!DealerCards.empty()) {
            Card topCard = DealerCards.back();
            DealerCards.pop_back();
            Stockpile.push_back(topCard);
        }
        else {
            std::cerr << "No cards in dealer's hand to place on board.";
            return Card(Spades, Ace);
        }
    }

    Card viewTopCard() const {
        if (!Stockpile.empty()) {
            return Stockpile.back();
        }
        else {
            return Card(Spades, Ace); // if stockpile is empty
        }
    }

    Card drawCardFromStockpile() {
        if (!Stockpile.empty()) {
            Card topCard = Stockpile.back();
            Stockpile.pop_back();
            return topCard;
        }
        else {
            std::cerr << "No cards in stockpile." << std::endl;
        }
    }
};