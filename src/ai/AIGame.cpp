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
// Filename: AIGame.cpp
// Description: Represents the state of the game from the AI perspective
// Author:		Danny Sok
// Date Created: 9/08/2013
// Date Last Modified: 27/09/2013
#include "AIGame.h"
#include "..\game\Game.h"

AIGame::~AIGame()
{
	//destroy();
}

void AIGame::init()
{
	m_hands.reserve(4);
	m_deck.reserve(52);
}

void AIGame::init(std::vector< std::vector<Card> > hands, int playerPos, int passCount, bool isFreeReign)
{
	m_hands = hands;
	m_originalPlayerPos = playerPos;
	m_playerPos = playerPos;
	m_passCount = passCount;
	m_playerCount = Game::getInstance().getPlayerCount();
	m_isGuessMode = false;
	m_isFreeReign = isFreeReign;
	m_turnCount++;
	m_depthCount = 1;
}

void AIGame::init(std::vector<Card> deck, std::vector<Card> playerHand, int playerPos, int passCount, bool isFreeReign)
{
	m_deck = deck;
	m_playerPos = playerPos;
	m_originalPlayerPos = playerPos;
	m_passCount = passCount;
	m_playerCount = Game::getInstance().getPlayerCount();
	m_isGuessMode = true;
	m_isFreeReign = isFreeReign;
	m_depthCount = 1;
	
	m_turnCount++;
	
	m_playerHand.insert(m_playerHand.end(), playerHand.begin(), playerHand.end());

	deal();
}

void AIGame::initAddedHands(int playerPos, int passCount, int originalPlayerPos, bool guessMode, bool isFreeReign, int depth)
{
	m_playerPos = playerPos;
	m_originalPlayerPos = originalPlayerPos;
	m_playerCount = Game::getInstance().getPlayerCount();
	m_passCount = passCount % m_playerCount;
	m_isGuessMode = guessMode; 
	//m_isGuessMode = guessMode;
	m_isFreeReign = isFreeReign;
	m_depthCount = depth+1; // This will be needed to ensure we don't use too much memory. Just say the game is over.
	m_turnCount++;
}

void AIGame::destroy()
{
	m_hands.clear(); m_hands.shrink_to_fit();
	m_deck.clear(); m_deck.shrink_to_fit();

}

void AIGame::setGuessMode(bool b)
{
	m_isGuessMode = b;
}

void AIGame::deal()
{
	int size = 0;
	for (int i = 0; i < m_playerCount; i++)
	{
		if (i != m_playerPos)
		{
			std::vector<Card> hand;
			if (Game::getInstance().getPlayer(i)->lock())
			{
				size = Game::getInstance().getPlayer(i)->getCards().size();
				Game::getInstance().getPlayer(i)->unlock();
			}
			for (int j = 0; j < size; j++)
			{
				m_deck[j].clearImgs();
				hand.push_back(m_deck[j]); 
			}
			m_hands.push_back(sortCards(hand));
		}
		else
		{
			//m_hands[m_playerPos].insert(m_hands[m_playerPos].end(), m_playerHand.begin(), m_playerHand.end());
			m_hands.push_back(m_playerHand);
		}
	}
}

void AIGame::addHand(std::vector<Card> hand)
{
	m_hands.push_back(hand);
}



int AIGame::getPlayerCount()
{
	return m_playerCount;
}

int AIGame::getPassCount()
{
	return m_passCount;
}

std::vector<Card> AIGame::getHand(int pos)
{
	return m_hands[pos];
}

bool AIGame::isOver()
{
	bool b = false;
	if (m_depthCount >= 2)
		return true; // Stop generating any children once we are 10 nodes deep

	// If it isn't guess mode (i.e. god mode) then search for a winner normally
	if (!m_isGuessMode)
	{
		if (isWinner())
			return true;
	}
	// otherwise for guess mode, stop the search once it hits a free reign turn
	else
	{
		if (isWinner())
			return true;
		else if (isFreeReign())
			return true;
	}
	return b;
}

bool AIGame::isWinner(int playerNum)
{
	return m_hands[playerNum].size() == 0;
}

bool AIGame::isWinner()
{
	return m_hands[m_originalPlayerPos].size() == 0;
}

bool AIGame::isFreeReign(int playerNum)
{
	return m_isFreeReign && m_originalPlayerPos == playerNum;
}


bool AIGame::isFreeReign()
{
	return m_isFreeReign && m_originalPlayerPos == m_playerPos;
}

std::vector<Card> AIGame::sortCards(std::vector<Card> pHand)
{
	auto hand = pHand;
	//std::sort(cards.begin(), cards.end(), cmp());
	int swapped;
	int i;
	for (i = 1; i < hand.size(); i++)
	{
		swapped = 0;    //this flag is to check if the array is already sorted
		int j;
		Card temp;
		for(j = 0; j < hand.size() - i; j++)
		{
			if (hand[j].getNumber() > hand[j + 1].getNumber())
			{
				if (! (hand[j + 1].getNumber() == Ace || hand[j + 1].getNumber() == Two) )
				{
					temp = hand[j];
					hand[j] = hand[j+1];
					hand[j+1] = temp;
					swapped = 1;
				}
			}
			else if (hand[j].getNumber() < hand[j + 1].getNumber())
			{
				if (hand[j].getNumber() == Ace || hand[j].getNumber() == Two)
				{
					temp = hand[j];
					hand[j] = hand[j+1];
					hand[j+1] = temp;
					swapped = 1;
				}
			}
			
			else //if left.Number == right.Number
			{
				if (hand[j].getSuite() > hand[j + 1].getSuite())
				{
					temp = hand[j];
					hand[j] = hand[j+1];
					hand[j+1] = temp;
					swapped = 1;
				}
			}
			
			if (hand[j].getNumber() == Two && hand[j + 1].getNumber() == Ace)
			{
				temp = hand[j];
				hand[j] = hand[j+1];
				hand[j+1] = temp;
				swapped = 1;
			}
		}
		if(!swapped){
			break; //if it is sorted then stop
		}
	}
	return hand;
}

int AIGame::getOriginalPlayerPos()
{
	return m_originalPlayerPos;
}

int AIGame::getDepth()
{
	return m_depthCount;
}