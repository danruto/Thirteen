/*
Copyright(C) 2013  Danny Sok <danny.sok@outlook.com>

This program is free software : you can redistribute it and / or modify
it under the terms of the GNU General Public License as published by
the Free Software Foundation, either version 3 of the License, or
(at your option) any later version.

This program is distributed in the hope that it will be useful,
but WITHOUT ANY WARRANTY; without even the implied warranty of
MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.See the
GNU General Public License for more details.

You should have received a copy of the GNU General Public License
along with this program.If not, see <http://www.gnu.org/licenses/>.
*/
// ----------------------------------------
// Filename: Deck.cpp
// Description: Represents a deck of cards and the modification of the deck
// Author: Danny Sok
// Date Created: 10/04/2013
// Date Last Modified: 12/04/2013

#include <string>
#include <list>
#include "Deck.h"

using namespace std;

// ----------------------------------------
// Deck (constructor)
// 
// Setup the deck by creating the cards and shuffling the cards
// Input:
//  
// Output:
//  
// Conditions:
//  
Deck::Deck()
{
	createCards();
	shuffle();
}

// ----------------------------------------
// Deck (deconstructor)
// 
// Deletes the deck
// Input:
//  
// Output:
//  
// Conditions:
//  
Deck::~Deck()
{
	//delete cards;
}

// ----------------------------------------
// setup
// 
// Setup the deck by creating and shuffling the cards
// Input:
//  
// Output:
//  
// Conditions:
//  
void Deck::setup()
{
	createCards();
	shuffle();
}

void Deck::setup(bool b)
{
	createCards(b);
	shuffle();
}

// ----------------------------------------
// setup
// 
// Setup the deck by creating and shuffling the cards
// Input:
//  
// Output:
//  
// Conditions:
//  
void Deck::setup(int Card_W, int Card_H)
{
	createCards(Card_W, Card_H);
	shuffle();
}

// ----------------------------------------
// createCards
// 
// Creates the 52 cards in the deck
// Input:
//  
// Output:
//  
// Conditions:
//  
void Deck::createCards()
{
	
	/* for use with list
	for (int i = 0; i < NUM_SUITES; i++)
	{
		for (int j = 0; j < SIZE_HAND; j++)
		{
			//http://stackoverflow.com/questions/2654556/error-c2440-initializing-cannot-convert-from-classname-to-classname
			Card c(i,j); // create a class instance (object) on the stack
			cards.push_back(c);
		}
	}*/
	
	/*int a;
	for (int i = 0; i < NUM_SUITES; i++)
	{
		for (int j = 0; j < SIZE_HAND; j++)
		{
			cards[a++] = new Card (j, i);
		}
	}*/

	// Ensure the vector is clear due to a reset or any other issue
	m_cards.clear();
	
	for (int i = 0; i < NUM_SUITES; i++)
	{
		for (int j = 0; j < SIZE_HAND; j++)
		{
			Card c(j,i); // create a class instance (object) on the stack
			m_cards.push_back(c);
		}
	}
}

void Deck::createCards(bool neededByAI)
{
	m_cards.clear();
	
	for (int i = 0; i < NUM_SUITES; i++)
	{
		for (int j = 0; j < SIZE_HAND; j++)
		{
			Card c(j,i); // create a class instance (object) on the stack
			c.setNeededByAI(true);
			c.setup();
			m_cards.push_back(c);
		}
	}
}

// ----------------------------------------
// createCards
// 
// Creates the 52 cards in the deck
// Input:
//  
// Output:
//  
// Conditions:
//  
void Deck::createCards(int Card_W, int Card_H)
{
	
	/* for use with list
	for (int i = 0; i < NUM_SUITES; i++)
	{
		for (int j = 0; j < SIZE_HAND; j++)
		{
			//http://stackoverflow.com/questions/2654556/error-c2440-initializing-cannot-convert-from-classname-to-classname
			Card c(i,j); // create a class instance (object) on the stack
			cards.push_back(c);
		}
	}*/
	
	/*int a;
	for (int i = 0; i < NUM_SUITES; i++)
	{
		for (int j = 0; j < SIZE_HAND; j++)
		{
			cards[a++] = new Card (j, i);
		}
	}*/

	// Ensure the vector is clear due to a reset or any other issue
	m_cards.clear();
	
	for (int i = 0; i < NUM_SUITES; i++)
	{
		for (int j = 0; j < SIZE_HAND; j++)
		{
			Card c(j,i, Card_W, Card_H); // create a class instance (object) on the stack
			m_cards.push_back(c);
		}
	}
}

// ----------------------------------------
// shuffle
// 
// Shuffle the deck randomly
// Input:
//  
// Output:
//  
// Conditions:
//  
void Deck::shuffle()
{
	std::random_shuffle (m_cards.begin(), m_cards.end());
}

// ----------------------------------------
// deal
// 
// Deal the cards to the player as long as there are cards available
// Input:
//  p is a pointer to a Player
// Output:
//  
// Conditions:
//  
void Deck::deal(shared_ptr<IPlayer> p)
{
	std::vector<Card> hand;
	for (int i = 0; i < SIZE_HAND; i++)
	{
		hand.push_back(m_cards[i]); 
	}
	//p->getCards() = hand;
	p->setCards(hand);

	// Delete the used cards as they have been given. We don't want to give them out again
	m_cards.erase(m_cards.begin(), m_cards.begin() + SIZE_HAND);
	//delete hand;
}

// ----------------------------------------
// deal
// 
// Deal the cards to the AI as long as there are cards available
// Input:
//  p is a pointer to an AI
// Output:
//  
// Conditions:
//  
void Deck::deal(AI *p)
{
	std::vector<Card> hand;
	for (int i = 0; i < SIZE_HAND; i++)
	{
		hand.push_back(m_cards[i]); 
	}
	//p->getCards() = hand;
	p->setCards(hand);

	// Delete the used cards as they have been given. We don't want to give them out again
	m_cards.erase(m_cards.begin(), m_cards.begin() + SIZE_HAND);
	//delete hand;
}

// ----------------------------------------
// getSize
// 
// Returns the current size of the deck
// Input:
//  
// Output:
//  size of the deck
// Conditions:
//  
int Deck::getSize()
{
	return m_cards.size();
}

// ----------------------------------------
// clear
// 
// Clears the deck
// Input:
//  
// Output:
//  
// Conditions:
//  
void Deck::clear()
{
	m_cards.clear();
	m_cards.shrink_to_fit();
}

// ----------------------------------------
// getDeck
// 
// Returns the deck
// Input:
//  
// Output:
//  
// Conditions:
// 
std::vector<Card> Deck::getDeck()
{
	return m_cards;
}