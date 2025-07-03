#include <iostream>
#include <unordered_map>
#include <algorithm>
#include "Deck.h"
#include "Players.h"

class Game {
private:
    Deck deck;
    std::vector<Player> players;
    const int maxPlayers = 3;

public:
    void playGame() {
        // Create players
        for (int i = 0; i < maxPlayers; ++i) {
            players.push_back(Player());
        }
        int currentPlayerIndex = 0;

        // distribute cards
        auto dealtCards = deck.dealToPlayers(maxPlayers);
        for (int i = 0; i < maxPlayers; ++i) {
            for (const auto& card : dealtCards[i]) {
                players[i].addCard(card);
            }
        }

        //// pass a card to next player
        //for (int i = 0; i < maxPlayers; i++) {
        //    Player& currentPlayer = players[i];
        //    Player& nextPlayer = players[(i + 1) % maxPlayers];

        //    currentPlayer.displayHand();

        //    int chosenIndex = 0;
        //    std::cout << "Player " << i << ", choose a card index to give to player " << (i) % maxPlayers << ": \n";
        //    std::cin >> chosenIndex;

        //    Card chosenCard = currentPlayer.giveCard(chosenIndex);
        //    if (chosenCard.isValid()) {
        //        nextPlayer.addCard(chosenCard);
        //        std::cout << "Player " << i << " Gave a card to player " << (i) % maxPlayers << ": " << "\n";
        //    }
        //}

        // Draw a card from the deck
        Card drawnCard;
        bool cardDrawn = false;

        do {
            // sugestion: check if not null
            if (!cardDrawn) {
                drawnCard = deck.drawCard();
                cardDrawn = true;
            }
            std::cout << "Drawn Card: ";
            drawnCard.display();

            std::cout << "\nPlayer " << (currentPlayerIndex + 1) << "'s " << "Turn: \n";
            players[currentPlayerIndex].organizeHand();
            players[currentPlayerIndex].displayHand();

            int choice;
            std::cout << "1. Meld with drawn card.\n2. Meld with cards from hand.\n3. Add a card to your meld\n4. Pass drawn card\n";
            std::cin >> choice;
            if (choice == 1) {
                createMeldWithDrawnCard(players[currentPlayerIndex], drawnCard);
                players[currentPlayerIndex].displayMelds();

                // player 3 dumps a card in index 1 rather 2 in the gameplay loop, becomes invalid choice as if choosing the from the initial choices then the turn goes backwards Ie. player 3 to 2.
                drawnCard = deck.dumpCard(players[currentPlayerIndex], players, currentPlayerIndex);
                cardDrawn = true; // make sure we don't overwrite it next turn
                // dumped card becomes the new drawn card and turn goes to the next player
                // move to the next player
                currentPlayerIndex = (currentPlayerIndex + 1) % players.size(); // Move to next player
            }
            if (choice == 2) {
                createMeldWithHand(players[currentPlayerIndex]);
                continue;
            }
            if (choice == 3) {
                addCardToMeld(players[currentPlayerIndex]);
                drawnCard = deck.dumpCard(players[currentPlayerIndex], players, currentPlayerIndex);
                cardDrawn = true; // make sure we don't overwrite it next turn
                // dumped card becomes the new drawn card and turn goes to the next player
                // move to the next player
                currentPlayerIndex = (currentPlayerIndex + 1) % players.size(); // Move to next player
            }
            else if (choice == 4) {
                std::cout << "You passed on the drawn card.\n";
				bool takenCard = passMotion(currentPlayerIndex, players, drawnCard);

                if (!takenCard) {
                    // If no one took the card, we continue with the same drawn card
                    cardDrawn = false; // Reset cardDrawn since a new card is drawn
                }
                else {
                    // If the card was taken, we continue to the next player
                    takenCard = true; // Reset cardDrawn since a new card is drawn
                    std::cout << "Card has been taken, A new card is drawn\n";
                }
            }
            else {
				std::cerr << "Invalid choice. Try again.\n";
				cardDrawn = true;
                currentPlayerIndex = (currentPlayerIndex + 1) % maxPlayers; // Move to the next player
                continue;
            }

            

        } while (!players[currentPlayerIndex].winCondition());
        // Announce the winner
        std::cout << "Player " << currentPlayerIndex << " is the Winner!\n";
    }

    void createMeldWithHand(Player& player) {
        // Loop to prompt the player to choose 3 cards to add to a meld
        bool validMeld = false;
        while (!validMeld) {
            player.displayHand();
            std::cout << "Choose 3 cards to add to a meld or type -1 to quit: \n";
            std::vector<int> selectedIndices;
            int handIndex;
            bool quitAttempt = false;

            for (int i = 0; i < 3; ++i) {
                std::cout << "Card " << i + 1 << ": \n";
                std::cin >> handIndex;
                if (handIndex == -1) {
                    quitAttempt = true;
                    break;
                }
                if (handIndex >= 1 && handIndex < static_cast<int>(player.getHand().size() + 1)) {
                    selectedIndices.push_back(handIndex - 1);
                }
                else {
                    std::cerr << "Invalid index. Try again.\n";
                    --i; // Ask for the same card again
                }
            }

            if (quitAttempt) {
                break;
            }

            std::vector<Card> newMeld = player.removeCardsForMeld(selectedIndices);

            if (player.isValidMeld(newMeld)) {
                player.addMeld(newMeld);
                validMeld = true;
                std::cout << "Meld added successfully.\n\n";
            }
            else {
                std::cout << "Invalid meld. Try again or type -1 to quit.\n";
                // Restore the cards to the player's hand if the meld is invalid
                for (const auto& card : newMeld) {
                    player.addCard(card);
                }
            }
        }
    }

    void createMeldWithDrawnCard(Player& player, const Card& drawnCard) {
        // Loop to prompt the player to choose 2 cards from hand and use the drawn card to create a meld
        bool validMeld = false;

		std::cout << "Drawn card: ";
		drawnCard.display();
		std::cout << std::endl;

        while (!validMeld) {
            player.displayHand();
            std::cout << "Choose 2 cards from your hand to add to the meld with the drawn card or type -1 to quit: \n";
            std::vector<int> selectedIndices;
            int handIndex;
            bool quitAttempt = false;

            for (int i = 0; i < 2; ++i) {
                std::cout << "Card " << i + 1 << ": \n";
                std::cin >> handIndex;
                if (handIndex == -1) {
                    quitAttempt = true;
                    break;
                }

                handIndex -= 1;

                int handSize = player.getHand().size();

                if (handIndex >= 0 && handIndex < handSize) {
                    selectedIndices.push_back(handIndex);
                }
                else {
                    std::cerr << "Invalid index. Try again.\n";
                    --i; // Ask for the same card again
                }
            }

            if (quitAttempt) {
                break;
            }

            std::vector<Card> newMeld = player.removeCardsForMeld(selectedIndices);
            newMeld.push_back(drawnCard); // Add the drawn card to the meld

            if (player.isValidMeld(newMeld)) {
                player.addMeld(newMeld);
                validMeld = true;
                std::cout << "Meld added successfully.\n";
            }
            else {
                std::cout << "Invalid meld. Try again or type -1 to quit.\n";
                // Restore the cards to the player's hand if the meld is invalid
                for (const auto& card : newMeld) {
                    if (!(card == drawnCard)) { // Don't restore the drawn card
                        player.addCard(card);
                    }
                }
            }
        }
    }

    void addCardToMeld(Player& player) {
        // Step 1: Display player's melds
        if (player.getMelds().empty()) {
            std::cout << "You have no melds to add a card to.\n";
            return;
        }

        std::cout << "Choose one of your melds to add a card to:\n";
        player.displayMelds();  // Function that shows the player's existing melds

        // Step 2: Let the player choose a meld
        int meldIndex;
        std::cout << "Meld index (1 to " << player.getMelds().size() << "): ";
        std::cin >> meldIndex;
        if (meldIndex < 1 || meldIndex > static_cast<int>(player.getMelds().size())) {
            std::cerr << "Invalid meld index.\n";
            return;
        }

        // Step 3: Let the player choose a card from their hand to add
        std::cout << "Choose a card from your hand to add to the meld:\n";
        player.displayHand();
        int cardIndex;
        std::cin >> cardIndex;
        if (cardIndex < 0 || cardIndex >= static_cast<int>(player.getHand().size())) {
            std::cerr << "Invalid card index.\n";
            return;
        }

        // Step 4: Retrieve the card and the meld
        Card selectedCard = player.getHand()[cardIndex];
        std::vector<Card>& selectedMeld = player.getMelds()[meldIndex - 1]; // Get the selected meld

        // Step 5: Add the card to the selected meld
        selectedMeld.push_back(selectedCard);

        // Step 6: Validate the updated meld
        if (player.isValidMeld(selectedMeld)) {
            player.removeCard(cardIndex);  // Remove the card from the player's hand
            std::cout << "Card added to the meld successfully.\n";
        }
        else {
            // Step 7: If the meld is invalid, remove the card from the meld
            std::cerr << "Invalid meld. The card cannot be added.\n";
            selectedMeld.pop_back();  // Undo the addition if it's not valid
        }
    }

    bool passMotion(int& playerIndex, std::vector<Player>& players, Card& drawnCard, int passCount = 1) {
        int numPlayers = players.size();
		int originalIndex = playerIndex; // Store the original index for reference

        // Only allow passing to the next 2 players
        if (passCount > 2) {
            std::cout << "All players have passed. Next turn begins.\n\n";
            // Set to the player AFTER the original player
            playerIndex = (originalIndex + 1) % numPlayers;
            return false;
        }

        // Move to the next player
        playerIndex = (playerIndex + 1) % numPlayers;
        Player& currentPlayer = players[playerIndex];

        std::cout << "\nDrawn card: "; 
        drawnCard.display();
        currentPlayer.organizeHand();
        currentPlayer.displayHand();

        std::cout << "Player " << (playerIndex + 1) << ", do you want the drawn card?\n";
        std::cout << "1. Want\n2. Pass\n";
        int choice;
        std::cin >> choice;

        if (choice == 1) {
            // Player wants the card, meld with it
            createMeldWithDrawnCard(currentPlayer, drawnCard);
            currentPlayer.displayMelds();
            // After a player takes the card they drop another card, then turn ends
            drawnCard = deck.dumpCard(currentPlayer, players, playerIndex);
            // move to the next player
            playerIndex = (playerIndex + 1) % players.size(); // cycle through players
            return true;
        }
        else if (choice == 2) {
            // Continue passing to the next player, but only up to 2 passes
            return passMotion(playerIndex, players, drawnCard, passCount + 1);
        }
        else {
			// Input validation does not work here
            std::cerr << "Invalid choice. Try again.\n";
            // Do not increment passCount for invalid input
            return passMotion(playerIndex, players, drawnCard, passCount);
        }
    }
};