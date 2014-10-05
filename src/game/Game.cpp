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
// Filename: Game.cpp
// Description: The main game instance. It will deal with setting up the cards and players
// Author: Danny Sok
// Date Created: 10/04/2013
// Date Last Modified: 26/06/2013

#include "Game.h"

// ----------------------------------------
// Game
// 
// 
// Input:
//  
// Output:
//  
// Conditions:
//  

// ----------------------------------------
// ~Game
// 
// 
// Input:
//  
// Output:
//  
// Conditions:
//  
Game::~Game()
{
	//delete deck;
	//delete player;

}

// ----------------------------------------
// setup
// 
// Setup the current game
// Input:
//  
// Output:
//  
// Conditions:
//  
void Game::setup()
{
	m_deck = Deck();
	m_deck.setup();
}

// ----------------------------------------
// setup
// 
// Setup the current game
// Input:
//  
// Output:
//  
// Conditions:
//  
void Game::setup(int Card_W, int Card_H)
{
	m_deck.setup(Card_W, Card_H);
}


void Game::start(string name)
{
	start(name, 4, AI_EASY);
}
// ----------------------------------------
// start
// 
// Starts the current game by creating the players and dealing the cards
// Input:
//  
// Output:
//  
// Conditions:
//  
void Game::start(string name, int numPlayers)
{
	start(name, numPlayers, AI_HARD);
}

void Game::start(string name, int numPlayers, PLAYER_TYPES ai)
{
	m_fieldState = NoHand;
	//for (int i = 1; i < numPlayers; i++) // Player vector

	// Uncomment below to have player(0) as HUMAN
	createPlayers(name, HUMAN, 0);

	// Change i = 1 to have player(0) as HUMAN
	for (int i = 1; i < numPlayers; i++) // AI Vector
	{
		createPlayers(makeAIName(), ai, i);
	}

	deal();
}

void Game::start(string name, std::vector<int> aiAmount)
{
	m_fieldState = NoHand;

	//createPlayers(name, HUMAN, 0);
	//createPlayers(makeAIName(), AI_HARD, 0);

	// Create Easy AI if any
	for (int i = 0; i < aiAmount[0]; i++)
	{
		createPlayers(makeAIName(), AI_EASY, m_players.size());
	}

	// Create Medium AI if any
	for (int i = 0; i < aiAmount[1]; i++)
	{
		createPlayers(makeAIName(), AI_MEDIUM, m_players.size());
	}

	// Create Hard AI if any
	for (int i = 0; i < aiAmount[2]; i++)
	{
		createPlayers(makeAIName(), AI_HARD, m_players.size());
	}

	deal();
}

void Game::createPlayers(string name, PLAYER_TYPES type, int pos)
{
	PlayerFactory *p = PlayerFactory::CreateFactory(type);
	//p->getPlayer()->setup(name, type);
	IPlayerPtr sptr = (IPlayerPtr)p->getPlayer();
	m_players.push_back(sptr);
	//m_players[m_players.size() - 1]->setup(name, type);
	m_players[m_players.size() - 1]->setup(name, type, pos);
}

void Game::finish()
{
	
}

// ----------------------------------------
// shuffle
// 
// Shuffle the deck
// Input:
//  
// Output:
//  
// Conditions:
//  
void Game::shuffle()
{
	m_deck.shuffle();
}

// ----------------------------------------
// deal
// 
// Deal the cards to the current players
// Input:
//  
// Output:
//  
// Conditions:
//  
void Game::deal()
{
	/*for (int i = 0; i < iPlayerCount; i++)
	{
		//for(std::list<Card>::iterator it = deck->getDeck().begin(); it != deck->getDeck().end(); it++)
		for(std::list<Card>::iterator it = deck->getDeck().begin(); it++)
		{
			player[i]->setHand(*it);
		}
	}*/

	//for (int i = 0; i < player.size(); i++) // Player vec
	for (int i = 0; i < m_players.size(); i++)
	{
		if (m_deck.getSize() >= SIZE_HAND)
		{
			//deck.deal(&this->player[i]);
			//player[i].sortCards();
			m_deck.deal(this->m_players[i]);
			m_players[i]->sortCards();
			//m_players[i]->setupCardImg();
		}
	}
}

void Game::addAIPlayers(int number)
{
	for (int i = 0; i < number; i++)
	{
		//player[i] = Player("AI" + i);
		createPlayers(makeAIName(), AI_EASY, i);
	}
}

// ----------------------------------------
// 
// 
// 
// Input:
//  
// Output:
//  
// Conditions:
//  
IPlayerPtr Game::getPlayer(int number)
{
	return m_players[number];
}

std::vector< IPlayerPtr > Game::getPlayers()
{
	return m_players;
}

// ----------------------------------------
// 
// 
// 
// Input:
//  
// Output:
//  
// Conditions:
//  
int Game::getPlayerCount()
{
	//return player.size();
	return m_players.size();
}

int Game::getAICount()
{
	return m_players.size() - 1;
}

// ----------------------------------------
// makeAIName
// 
// Returns a random name
// Input:
//  
// Output:
//  
// Conditions:
//  
std::string Game::makeAIName()
{
	std::vector<std::string> AINames;
	AINames.push_back("Luffy");
	AINames.push_back("Chopper");
	AINames.push_back("Franky");
	AINames.push_back("Sanji");
	AINames.push_back("Zoro");
	AINames.push_back("Kid");
	AINames.push_back("Crocodile");
	AINames.push_back("Nami");
	AINames.push_back("Usopp");
	AINames.push_back("Brooke");
	AINames.push_back("Robin");
	AINames.push_back("Sunny Go");

	std::random_shuffle (AINames.begin(), AINames.end());
	return AINames[rand() % AINames.size()];
	
}

// ----------------------------------------
// clear
// 
// Clears the deck and players to allow for a new setup reusing the class
// Input:
//  
// Output:
//  
// Conditions:
//  
void Game::clear()
{
	m_deck.clear();

	// Make sure all player threads are terminated first!
	/*for (int i = 0; i < m_players.size(); i++)
	{
		m_players[i]->stopThread();
	}*/

	m_players.clear();
	m_players.shrink_to_fit();
}

void Game::updateWinner(int pNum)
{
	m_players[pNum]->incWinCount();
}

int Game::getPassCount()
{
	return m_passCount;
}

void Game::setPassCount(int c)
{
	m_passCount = c;
}

void Game::setLastPlayed(std::vector<Card> c) 
{
	m_lastPlayed.clear(); 
	m_lastPlayed.insert(m_lastPlayed.end(), c.begin(), c.end()); 
}

std::vector<Card> Game::getLastPlayed()
{
	 return m_lastPlayed;
}