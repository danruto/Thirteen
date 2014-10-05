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
// Filename: GameMove.cpp
// Description: Represents the possible moves that can be played.
// Author:		Danny Sok
// Date Created: 9/08/2013
// Date Last Modified: 27/09/2013
#include "GameMove.h"
#include "HandEvaluator.h"

void GameMove::init(std::vector<Card> hand)
{
	m_hand = hand;
}

// ----------------------------------------
// playSingle
// 
// Plays the hand of type Single
// Input:
//  lastPlayed is the list of the last played cards
// Output:
//  none
// Conditions:
//  none
std::vector<Card> GameMove::playSingle(std::vector<Card> lastPlayed)
{
	if (lastPlayed.size() == 0)
		return playSingle();

	int curPos = 0;
	for(int i = 0; i < m_hand.size(); i++)
	{
		// Check if the card is bigger than the card played and play it unless
		// the previous card was an Ace or Two in which case it is actually smaller
		if (m_hand[i].getNumber() > lastPlayed[curPos].getNumber())
		{
			if (!(lastPlayed[curPos].getNumber() == Ace || lastPlayed[curPos].getNumber() == Two))
			{
				selectCards(1, i);
			}
		}

		// Check if the card values are the same. If they are then check which one has the
		// higher suite
		else if (m_hand[i].getNumber() == lastPlayed[curPos].getNumber())
		{
			if (m_hand[i].getSuite() > lastPlayed[curPos].getSuite())
			{
				selectCards(1, i);
			}
		}

		// Check if the card is a lower value than the one played. If the card happens to be
		// an Ace or Two then play it because these are actually larger cards in terms of game rules
		else if (m_hand[i].getNumber() < lastPlayed[curPos].getNumber())
		{
			if (m_hand[i].getNumber() == Ace && lastPlayed[curPos].getNumber() != Two)
			{
				selectCards(1, i);
			}
			else if (m_hand[i].getNumber() == Two)
			{
				selectCards(1, i);
			}
		}
	}

	// If we get to here that means we have to pass because we have no good cards
	if (!areCardsSelected())
		 return pass();

	return m_hand;
}

// ----------------------------------------
// playPair
// 
// Plays the hand of type Pair
// Input:
//  lastPlayed is the list of the last played cards
// Output:
//  none
// Conditions:
//  none
std::vector<Card> GameMove::playPair(std::vector<Card> lastPlayed)
{
	if (lastPlayed.size() == 0)
		return playPair();

	int curPos = 1;
	
	// We want to check all cards. As we have 1 card here, we don't want to check the last card. We instead do i+1
	for(int i = 0; i < m_hand.size() - curPos; i++)
	{
		if (m_hand[i].getNumber() == m_hand[i+1].getNumber() 
			&& m_hand[i].getNumber() > lastPlayed[curPos].getNumber())
		{
			if (!(lastPlayed[curPos].getNumber() == Ace || lastPlayed[curPos].getNumber() == Two))
			{
				selectCards(2, i);
			}
		}
		else if (m_hand[i].getNumber() == m_hand[i+1].getNumber() 
			&& m_hand[i].getNumber() == lastPlayed[curPos].getNumber())
		{
			if (m_hand[i+1].getSuite() > lastPlayed[curPos].getSuite())
			{
				selectCards(2, i);
			}
		}
		else if (m_hand[i].getNumber() == m_hand[i+1].getNumber() 
			&&  m_hand[i].getNumber() < lastPlayed[curPos].getNumber())
		{
			if (m_hand[i].getNumber() == Ace && lastPlayed[curPos].getNumber() != Two)
			{
				selectCards(2, i);
			}
			else if (m_hand[i].getNumber() == Two)
			{
				selectCards(2, i);
			}
		}
	}

	// If we get to here that means we have to pass because we have no good cards
	if (!areCardsSelected())
		pass();

	return m_hand;
}

// ----------------------------------------
// playTriple
// 
// Plays the hand of type Triple
// Input:
//  lastPlayed is the list of the last played cards
// Output:
//  none
// Conditions:
//  none
// TODO: Special check for bombing aces
std::vector<Card> GameMove::playTriple(std::vector<Card> lastPlayed)
{
	// Previous players have passed
	if (lastPlayed.size() == 0)
		return playTriple();

	// We are trying to bomb an ace
	if (lastPlayed.size() == 1)
		return playBombAce();

	int curPos = 2;
	for(int i = 0; i < m_hand.size() - curPos; i++)
	{
		if (m_hand[i].getNumber() == m_hand[i+1].getNumber() && 
			m_hand[i+1].getNumber() == m_hand[i+2].getNumber() &&
			m_hand[i].getNumber() > lastPlayed[curPos].getNumber())
		{
			if (!(lastPlayed[curPos].getNumber() == Ace || lastPlayed[curPos].getNumber() == Two))
			{
				selectCards(3, i);
			}
		}
		else if (m_hand[i].getNumber() == m_hand[i+1].getNumber() && 
				 m_hand[i+1].getNumber() == m_hand[i+2].getNumber() &&
				 m_hand[i].getNumber() == lastPlayed[curPos].getNumber())
		{
			if (m_hand[i+2].getSuite() > lastPlayed[curPos].getSuite())
			{
				selectCards(3, i);
			}
		}
		else if (m_hand[i].getNumber() == m_hand[i+1].getNumber() && 
				 m_hand[i+1].getNumber() == m_hand[i+2].getNumber() &&
				 m_hand[i].getNumber() < lastPlayed[curPos].getNumber())
		{
			if (m_hand[i].getNumber() == Ace && lastPlayed[curPos].getNumber() != Two)
			{
				selectCards(3, i);
			}
			else if (m_hand[i].getNumber() == Two)
			{
				selectCards(3, i);
			}
		}
	}

	// If we get to here that means we have to pass because we have no good cards
	if (!areCardsSelected())
		pass();

	return m_hand;
}

// ----------------------------------------
// playFourKind
// 
// Plays the m_hand of type FourKind
// Input:
//  lastPlayed is the list of the last played cards
// Output:
//  none
// Conditions:
//  none
std::vector<Card> GameMove::playFourKind(std::vector<Card> lastPlayed)
{
	if (lastPlayed.size() == 0)
		return playFourKind();

	int curPos = 3;
	for(int i = 0; i < m_hand.size() - curPos; i++)
	{
		if (m_hand[i].getNumber() == m_hand[i+1].getNumber() && 
			m_hand[i+1].getNumber() == m_hand[i+2].getNumber() && 
			m_hand[i+2].getNumber() == m_hand[i+3].getNumber())
		{
			if (m_hand[i].getNumber() > lastPlayed[curPos].getNumber() &&
				!(lastPlayed[curPos].getNumber() == Ace || lastPlayed[curPos].getNumber() == Two))
			{
				selectCards(4, i);
			}

			else if (m_hand[i].getNumber() == lastPlayed[curPos].getNumber() &&
				m_hand[i+2].getSuite() > lastPlayed[curPos].getSuite())
			{
				selectCards(4, i);
			}

			else if (m_hand[i].getNumber() < lastPlayed[curPos].getNumber())
			{
				if (m_hand[i].getNumber() == Ace && lastPlayed[curPos].getNumber() != Two)
				{
					selectCards(4, i);
				}
				else if (m_hand[i].getNumber() == Two)
				{
					selectCards(4, i);
				}
			}
		}
	}

	// If we get to here that means we have to pass because we have no good cards
	if (!areCardsSelected())
		pass();

	return m_hand;
}

// ----------------------------------------
// playStraight
// 
// Plays the hand of type Straight
// Input:
//  lastPlayed is the list of the last played cards
// Output:
//  none
// Conditions:
//  none
std::vector<Card> GameMove::playStraight(std::vector<Card> lastPlayed)
{
	if (lastPlayed.size() == 0)
		return playStraight();

	// Get the last card played as we want to compare the highest values
	int szHand = lastPlayed.size();

	// Get the value of the last card for ease of reference
	int posLastCard = szHand - 1;

	// Find the size of the cards, then play the appropriate amount of cards
	// for that straight
	if (szHand == 3)
	{
		for (int i = 0; i < m_hand.size() - posLastCard; i++)
		{
			if (m_hand[i+2].getNumber() == m_hand[i+1].getNumber() + 1 && 
				m_hand[i+1].getNumber() == m_hand[i].getNumber() + 1)
			{
				if (m_hand[i+posLastCard].getNumber() > lastPlayed[posLastCard].getNumber() &&
					!(lastPlayed[posLastCard].getNumber() == Ace || lastPlayed[posLastCard].getNumber() == Two))
				{
					selectCards(szHand, i);
				}

				else if (m_hand[i+posLastCard].getNumber() == lastPlayed[posLastCard].getNumber() &&
					m_hand[i+posLastCard].getSuite() > lastPlayed[posLastCard].getSuite())
				{
					selectCards(szHand, i);
				}

				else if (m_hand[i+posLastCard].getNumber() < lastPlayed[posLastCard].getNumber() &&
					m_hand[i+posLastCard].getNumber() == Ace && lastPlayed[posLastCard].getNumber() != Two)
				{
					selectCards(szHand, i);
				}
			}
		}
	}
	else if (szHand == 4)
	{
		for (int i = 0; i < m_hand.size() - posLastCard; i++)
		{
			if (m_hand[i+3].getNumber() == m_hand[i+2].getNumber() + 1 && 
				m_hand[i+2].getNumber() == m_hand[i+1].getNumber() + 1 && 
				m_hand[i+1].getNumber() == m_hand[i].getNumber() + 1)
			{
				if (m_hand[i+posLastCard].getNumber() > lastPlayed[posLastCard].getNumber() &&
					!(lastPlayed[posLastCard].getNumber() == Ace || lastPlayed[posLastCard].getNumber() == Two))
				{
					selectCards(szHand, i);
				}

				else if (m_hand[i+posLastCard].getNumber() == lastPlayed[posLastCard].getNumber() &&
					m_hand[i+posLastCard].getSuite() > lastPlayed[posLastCard].getSuite())
				{
					selectCards(szHand, i);
				}

				else if (m_hand[i+posLastCard].getNumber() < lastPlayed[posLastCard].getNumber() &&
					m_hand[i+posLastCard].getNumber() == Ace && lastPlayed[posLastCard].getNumber() != Two)
				{
					selectCards(szHand, i);
				}
			}
		}
	}
	else if (szHand == 5)
	{
		for (int i = 0; i < m_hand.size() - posLastCard; i++)
		{
			if (m_hand[i+4].getNumber() == m_hand[i+3].getNumber() + 1 && 
				m_hand[i+3].getNumber() == m_hand[i+2].getNumber() + 1 && 
				m_hand[i+2].getNumber() == m_hand[i+1].getNumber() + 1 && 
				m_hand[i+1].getNumber() == m_hand[i].getNumber() + 1)
			{
				if (m_hand[i+posLastCard].getNumber() > lastPlayed[posLastCard].getNumber() &&
					!(lastPlayed[posLastCard].getNumber() == Ace || lastPlayed[posLastCard].getNumber() == Two))
				{
					selectCards(szHand, i);
				}

				else if (m_hand[i+posLastCard].getNumber() == lastPlayed[posLastCard].getNumber() &&
					m_hand[i+posLastCard].getSuite() > lastPlayed[posLastCard].getSuite())
				{
					selectCards(szHand, i);
				}

				else if (m_hand[i+posLastCard].getNumber() < lastPlayed[posLastCard].getNumber() &&
					m_hand[i+posLastCard].getNumber() == Ace && lastPlayed[posLastCard].getNumber() != Two)
				{
					selectCards(szHand, i);
				}
			}
		}
	}
	else if (szHand == 6)
	{
		for (int i = 0; i < m_hand.size() - posLastCard; i++)
		{
			if (m_hand[i+5].getNumber() == m_hand[i+4].getNumber() + 1 && 
				m_hand[i+4].getNumber() == m_hand[i+3].getNumber() + 1 && 
				m_hand[i+3].getNumber() == m_hand[i+2].getNumber() + 1 && 
				m_hand[i+2].getNumber() == m_hand[i+1].getNumber() + 1 && 
				m_hand[i+1].getNumber() == m_hand[i].getNumber() + 1)
			{
				if (m_hand[i+posLastCard].getNumber() > lastPlayed[posLastCard].getNumber() &&
					!(lastPlayed[posLastCard].getNumber() == Ace || lastPlayed[posLastCard].getNumber() == Two))
				{
					selectCards(szHand, i);
				}

				else if (m_hand[i+posLastCard].getNumber() == lastPlayed[posLastCard].getNumber() &&
					m_hand[i+posLastCard].getSuite() > lastPlayed[posLastCard].getSuite())
				{
					selectCards(szHand, i);
				}

				else if (m_hand[i+posLastCard].getNumber() < lastPlayed[posLastCard].getNumber() &&
					m_hand[i+posLastCard].getNumber() == Ace && lastPlayed[posLastCard].getNumber() != Two)
				{
					selectCards(szHand, i);
				}
			}
		}
	}
	else if (szHand == 7)
	{
		for (int i = 0; i < m_hand.size() - posLastCard; i++)
		{
			if (m_hand[i+6].getNumber() == m_hand[i+5].getNumber() + 1 && 
				m_hand[i+5].getNumber() == m_hand[i+4].getNumber() + 1 && 
				m_hand[i+4].getNumber() == m_hand[i+3].getNumber() + 1 && 
				m_hand[i+3].getNumber() == m_hand[i+2].getNumber() + 1 && 
				m_hand[i+2].getNumber() == m_hand[i+1].getNumber() + 1 && 
				m_hand[i+1].getNumber() == m_hand[i].getNumber() + 1)
			{
				if (m_hand[i+posLastCard].getNumber() > lastPlayed[posLastCard].getNumber() &&
					!(lastPlayed[posLastCard].getNumber() == Ace || lastPlayed[posLastCard].getNumber() == Two))
				{
					selectCards(szHand, i);
				}

				else if (m_hand[i+posLastCard].getNumber() == lastPlayed[posLastCard].getNumber() &&
					m_hand[i+posLastCard].getSuite() > lastPlayed[posLastCard].getSuite())
				{
					selectCards(szHand, i);
				}

				else if (m_hand[i+posLastCard].getNumber() < lastPlayed[posLastCard].getNumber() &&
					m_hand[i+posLastCard].getNumber() == Ace && lastPlayed[posLastCard].getNumber() != Two)
				{
					selectCards(szHand, i);
				}
			}
		}
	}
	else if (szHand == 8)
	{
		for (int i = 0; i < m_hand.size() - posLastCard; i++)
		{
			if (m_hand[i+7].getNumber() == m_hand[i+6].getNumber() + 1 && 
				m_hand[i+6].getNumber() == m_hand[i+5].getNumber() + 1 && 
				m_hand[i+5].getNumber() == m_hand[i+4].getNumber() + 1 && 
				m_hand[i+4].getNumber() == m_hand[i+3].getNumber() + 1 && 
				m_hand[i+3].getNumber() == m_hand[i+2].getNumber() + 1 && 
				m_hand[i+2].getNumber() == m_hand[i+1].getNumber() + 1 && 
				m_hand[i+1].getNumber() == m_hand[i].getNumber() + 1)
			{
				if (m_hand[i+posLastCard].getNumber() > lastPlayed[posLastCard].getNumber() &&
					!(lastPlayed[posLastCard].getNumber() == Ace || lastPlayed[posLastCard].getNumber() == Two))
				{
					selectCards(szHand, i);
				}

				else if (m_hand[i+posLastCard].getNumber() == lastPlayed[posLastCard].getNumber() &&
					m_hand[i+posLastCard].getSuite() > lastPlayed[posLastCard].getSuite())
				{
					selectCards(szHand, i);
				}

				else if (m_hand[i+posLastCard].getNumber() < lastPlayed[posLastCard].getNumber() &&
					m_hand[i+posLastCard].getNumber() == Ace && lastPlayed[posLastCard].getNumber() != Two)
				{
					selectCards(szHand, i);
				}
			}
		}
	}
	else if (szHand == 9)
	{
		for (int i = 0; i < m_hand.size() - posLastCard; i++)
		{
			if (m_hand[i+8].getNumber() == m_hand[i+7].getNumber() + 1 && 
				m_hand[i+7].getNumber() == m_hand[i+6].getNumber() + 1 && 
				m_hand[i+6].getNumber() == m_hand[i+5].getNumber() + 1 && 
				m_hand[i+5].getNumber() == m_hand[i+4].getNumber() + 1 && 
				m_hand[i+4].getNumber() == m_hand[i+3].getNumber() + 1 && 
				m_hand[i+3].getNumber() == m_hand[i+2].getNumber() + 1 && 
				m_hand[i+2].getNumber() == m_hand[i+1].getNumber() + 1 && 
				m_hand[i+1].getNumber() == m_hand[i].getNumber() + 1)
			{
				if (m_hand[i+posLastCard].getNumber() > lastPlayed[posLastCard].getNumber() &&
					!(lastPlayed[posLastCard].getNumber() == Ace || lastPlayed[posLastCard].getNumber() == Two))
				{
					selectCards(szHand, i);
				}

				else if (m_hand[i+posLastCard].getNumber() == lastPlayed[posLastCard].getNumber() &&
					m_hand[i+posLastCard].getSuite() > lastPlayed[posLastCard].getSuite())
				{
					selectCards(szHand, i);
				}

				else if (m_hand[i+posLastCard].getNumber() < lastPlayed[posLastCard].getNumber() &&
					m_hand[i+posLastCard].getNumber() == Ace && lastPlayed[posLastCard].getNumber() != Two)
				{
					selectCards(szHand, i);
				}
			}
		}
	}
	else if (szHand == 10)
	{
		for (int i = 0; i < m_hand.size() - posLastCard; i++)
		{
			if (m_hand[i+9].getNumber() == m_hand[i+8].getNumber() + 1 && 
				m_hand[i+8].getNumber() == m_hand[i+7].getNumber() + 1 && 
				m_hand[i+7].getNumber() == m_hand[i+6].getNumber() + 1 && 
				m_hand[i+6].getNumber() == m_hand[i+5].getNumber() + 1 && 
				m_hand[i+5].getNumber() == m_hand[i+4].getNumber() + 1 && 
				m_hand[i+4].getNumber() == m_hand[i+3].getNumber() + 1 && 
				m_hand[i+3].getNumber() == m_hand[i+2].getNumber() + 1 && 
				m_hand[i+2].getNumber() == m_hand[i+1].getNumber() + 1 && 
				m_hand[i+1].getNumber() == m_hand[i].getNumber() + 1)
			{
				if (m_hand[i+posLastCard].getNumber() > lastPlayed[posLastCard].getNumber() &&
					!(lastPlayed[posLastCard].getNumber() == Ace || lastPlayed[posLastCard].getNumber() == Two))
				{
					selectCards(szHand, i);
				}

				else if (m_hand[i+posLastCard].getNumber() == lastPlayed[posLastCard].getNumber() &&
					m_hand[i+posLastCard].getSuite() > lastPlayed[posLastCard].getSuite())
				{
					selectCards(szHand, i);
				}

				else if (m_hand[i+posLastCard].getNumber() < lastPlayed[posLastCard].getNumber() &&
					m_hand[i+posLastCard].getNumber() == Ace && lastPlayed[posLastCard].getNumber() != Two)
				{
					selectCards(szHand, i);
				}
			}
		}
	}
	else if (szHand == 11)
	{
		for (int i = 0; i < m_hand.size() - posLastCard; i++)
		{
			if (m_hand[i+10].getNumber() == m_hand[i+9].getNumber() + 1 && 
				m_hand[i+9].getNumber() == m_hand[i+8].getNumber() + 1 && 
				m_hand[i+8].getNumber() == m_hand[i+7].getNumber() + 1 && 
				m_hand[i+7].getNumber() == m_hand[i+6].getNumber() + 1 && 
				m_hand[i+6].getNumber() == m_hand[i+5].getNumber() + 1 && 
				m_hand[i+5].getNumber() == m_hand[i+4].getNumber() + 1 && 
				m_hand[i+4].getNumber() == m_hand[i+3].getNumber() + 1 && 
				m_hand[i+3].getNumber() == m_hand[i+2].getNumber() + 1 && 
				m_hand[i+2].getNumber() == m_hand[i+1].getNumber() + 1 && 
				m_hand[i+1].getNumber() == m_hand[i].getNumber() + 1)
			{
				if (m_hand[i+posLastCard].getNumber() > lastPlayed[posLastCard].getNumber() &&
					!(lastPlayed[posLastCard].getNumber() == Ace || lastPlayed[posLastCard].getNumber() == Two))
				{
					selectCards(szHand, i);
				}
				else if (m_hand[i+posLastCard].getNumber() == lastPlayed[posLastCard].getNumber() &&
					m_hand[i+posLastCard].getSuite() > lastPlayed[posLastCard].getSuite())
				{
					selectCards(szHand, i);
				}
				else if (m_hand[i+posLastCard].getNumber() < lastPlayed[posLastCard].getNumber() &&
					m_hand[i+posLastCard].getNumber() == Ace && lastPlayed[posLastCard].getNumber() != Two)
				{
					selectCards(szHand, i);
				}
			}
		}
	}
	else if (szHand == 12)
	{
		for (int i = 0; i < m_hand.size() - szHand; i++)
		{
			if (m_hand[i+11].getNumber() == m_hand[i+10].getNumber() + 1 && 
				m_hand[i+10].getNumber() == m_hand[i+9].getNumber() + 1 && 
				m_hand[i+9].getNumber() == m_hand[i+8].getNumber() + 1 && 
				m_hand[i+8].getNumber() == m_hand[i+7].getNumber() + 1 && 
				m_hand[i+7].getNumber() == m_hand[i+6].getNumber() + 1 && 
				m_hand[i+6].getNumber() == m_hand[i+5].getNumber() + 1 && 
				m_hand[i+5].getNumber() == m_hand[i+4].getNumber() + 1 && 
				m_hand[i+4].getNumber() == m_hand[i+3].getNumber() + 1 && 
				m_hand[i+3].getNumber() == m_hand[i+2].getNumber() + 1 && 
				m_hand[i+2].getNumber() == m_hand[i+1].getNumber() + 1 && 
				m_hand[i+1].getNumber() == m_hand[i].getNumber() + 1)
			{
				if (m_hand[i+posLastCard].getNumber() > lastPlayed[posLastCard].getNumber() &&
					!(lastPlayed[posLastCard].getNumber() == Ace || lastPlayed[posLastCard].getNumber() == Two))
				{
					selectCards(szHand, i);
				}

				else if (m_hand[i+posLastCard].getNumber() == lastPlayed[posLastCard].getNumber() &&
					m_hand[i+posLastCard].getSuite() > lastPlayed[posLastCard].getSuite())
				{
					selectCards(szHand, i);
				}

				else if (m_hand[i+posLastCard].getNumber() < lastPlayed[posLastCard].getNumber() &&
					m_hand[i+posLastCard].getNumber() == Ace && lastPlayed[posLastCard].getNumber() != Two)
				{
					selectCards(szHand, i);
				}
			}
		}
	}
		// Straight can be 3 Cards to 13
		// Search for a straight of the size of the played Card
		// Then check the highest card values against each other
		// Then determine to play or pass
		

	// If we get to here that means we have to pass because we have no good cards
	if (!areCardsSelected())
		pass();

	return m_hand;
}

std::vector<Card> GameMove::playPairStraight(std::vector<Card> lastPlayed)
{
	// Previous players passed
	if (lastPlayed.size() == 0)
		return playPairStraight();

	if (lastPlayed.size() == 1)
		return playBombTwo();


	// Get the last card played as we want to compare the highest values
	int szHand = lastPlayed.size();

	// Get the value of the last card for ease of reference
	int posLastCard = szHand - 1;

	// Find the size of the cards, then play the appropriate amount of cards
	// for that straight
	if (szHand == 6)
	{
		// 13 - 6 = 7
		// i = 6 MAX. 6 + 6 = 12;
		for (int i = 0; i < m_hand.size() - szHand; i++)
		{
			// 01 23 45
			if (m_hand[i+6].getNumber() == m_hand[i+4].getNumber() + 1 && 
				m_hand[i+4].getNumber() == m_hand[i+2].getNumber() + 1 &&
				m_hand[i+2].getNumber() == m_hand[i].getNumber() + 1)
			{
				if (m_hand[i+posLastCard].getNumber() > lastPlayed[posLastCard].getNumber() &&
					!(lastPlayed[posLastCard].getNumber() == Ace || lastPlayed[posLastCard].getNumber() == Two))
				{
					selectCards(szHand, i);
				}

				else if (m_hand[i+posLastCard].getNumber() == lastPlayed[posLastCard].getNumber() &&
					m_hand[i+posLastCard].getSuite() > lastPlayed[posLastCard].getSuite())
				{
					selectCards(szHand, i);
				}

				else if (m_hand[i+posLastCard].getNumber() < lastPlayed[posLastCard].getNumber() &&
					m_hand[i+posLastCard].getNumber() == Ace && lastPlayed[posLastCard].getNumber() != Two)
				{
					selectCards(szHand, i);
				}
			}
		}
	}
	else if (szHand == 8)
	{
		for (int i = 0; i < m_hand.size() - szHand; i++)
		{
			if (m_hand[i+8].getNumber() == m_hand[i+6].getNumber() + 1 && 
				m_hand[i+6].getNumber() == m_hand[i+4].getNumber() + 1 && 
				m_hand[i+4].getNumber() == m_hand[i+2].getNumber() + 1 &&
				m_hand[i+2].getNumber() == m_hand[i].getNumber() + 1)
			{
				if (m_hand[i+posLastCard].getNumber() > lastPlayed[posLastCard].getNumber() &&
					!(lastPlayed[posLastCard].getNumber() == Ace || lastPlayed[posLastCard].getNumber() == Two))
				{
					selectCards(szHand, i);
				}

				else if (m_hand[i+posLastCard].getNumber() == lastPlayed[posLastCard].getNumber() &&
					m_hand[i+posLastCard].getSuite() > lastPlayed[posLastCard].getSuite())
				{
					selectCards(szHand, i);
				}

				else if (m_hand[i+posLastCard].getNumber() < lastPlayed[posLastCard].getNumber() &&
					m_hand[i+posLastCard].getNumber() == Ace && lastPlayed[posLastCard].getNumber() != Two)
				{
					selectCards(szHand, i);
				}
			}
		}
	}
	else if (szHand == 10)
	{
		for (int i = 0; i < m_hand.size() - szHand; i++)
		{
			if (m_hand[i+10].getNumber() == m_hand[i+8].getNumber() + 1 && 
				m_hand[i+8].getNumber() == m_hand[i+6].getNumber() + 1 && 
				m_hand[i+6].getNumber() == m_hand[i+4].getNumber() + 1 && 
				m_hand[i+4].getNumber() == m_hand[i+2].getNumber() + 1 &&
				m_hand[i+2].getNumber() == m_hand[i].getNumber() + 1)
			{
				if (m_hand[i+posLastCard].getNumber() > lastPlayed[posLastCard].getNumber() &&
					!(lastPlayed[posLastCard].getNumber() == Ace || lastPlayed[posLastCard].getNumber() == Two))
				{
					selectCards(szHand, i);
				}

				else if (m_hand[i+posLastCard].getNumber() == lastPlayed[posLastCard].getNumber() &&
					m_hand[i+posLastCard].getSuite() > lastPlayed[posLastCard].getSuite())
				{
					selectCards(szHand, i);
				}

				else if (m_hand[i+posLastCard].getNumber() < lastPlayed[posLastCard].getNumber() &&
					m_hand[i+posLastCard].getNumber() == Ace && lastPlayed[posLastCard].getNumber() != Two)
				{
					selectCards(szHand, i);
				}
			}
		}
	}
	else if (szHand == 12)
	{
		for (int i = 0; i < m_hand.size() - szHand; i++)
		{
			if (m_hand[i+12].getNumber() == m_hand[i+10].getNumber() + 1 && 
				m_hand[i+10].getNumber() == m_hand[i+8].getNumber() + 1 && 
				m_hand[i+8].getNumber() == m_hand[i+6].getNumber() + 1 && 
				m_hand[i+6].getNumber() == m_hand[i+4].getNumber() + 1 && 
				m_hand[i+4].getNumber() == m_hand[i+2].getNumber() + 1 &&
				m_hand[i+2].getNumber() == m_hand[i].getNumber() + 1)
			{
				if (m_hand[i+posLastCard].getNumber() > lastPlayed[posLastCard].getNumber() &&
					!(lastPlayed[posLastCard].getNumber() == Ace || lastPlayed[posLastCard].getNumber() == Two))
				{
					selectCards(szHand, i);
				}

				else if (m_hand[i+posLastCard].getNumber() == lastPlayed[posLastCard].getNumber() &&
					m_hand[i+posLastCard].getSuite() > lastPlayed[posLastCard].getSuite())
				{
					selectCards(szHand, i);
				}

				else if (m_hand[i+posLastCard].getNumber() < lastPlayed[posLastCard].getNumber() &&
					m_hand[i+posLastCard].getNumber() == Ace && lastPlayed[posLastCard].getNumber() != Two)
				{
					selectCards(szHand, i);
				}
			}
		}
	}

	
	if (!areCardsSelected())
		pass();
	
	return m_hand;
}

// ----------------------------------------
// selectCards
// 
// Plays the cards based on the parameters 
// Input:
//  size - the size of the hand to be played (e.g. 3 for a triple)
//	i - the starting position of the hand (e.g. 3 for the 3rd card in the players overall hand)
// Output:
//  none
// Conditions:
//  none
void GameMove::selectCards(int size, int i)
{
	if (!areCardsSelected())
	{
		for (int j = i; j < i+size; j++)
		{
			m_hand[j].setSelected(true);
		}
	}
}

void GameMove::deselectCards(int size, int i)
{
	for (int j = i; j < i+size; j++)
	{
		m_hand[j].setSelected(false);
	}
}

std::vector<Card> GameMove::deselectCards(std::vector<Card> phand)
{
	auto hand = phand;
	for (int i = 0; i < hand.size(); i++)
	{
		hand[i].setSelected(false);
	}

	return hand;
}


bool GameMove::areCardsSelected()
{
	std::vector<Card>::iterator it = m_hand.begin();
	for (it; it != m_hand.end(); ++it)
	{
		if (it->isSelected())
			return true;
	}

	return false;
}

bool GameMove::areCardsSelected(std::vector<Card> hand)
{
	for (auto &e: hand)
	{
		if (e.isSelected())
			return true;
	}

	return false;
}

std::vector<Card> GameMove::pass()
{
	deselectCards(m_hand.size()-1, 0);
	return m_hand;
}

//--------------------------------
// Copied from MediumAI, will need to refactor properly later
//--------------------------------
std::vector<Card> GameMove::playLowestPossibleHand(HAND_TYPES hand, std::vector<Card> lastPlayed)
{
	switch(hand)
	{
		case HAND_TYPES::Single:
			if (lastPlayed.size() > 0)
				playSingle(lastPlayed);
			else
				playSingle();
			break;
		case HAND_TYPES::Pair:
			if (lastPlayed.size() > 0)
				playPair(lastPlayed);
			else
				playPair();
			break;
		case HAND_TYPES::Triple:
			if (lastPlayed.size() > 0)
				playTriple(lastPlayed);
			else
				playTriple();
			break;
		case HAND_TYPES::Straight:
			if (lastPlayed.size() > 0)
				playStraight(lastPlayed);
			else
				playStraight();
			break;
		case HAND_TYPES::PairStraight:
			if (lastPlayed.size() > 0)
				playPairStraight(lastPlayed);
			else
				playPairStraight();
			break;
		case HAND_TYPES::FourKind:
			if (lastPlayed.size() > 0)
				playFourKind(lastPlayed);
			else
				playFourKind();
			break;
		// Means we have a free reign turn
		default:
			playSingle();
			//Human::pass();
			break;
	}

	return m_hand;
}

std::vector<Card> GameMove::playHighestPossibleHand(HAND_TYPES hand, std::vector<Card> lastPlayed)
{
	switch(hand)
	{
		case HAND_TYPES::Single:
			if (lastPlayed.size() > 0)
				playHighSingle(lastPlayed);
			else
				playHighSingle();
			break;
		case HAND_TYPES::Pair:
			if (lastPlayed.size() > 0)
				playHighPair(lastPlayed);
			else
				playHighPair();
			break;
		case HAND_TYPES::Triple:
			if (lastPlayed.size() > 0)
				playHighTriple(lastPlayed);
			else
				playHighTriple();
			break;
		case HAND_TYPES::Straight:
			if (lastPlayed.size() > 0)
				playHighStraight(lastPlayed);
			else
				playHighStraight();
			break;
		case HAND_TYPES::PairStraight:
			if (lastPlayed.size() > 0)
				playHighPairStraight(lastPlayed);
			else
				playHighPairStraight();
			break;
		case HAND_TYPES::FourKind:
			if (lastPlayed.size() > 0)
				playHighFourKind(lastPlayed);
			else
				playHighFourKind();
			break;

		// means we have a free reign turn
		default:
			HandEvaluator eval;
			auto possibleHands = eval.findPossibleHands(m_hand);
			playHighestPossibleHand(possibleHands.back(), lastPlayed);
			break;
	}

	return m_hand;
}

// Break the Pair Straights to beat last hand if GREATER than last played AND no 2's are in hand
// VS: Pair, Straight
std::vector<Card> GameMove::playActionD(std::vector<Card> lastPlayed)
{
	// Get the last card played as we want to compare the highest values
	int szHand = lastPlayed.size();

	// Get the value of the last card for ease of reference
	int posLastCard = szHand - 1;

	if (hasTwoInHand())
	{
		return pass();
	}

	// Pair
	if (szHand == 2)
	{
		return playPair(lastPlayed);
		
	}

	// Straight
	if (szHand == 3)
	{
		for (int i = 0; i < m_hand.size() - posLastCard; i++)
		{
			if (m_hand[i+4].getNumber() == m_hand[i+2].getNumber() + 1 && 
				m_hand[i+2].getNumber() == m_hand[i].getNumber() + 1)
			{
				if (m_hand[i+posLastCard].getNumber() > lastPlayed[posLastCard].getNumber() &&
					!(lastPlayed[posLastCard].getNumber() == Ace || lastPlayed[posLastCard].getNumber() == Two))
				{
					selectCards(szHand, i);
				}

				else if (m_hand[i+posLastCard].getNumber() == lastPlayed[posLastCard].getNumber() &&
					m_hand[i+posLastCard].getSuite() > lastPlayed[posLastCard].getSuite())
				{
					selectCards(szHand, i);
				}

				else if (m_hand[i+posLastCard].getNumber() < lastPlayed[posLastCard].getNumber() &&
					m_hand[i+posLastCard].getNumber() == Ace && lastPlayed[posLastCard].getNumber() != Two)
				{
					selectCards(szHand, i);
				}
			}
		}
	}
	else if (szHand == 4)
	{
		for (int i = 0; i < m_hand.size() - posLastCard; i++)
		{
			if (m_hand[i+6].getNumber() == m_hand[i+4].getNumber() + 1 && 
				m_hand[i+4].getNumber() == m_hand[i+4].getNumber() + 1 && 
				m_hand[i+2].getNumber() == m_hand[i].getNumber() + 1)
			{
				if (m_hand[i+posLastCard].getNumber() > lastPlayed[posLastCard].getNumber() &&
					!(lastPlayed[posLastCard].getNumber() == Ace || lastPlayed[posLastCard].getNumber() == Two))
				{
					selectCards(szHand, i);
				}

				else if (m_hand[i+posLastCard].getNumber() == lastPlayed[posLastCard].getNumber() &&
					m_hand[i+posLastCard].getSuite() > lastPlayed[posLastCard].getSuite())
				{
					selectCards(szHand, i);
				}

				else if (m_hand[i+posLastCard].getNumber() < lastPlayed[posLastCard].getNumber() &&
					m_hand[i+posLastCard].getNumber() == Ace && lastPlayed[posLastCard].getNumber() != Two)
				{
					selectCards(szHand, i);
				}
			}
		}
	}
	else if (szHand == 5)
	{
		for (int i = 0; i < m_hand.size() - posLastCard; i++)
		{
			if (m_hand[i+8].getNumber() == m_hand[i+6].getNumber() + 1 && 
				m_hand[i+6].getNumber() == m_hand[i+4].getNumber() + 1 && 
				m_hand[i+4].getNumber() == m_hand[i+2].getNumber() + 1 && 
				m_hand[i+2].getNumber() == m_hand[i].getNumber() + 1)
			{
				if (m_hand[i+posLastCard].getNumber() > lastPlayed[posLastCard].getNumber() &&
					!(lastPlayed[posLastCard].getNumber() == Ace || lastPlayed[posLastCard].getNumber() == Two))
				{
					selectCards(szHand, i);
				}

				else if (m_hand[i+posLastCard].getNumber() == lastPlayed[posLastCard].getNumber() &&
					m_hand[i+posLastCard].getSuite() > lastPlayed[posLastCard].getSuite())
				{
					selectCards(szHand, i);
				}

				else if (m_hand[i+posLastCard].getNumber() < lastPlayed[posLastCard].getNumber() &&
					m_hand[i+posLastCard].getNumber() == Ace && lastPlayed[posLastCard].getNumber() != Two)
				{
					selectCards(szHand, i);
				}
			}
		}
	}
	else if (szHand == 6)
	{
		for (int i = 0; i < m_hand.size() - posLastCard; i++)
		{
			if (m_hand[i+10].getNumber() == m_hand[i+8].getNumber() + 1 && 
				m_hand[i+8].getNumber()  == m_hand[i+6].getNumber() + 1 && 
				m_hand[i+6].getNumber()  == m_hand[i+4].getNumber() + 1 && 
				m_hand[i+4].getNumber()  == m_hand[i+2].getNumber() + 1 && 
				m_hand[i+2].getNumber()  == m_hand[i].getNumber() + 1)
			{
				if (m_hand[i+posLastCard].getNumber() > lastPlayed[posLastCard].getNumber() &&
					!(lastPlayed[posLastCard].getNumber() == Ace || lastPlayed[szHand].getNumber() == Two))
				{
					selectCards(szHand, i);
				}

				else if (m_hand[i+posLastCard].getNumber() == lastPlayed[posLastCard].getNumber() &&
					m_hand[i+posLastCard].getSuite() > lastPlayed[posLastCard].getSuite())
				{
					selectCards(szHand, i);
				}

				else if (m_hand[i+posLastCard].getNumber() < lastPlayed[posLastCard].getNumber() &&
					m_hand[i+posLastCard].getNumber() == Ace && lastPlayed[posLastCard].getNumber() != Two)
				{
					selectCards(szHand, i);
				}
			}
		}
	}
	
	// Only up to 7 because we can assume that the hand will have a maximum of 6 pairs (12 cards) + 1 possible extra (13th card)
	else if (szHand == 7)
	{
		for (int i = 0; i < m_hand.size() - posLastCard; i++)
		{
			if (m_hand[i+12].getNumber() == m_hand[i+10].getNumber() + 1 && 
				m_hand[i+10].getNumber() == m_hand[i+8].getNumber() + 1 && 
				m_hand[i+8].getNumber()  == m_hand[i+6].getNumber() + 1 && 
				m_hand[i+6].getNumber()  == m_hand[i+4].getNumber() + 1 && 
				m_hand[i+4].getNumber()  == m_hand[i+2].getNumber() + 1 && 
				m_hand[i+2].getNumber()  == m_hand[i].getNumber() + 1)
			{
				if (m_hand[i+posLastCard].getNumber() > lastPlayed[posLastCard].getNumber() &&
					!(lastPlayed[posLastCard].getNumber() == Ace || lastPlayed[posLastCard].getNumber() == Two))
				{
					selectCards(szHand, i);
				}

				else if (m_hand[i+posLastCard].getNumber() == lastPlayed[posLastCard].getNumber() &&
					m_hand[i+posLastCard].getSuite() > lastPlayed[posLastCard].getSuite())
				{
					selectCards(szHand, i);
				}

				else if (m_hand[i+posLastCard].getNumber() < lastPlayed[posLastCard].getNumber() &&
					m_hand[i+posLastCard].getNumber() == Ace && lastPlayed[posLastCard].getNumber() != Two)
				{
					selectCards(szHand, i);
				}
			}
		}
	}
	

	if (!areCardsSelected())
		pass();

	return m_hand;
}

// Play a Pair or higher if any opponent(s) have only one card in hand
std::vector<Card> GameMove::playActionF(bool opponentHasOneCardLeft)
{
	/*for (int i = 1; i < m_game->getPlayerCount(); i++)
	{
		auto p = m_game->getPlayer(i);
		if (p->getCards().size() == 1)
		{
			playPair();
		}
	}*/

	HandEvaluator handEval;
	int posToPlay = 0;
	auto possibleHands = handEval.findPossibleHands(m_hand);
	
	for (int i = 0; i < possibleHands.size(); i++)
	{
		if (possibleHands[i] != HAND_TYPES::Single)
		{
			posToPlay = i;
			break;
		}
	}

	if (opponentHasOneCardLeft)
	{
		switch (possibleHands[posToPlay])
		{
		case HAND_TYPES::Pair:
			playPair();
			break;
		case HAND_TYPES::Triple:
			playTriple();
			break;
		case HAND_TYPES::Straight:
			playStraight();
			break;
		case HAND_TYPES::FourKind:
			playFourKind();
			break;
		case HAND_TYPES::PairStraight:
			playPairStraight();
			break;
		default:
			break;
		}
	}

	if (!areCardsSelected())
		pass();

	return m_hand;
}

// Break a Hand IF you cannot get a free reign turn
// i.e. break a pair two to force the other players to pass
// e.g Play a 2D from a pair 2 in order to get the free reign
std::vector<Card> GameMove::playActionG()
{
	HandEvaluator eval;
	for (int i = 0; i < m_hand.size(); i++)
	{
		if (eval.isHighestCardInGame(&m_hand[i]))
			selectCards(1, i);
	}
	if (!areCardsSelected())
		pass();

	return m_hand;
}

std::vector<Card> GameMove::playHighSingle(std::vector<Card> lastPlayed)
{
	if (lastPlayed.size() == 0)
		return playHighSingle();

	int curPos = 0;
	for(int i = m_hand.size() - 1; i > 0; i--)
	{
		// Check if the card is bigger than the card played and play it unless
		// the previous card was an Ace or Two in which case it is actually smaller
		if (m_hand[i].getNumber() > lastPlayed[curPos].getNumber())
		{
			if (!(lastPlayed[curPos].getNumber() == Ace || lastPlayed[curPos].getNumber() == Two))
			{
				selectCards(1, i);
			}
		}

		// Check if the card values are the same. If they are then check which one has the
		// higher suite
		else if (m_hand[i].getNumber() == lastPlayed[curPos].getNumber())
		{
			if (m_hand[i].getSuite() > lastPlayed[curPos].getSuite())
			{
				selectCards(1, i);
			}
		}

		// Check if the card is a lower value than the one played. If the card happens to be
		// an Ace or Two then play it because these are actually larger cards in terms of game rules
		else if (m_hand[i].getNumber() < lastPlayed[curPos].getNumber())
		{
			if (m_hand[i].getNumber() == Ace && lastPlayed[curPos].getNumber() != Two)
			{
				selectCards(1, i);
			}
			else if (m_hand[i].getNumber() == Two)
			{
				selectCards(1, i);
			}
		}
	}

	// If we get to here that means we have to pass because we have no good cards
	if (!areCardsSelected())
		pass();

	return m_hand;
}

std::vector<Card> GameMove::playHighPair(std::vector<Card> lastPlayed)
{
	if (lastPlayed.size() == 0)
		return playHighPair();

	int curPos = 1;
	
	// We want to check all cards. As we have 1 card here, we don't want to check the last card. We instead do i+1
	//for(int i = 0; i < m_hand.size() - curPos; i++)
	for (int i = m_hand.size() - curPos - 1; i >= 0; i--)
	{
		if (m_hand[i].getNumber() == m_hand[i+1].getNumber() 
			&& m_hand[i].getNumber() > lastPlayed[curPos].getNumber())
		{
			if (!(lastPlayed[curPos].getNumber() == Ace || lastPlayed[curPos].getNumber() == Two))
			{
				selectCards(2, i);
			}
		}
		else if (m_hand[i].getNumber() == m_hand[i+1].getNumber() 
			&& m_hand[i].getNumber() == lastPlayed[curPos].getNumber())
		{
			if (m_hand[i+1].getSuite() > lastPlayed[curPos].getSuite())
			{
				selectCards(2, i);
			}
		}
		else if (m_hand[i].getNumber() == m_hand[i+1].getNumber() 
			&&  m_hand[i].getNumber() < lastPlayed[curPos].getNumber())
		{
			if (m_hand[i].getNumber() == Ace && lastPlayed[curPos].getNumber() != Two)
			{
				selectCards(2, i);
			}
			else if (m_hand[i].getNumber() == Two)
			{
				selectCards(2, i);
			}
		}
	}

	// If we get to here that means we have to pass because we have no good cards
	if (!areCardsSelected())
		pass();

	return m_hand;
}

std::vector<Card> GameMove::playHighTriple(std::vector<Card> lastPlayed)
{
	if (lastPlayed.size() == 0)
		return playHighTriple();

	if (lastPlayed.size() == 1)
		return playBombAce();

	int curPos = 2;
	//for(int i = 0; i < m_hand.size() - curPos; i++)
	for (int i = m_hand.size() - curPos - 1; i >= 0; i--)
	{
		if (m_hand[i].getNumber() == m_hand[i+1].getNumber() && 
			m_hand[i+1].getNumber() == m_hand[i+2].getNumber() &&
			m_hand[i].getNumber() > lastPlayed[curPos].getNumber())
		{
			if (!(lastPlayed[curPos].getNumber() == Ace || lastPlayed[curPos].getNumber() == Two))
			{
				selectCards(3, i);
			}
		}
		else if (m_hand[i].getNumber() == m_hand[i+1].getNumber() && 
				 m_hand[i+1].getNumber() == m_hand[i+2].getNumber() &&
				 m_hand[i].getNumber() == lastPlayed[curPos].getNumber())
		{
			if (m_hand[i+2].getSuite() > lastPlayed[curPos].getSuite())
			{
				selectCards(3, i);
			}
		}
		else if (m_hand[i].getNumber() == m_hand[i+1].getNumber() && 
				 m_hand[i+1].getNumber() == m_hand[i+2].getNumber() &&
				 m_hand[i].getNumber() < lastPlayed[curPos].getNumber())
		{
			if (m_hand[i].getNumber() == Ace && lastPlayed[curPos].getNumber() != Two)
			{
				selectCards(3, i);
			}
			else if (m_hand[i].getNumber() == Two)
			{
				selectCards(3, i);
			}
		}
	}

	// If we get to here that means we have to pass because we have no good cards
	if (!areCardsSelected())
		pass();

	return m_hand;
}

std::vector<Card> GameMove::playHighStraight(std::vector<Card> lastPlayed)
{
	if (lastPlayed.size() == 0)
		return playHighStraight();

	// Get the last card played as we want to compare the highest values
	int szHand = lastPlayed.size();

	// Get the value of the last card for ease of reference
	int posLastCard = szHand - 1;

	// Find the size of the cards, then play the appropriate amount of cards
	// for that straight
	if (szHand == 3)
	{
		//for (int i = 0; i < m_hand.size() - posLastCard; i++)
		for (int i = m_hand.size() - posLastCard - 1; i >= 0; i--)
		{
			if (m_hand[i+2].getNumber() == m_hand[i+1].getNumber() + 1 && 
				m_hand[i+1].getNumber() == m_hand[i].getNumber() + 1)
			{
				if (m_hand[i+posLastCard].getNumber() > lastPlayed[posLastCard].getNumber() &&
					!(lastPlayed[posLastCard].getNumber() == Ace || lastPlayed[posLastCard].getNumber() == Two))
				{
					selectCards(szHand, i);
				}

				else if (m_hand[i+posLastCard].getNumber() == lastPlayed[posLastCard].getNumber() &&
					m_hand[i+posLastCard].getSuite() > lastPlayed[posLastCard].getSuite())
				{
					selectCards(szHand, i);
				}

				else if (m_hand[i+posLastCard].getNumber() < lastPlayed[posLastCard].getNumber() &&
					m_hand[i+posLastCard].getNumber() == Ace && lastPlayed[posLastCard].getNumber() != Two)
				{
					selectCards(szHand, i);
				}
			}
		}
	}
	else if (szHand == 4)
	{
		//for (int i = 0; i < m_hand.size() - posLastCard; i++)
		for (int i = m_hand.size() - posLastCard - 1; i >= 0; i--)
		{
			if (m_hand[i+3].getNumber() == m_hand[i+2].getNumber() + 1 && 
				m_hand[i+2].getNumber() == m_hand[i+1].getNumber() + 1 && 
				m_hand[i+1].getNumber() == m_hand[i].getNumber() + 1)
			{
				if (m_hand[i+posLastCard].getNumber() > lastPlayed[posLastCard].getNumber() &&
					!(lastPlayed[posLastCard].getNumber() == Ace || lastPlayed[posLastCard].getNumber() == Two))
				{
					selectCards(szHand, i);
				}

				else if (m_hand[i+posLastCard].getNumber() == lastPlayed[posLastCard].getNumber() &&
					m_hand[i+posLastCard].getSuite() > lastPlayed[posLastCard].getSuite())
				{
					selectCards(szHand, i);
				}

				else if (m_hand[i+posLastCard].getNumber() < lastPlayed[posLastCard].getNumber() &&
					m_hand[i+posLastCard].getNumber() == Ace && lastPlayed[posLastCard].getNumber() != Two)
				{
					selectCards(szHand, i);
				}
			}
		}
	}
	else if (szHand == 5)
	{
		//for (int i = 0; i < m_hand.size() - posLastCard; i++)
		for (int i = m_hand.size() - posLastCard - 1; i >= 0; i--)
		{
			if (m_hand[i+4].getNumber() == m_hand[i+3].getNumber() + 1 && 
				m_hand[i+3].getNumber() == m_hand[i+2].getNumber() + 1 && 
				m_hand[i+2].getNumber() == m_hand[i+1].getNumber() + 1 && 
				m_hand[i+1].getNumber() == m_hand[i].getNumber() + 1)
			{
				if (m_hand[i+posLastCard].getNumber() > lastPlayed[posLastCard].getNumber() &&
					!(lastPlayed[posLastCard].getNumber() == Ace || lastPlayed[posLastCard].getNumber() == Two))
				{
					selectCards(szHand, i);
				}

				else if (m_hand[i+posLastCard].getNumber() == lastPlayed[posLastCard].getNumber() &&
					m_hand[i+posLastCard].getSuite() > lastPlayed[posLastCard].getSuite())
				{
					selectCards(szHand, i);
				}

				else if (m_hand[i+posLastCard].getNumber() < lastPlayed[posLastCard].getNumber() &&
					m_hand[i+posLastCard].getNumber() == Ace && lastPlayed[posLastCard].getNumber() != Two)
				{
					selectCards(szHand, i);
				}
			}
		}
	}
	else if (szHand == 6)
	{
		//for (int i = 0; i < m_hand.size() - posLastCard; i++)
		for (int i = m_hand.size() - posLastCard - 1; i >= 0; i--)
		{
			if (m_hand[i+5].getNumber() == m_hand[i+4].getNumber() + 1 && 
				m_hand[i+4].getNumber() == m_hand[i+3].getNumber() + 1 && 
				m_hand[i+3].getNumber() == m_hand[i+2].getNumber() + 1 && 
				m_hand[i+2].getNumber() == m_hand[i+1].getNumber() + 1 && 
				m_hand[i+1].getNumber() == m_hand[i].getNumber() + 1)
			{
				if (m_hand[i+posLastCard].getNumber() > lastPlayed[posLastCard].getNumber() &&
					!(lastPlayed[posLastCard].getNumber() == Ace || lastPlayed[szHand].getNumber() == Two))
				{
					selectCards(szHand, i);
				}

				else if (m_hand[i+posLastCard].getNumber() == lastPlayed[posLastCard].getNumber() &&
					m_hand[i+posLastCard].getSuite() > lastPlayed[posLastCard].getSuite())
				{
					selectCards(szHand, i);
				}

				else if (m_hand[i+posLastCard].getNumber() < lastPlayed[posLastCard].getNumber() &&
					m_hand[i+posLastCard].getNumber() == Ace && lastPlayed[posLastCard].getNumber() != Two)
				{
					selectCards(szHand, i);
				}
			}
		}
	}
	else if (szHand == 7)
	{
		//for (int i = 0; i < m_hand.size() - posLastCard; i++)
		for (int i = m_hand.size() - posLastCard - 1; i >= 0; i--)
		{
			if (m_hand[i+6].getNumber() == m_hand[i+5].getNumber() + 1 && 
				m_hand[i+5].getNumber() == m_hand[i+4].getNumber() + 1 && 
				m_hand[i+4].getNumber() == m_hand[i+3].getNumber() + 1 && 
				m_hand[i+3].getNumber() == m_hand[i+2].getNumber() + 1 && 
				m_hand[i+2].getNumber() == m_hand[i+1].getNumber() + 1 && 
				m_hand[i+1].getNumber() == m_hand[i].getNumber() + 1)
			{
				if (m_hand[i+posLastCard].getNumber() > lastPlayed[posLastCard].getNumber() &&
					!(lastPlayed[posLastCard].getNumber() == Ace || lastPlayed[posLastCard].getNumber() == Two))
				{
					selectCards(szHand, i);
				}

				else if (m_hand[i+posLastCard].getNumber() == lastPlayed[posLastCard].getNumber() &&
					m_hand[i+posLastCard].getSuite() > lastPlayed[posLastCard].getSuite())
				{
					selectCards(szHand, i);
				}

				else if (m_hand[i+posLastCard].getNumber() < lastPlayed[posLastCard].getNumber() &&
					m_hand[i+posLastCard].getNumber() == Ace && lastPlayed[posLastCard].getNumber() != Two)
				{
					selectCards(szHand, i);
				}
			}
		}
	}
	else if (szHand == 8)
	{
		//for (int i = 0; i < m_hand.size() - posLastCard; i++)
		for (int i = m_hand.size() - posLastCard - 1; i >= 0; i--)
		{
			if (m_hand[i+7].getNumber() == m_hand[i+6].getNumber() + 1 && 
				m_hand[i+6].getNumber() == m_hand[i+5].getNumber() + 1 && 
				m_hand[i+5].getNumber() == m_hand[i+4].getNumber() + 1 && 
				m_hand[i+4].getNumber() == m_hand[i+3].getNumber() + 1 && 
				m_hand[i+3].getNumber() == m_hand[i+2].getNumber() + 1 && 
				m_hand[i+2].getNumber() == m_hand[i+1].getNumber() + 1 && 
				m_hand[i+1].getNumber() == m_hand[i].getNumber() + 1)
			{
				if (m_hand[i+posLastCard].getNumber() > lastPlayed[posLastCard].getNumber() &&
					!(lastPlayed[posLastCard].getNumber() == Ace || lastPlayed[posLastCard].getNumber() == Two))
				{
					selectCards(szHand, i);
				}

				else if (m_hand[i+posLastCard].getNumber() == lastPlayed[posLastCard].getNumber() &&
					m_hand[i+posLastCard].getSuite() > lastPlayed[posLastCard].getSuite())
				{
					selectCards(szHand, i);
				}

				else if (m_hand[i+posLastCard].getNumber() < lastPlayed[posLastCard].getNumber() &&
					m_hand[i+posLastCard].getNumber() == Ace && lastPlayed[posLastCard].getNumber() != Two)
				{
					selectCards(szHand, i);
				}
			}
		}
	}
	else if (szHand == 9)
	{
		//for (int i = 0; i < m_hand.size() - posLastCard; i++)
		for (int i = m_hand.size() - posLastCard - 1; i >= 0; i--)
		{
			if (m_hand[i+8].getNumber() == m_hand[i+7].getNumber() + 1 && 
				m_hand[i+7].getNumber() == m_hand[i+6].getNumber() + 1 && 
				m_hand[i+6].getNumber() == m_hand[i+5].getNumber() + 1 && 
				m_hand[i+5].getNumber() == m_hand[i+4].getNumber() + 1 && 
				m_hand[i+4].getNumber() == m_hand[i+3].getNumber() + 1 && 
				m_hand[i+3].getNumber() == m_hand[i+2].getNumber() + 1 && 
				m_hand[i+2].getNumber() == m_hand[i+1].getNumber() + 1 && 
				m_hand[i+1].getNumber() == m_hand[i].getNumber() + 1)
			{
				if (m_hand[i+posLastCard].getNumber() > lastPlayed[posLastCard].getNumber() &&
					!(lastPlayed[posLastCard].getNumber() == Ace || lastPlayed[posLastCard].getNumber() == Two))
				{
					selectCards(szHand, i);
				}

				else if (m_hand[i+posLastCard].getNumber() == lastPlayed[posLastCard].getNumber() &&
					m_hand[i+posLastCard].getSuite() > lastPlayed[posLastCard].getSuite())
				{
					selectCards(szHand, i);
				}

				else if (m_hand[i+posLastCard].getNumber() < lastPlayed[posLastCard].getNumber() &&
					m_hand[i+posLastCard].getNumber() == Ace && lastPlayed[posLastCard].getNumber() != Two)
				{
					selectCards(szHand, i);
				}
			}
		}
	}
	else if (szHand == 10)
	{
		//for (int i = 0; i < m_hand.size() - posLastCard; i++)
		for (int i = m_hand.size() - posLastCard - 1; i >= 0; i--)
		{
			if (m_hand[i+9].getNumber() == m_hand[i+8].getNumber() + 1 && 
				m_hand[i+8].getNumber() == m_hand[i+7].getNumber() + 1 && 
				m_hand[i+7].getNumber() == m_hand[i+6].getNumber() + 1 && 
				m_hand[i+6].getNumber() == m_hand[i+5].getNumber() + 1 && 
				m_hand[i+5].getNumber() == m_hand[i+4].getNumber() + 1 && 
				m_hand[i+4].getNumber() == m_hand[i+3].getNumber() + 1 && 
				m_hand[i+3].getNumber() == m_hand[i+2].getNumber() + 1 && 
				m_hand[i+2].getNumber() == m_hand[i+1].getNumber() + 1 && 
				m_hand[i+1].getNumber() == m_hand[i].getNumber() + 1)
			{
				if (m_hand[i+posLastCard].getNumber() > lastPlayed[posLastCard].getNumber() &&
					!(lastPlayed[posLastCard].getNumber() == Ace || lastPlayed[posLastCard].getNumber() == Two))
				{
					selectCards(szHand, i);
				}

				else if (m_hand[i+posLastCard].getNumber() == lastPlayed[posLastCard].getNumber() &&
					m_hand[i+posLastCard].getSuite() > lastPlayed[posLastCard].getSuite())
				{
					selectCards(szHand, i);
				}

				else if (m_hand[i+posLastCard].getNumber() < lastPlayed[posLastCard].getNumber() &&
					m_hand[i+posLastCard].getNumber() == Ace && lastPlayed[posLastCard].getNumber() != Two)
				{
					selectCards(szHand, i);
				}
			}
		}
	}
	else if (szHand == 11)
	{
		//for (int i = 0; i < m_hand.size() - posLastCard; i++)
		for (int i = m_hand.size() - posLastCard - 1; i >= 0; i--)
		{
			if (m_hand[i+10].getNumber() == m_hand[i+9].getNumber() + 1 && 
				m_hand[i+9].getNumber() == m_hand[i+8].getNumber() + 1 && 
				m_hand[i+8].getNumber() == m_hand[i+7].getNumber() + 1 && 
				m_hand[i+7].getNumber() == m_hand[i+6].getNumber() + 1 && 
				m_hand[i+6].getNumber() == m_hand[i+5].getNumber() + 1 && 
				m_hand[i+5].getNumber() == m_hand[i+4].getNumber() + 1 && 
				m_hand[i+4].getNumber() == m_hand[i+3].getNumber() + 1 && 
				m_hand[i+3].getNumber() == m_hand[i+2].getNumber() + 1 && 
				m_hand[i+2].getNumber() == m_hand[i+1].getNumber() + 1 && 
				m_hand[i+1].getNumber() == m_hand[i].getNumber() + 1)
			{
				if (m_hand[i+posLastCard].getNumber() > lastPlayed[posLastCard].getNumber() &&
					!(lastPlayed[posLastCard].getNumber() == Ace || lastPlayed[posLastCard].getNumber() == Two))
				{
					selectCards(szHand, i);
				}
				else if (m_hand[i+posLastCard].getNumber() == lastPlayed[posLastCard].getNumber() &&
					m_hand[i+posLastCard].getSuite() > lastPlayed[posLastCard].getSuite())
				{
					selectCards(szHand, i);
				}
				else if (m_hand[i+posLastCard].getNumber() < lastPlayed[posLastCard].getNumber() &&
					m_hand[i+posLastCard].getNumber() == Ace && lastPlayed[posLastCard].getNumber() != Two)
				{
					selectCards(szHand, i);
				}
			}
		}
	}
	else if (szHand == 12)
	{
		//for (int i = 0; i < m_hand.size() - szHand; i++)
		for (int i = m_hand.size() - posLastCard - 1; i >= 0; i--)
		{
			if (m_hand[i+11].getNumber() == m_hand[i+10].getNumber() + 1 && 
				m_hand[i+10].getNumber() == m_hand[i+9].getNumber() + 1 && 
				m_hand[i+9].getNumber() == m_hand[i+8].getNumber() + 1 && 
				m_hand[i+8].getNumber() == m_hand[i+7].getNumber() + 1 && 
				m_hand[i+7].getNumber() == m_hand[i+6].getNumber() + 1 && 
				m_hand[i+6].getNumber() == m_hand[i+5].getNumber() + 1 && 
				m_hand[i+5].getNumber() == m_hand[i+4].getNumber() + 1 && 
				m_hand[i+4].getNumber() == m_hand[i+3].getNumber() + 1 && 
				m_hand[i+3].getNumber() == m_hand[i+2].getNumber() + 1 && 
				m_hand[i+2].getNumber() == m_hand[i+1].getNumber() + 1 && 
				m_hand[i+1].getNumber() == m_hand[i].getNumber() + 1)
			{
				if (m_hand[i+posLastCard].getNumber() > lastPlayed[posLastCard].getNumber() &&
					!(lastPlayed[posLastCard].getNumber() == Ace || lastPlayed[posLastCard].getNumber() == Two))
				{
					selectCards(szHand, i);
				}

				else if (m_hand[i+posLastCard].getNumber() == lastPlayed[posLastCard].getNumber() &&
					m_hand[i+posLastCard].getSuite() > lastPlayed[posLastCard].getSuite())
				{
					selectCards(szHand, i);
				}

				else if (m_hand[i+posLastCard].getNumber() < lastPlayed[posLastCard].getNumber() &&
					m_hand[i+posLastCard].getNumber() == Ace && lastPlayed[posLastCard].getNumber() != Two)
				{
					selectCards(szHand, i);
				}
			}
		}
	}
		// Straight can be 3 Cards to 13
		// Search for a straight of the size of the played Card
		// Then check the highest card values against each other
		// Then determine to play or pass
		

	// If we get to here that means we have to pass because we have no good cards
	if (!areCardsSelected())
		pass();

	return m_hand;
}

std::vector<Card> GameMove::playHighPairStraight(std::vector<Card> lastPlayed)
{
	if (lastPlayed.size() == 0)
		return playHighPairStraight();

	if (lastPlayed.size() == 1)
		return playBombTwo();

	// Get the last card played as we want to compare the highest values
	int szHand = lastPlayed.size();

	// Get the value of the last card for ease of reference
	int posLastCard = szHand - 1;

	// Find the size of the cards, then play the appropriate amount of cards
	// for that straight
	if (szHand == 6)
	{
		//for (int i = 0; i < m_hand.size() - posLastCard; i++)
		for (int i = m_hand.size() - posLastCard - 2; i >= 0; i--)
		{
			if (m_hand[i+6].getNumber() == m_hand[i+4].getNumber() + 1 && 
				m_hand[i+4].getNumber() == m_hand[i+2].getNumber() + 1 &&
				m_hand[i+2].getNumber() == m_hand[i].getNumber() + 1)
			{
				if (m_hand[i+posLastCard].getNumber() > lastPlayed[posLastCard].getNumber() &&
					!(lastPlayed[posLastCard].getNumber() == Ace || lastPlayed[posLastCard].getNumber() == Two))
				{
					selectCards(szHand, i);
				}

				else if (m_hand[i+posLastCard].getNumber() == lastPlayed[posLastCard].getNumber() &&
					m_hand[i+posLastCard].getSuite() > lastPlayed[posLastCard].getSuite())
				{
					selectCards(szHand, i);
				}

				else if (m_hand[i+posLastCard].getNumber() < lastPlayed[posLastCard].getNumber() &&
					m_hand[i+posLastCard].getNumber() == Ace && lastPlayed[posLastCard].getNumber() != Two)
				{
					selectCards(szHand, i);
				}
			}
		}
	}
	if (szHand == 8)
	{
		for (int i = m_hand.size() - posLastCard - 2; i >= 0; i--)
		{
			if (m_hand[i+8].getNumber() == m_hand[i+6].getNumber() + 1 && 
				m_hand[i+6].getNumber() == m_hand[i+4].getNumber() + 1 && 
				m_hand[i+4].getNumber() == m_hand[i+2].getNumber() + 1 &&
				m_hand[i+2].getNumber() == m_hand[i].getNumber() + 1)
			{
				if (m_hand[i+posLastCard].getNumber() > lastPlayed[posLastCard].getNumber() &&
					!(lastPlayed[posLastCard].getNumber() == Ace || lastPlayed[posLastCard].getNumber() == Two))
				{
					selectCards(szHand, i);
				}

				else if (m_hand[i+posLastCard].getNumber() == lastPlayed[posLastCard].getNumber() &&
					m_hand[i+posLastCard].getSuite() > lastPlayed[posLastCard].getSuite())
				{
					selectCards(szHand, i);
				}

				else if (m_hand[i+posLastCard].getNumber() < lastPlayed[posLastCard].getNumber() &&
					m_hand[i+posLastCard].getNumber() == Ace && lastPlayed[posLastCard].getNumber() != Two)
				{
					selectCards(szHand, i);
				}
			}
		}
	}
	if (szHand == 10)
	{
		for (int i = m_hand.size() - posLastCard - 2; i >= 0; i--)
		{
			if (m_hand[i+10].getNumber() == m_hand[i+8].getNumber() + 1 && 
				m_hand[i+8].getNumber() == m_hand[i+6].getNumber() + 1 && 
				m_hand[i+6].getNumber() == m_hand[i+4].getNumber() + 1 && 
				m_hand[i+4].getNumber() == m_hand[i+2].getNumber() + 1 &&
				m_hand[i+2].getNumber() == m_hand[i].getNumber() + 1)
			{
				if (m_hand[i+posLastCard].getNumber() > lastPlayed[posLastCard].getNumber() &&
					!(lastPlayed[posLastCard].getNumber() == Ace || lastPlayed[posLastCard].getNumber() == Two))
				{
					selectCards(szHand, i);
				}

				else if (m_hand[i+posLastCard].getNumber() == lastPlayed[posLastCard].getNumber() &&
					m_hand[i+posLastCard].getSuite() > lastPlayed[posLastCard].getSuite())
				{
					selectCards(szHand, i);
				}

				else if (m_hand[i+posLastCard].getNumber() < lastPlayed[posLastCard].getNumber() &&
					m_hand[i+posLastCard].getNumber() == Ace && lastPlayed[posLastCard].getNumber() != Two)
				{
					selectCards(szHand, i);
				}
			}
		}
	}
	if (szHand == 12)
	{
		for (int i = m_hand.size() - posLastCard - 2; i >= 0; i--)
		{
			if (m_hand[i+12].getNumber() == m_hand[i+10].getNumber() + 1 && 
				m_hand[i+10].getNumber() == m_hand[i+8].getNumber() + 1 && 
				m_hand[i+8].getNumber() == m_hand[i+6].getNumber() + 1 && 
				m_hand[i+6].getNumber() == m_hand[i+4].getNumber() + 1 && 
				m_hand[i+4].getNumber() == m_hand[i+2].getNumber() + 1 &&
				m_hand[i+2].getNumber() == m_hand[i].getNumber() + 1)
			{
				if (m_hand[i+posLastCard].getNumber() > lastPlayed[posLastCard].getNumber() &&
					!(lastPlayed[posLastCard].getNumber() == Ace || lastPlayed[posLastCard].getNumber() == Two))
				{
					selectCards(szHand, i);
				}

				else if (m_hand[i+posLastCard].getNumber() == lastPlayed[posLastCard].getNumber() &&
					m_hand[i+posLastCard].getSuite() > lastPlayed[posLastCard].getSuite())
				{
					selectCards(szHand, i);
				}

				else if (m_hand[i+posLastCard].getNumber() < lastPlayed[posLastCard].getNumber() &&
					m_hand[i+posLastCard].getNumber() == Ace && lastPlayed[posLastCard].getNumber() != Two)
				{
					selectCards(szHand, i);
				}
			}
		}
	}

	if (!areCardsSelected())
		pass();

	return m_hand;
}

std::vector<Card> GameMove::playHighFourKind(std::vector<Card> lastPlayed)
{
	if (lastPlayed.size() == 0)
		return playHighFourKind();

	if (lastPlayed.size() == 1)
		return playBombTwo();

	int curPos = 3;
	//for(int i = 0; i < m_hand.size() - curPos; i++)
	for (int i = m_hand.size() - curPos - 1; i >= 0; i--)
	{
		if (m_hand[i].getNumber() == m_hand[i+1].getNumber() && 
			m_hand[i+1].getNumber() == m_hand[i+2].getNumber() && 
			m_hand[i+2].getNumber() == m_hand[i+3].getNumber())
		{
			if (m_hand[i].getNumber() > lastPlayed[curPos].getNumber() &&
				!(lastPlayed[curPos].getNumber() == Ace || lastPlayed[curPos].getNumber() == Two))
			{
				selectCards(4, i);
			}

			else if (m_hand[i].getNumber() == lastPlayed[curPos].getNumber() &&
				m_hand[i+2].getSuite() > lastPlayed[curPos].getSuite())
			{
				selectCards(4, i);
			}

			else if (m_hand[i].getNumber() < lastPlayed[curPos].getNumber())
			{
				if (m_hand[i].getNumber() == Ace && lastPlayed[curPos].getNumber() != Two)
				{
					selectCards(4, i);
				}
				else if (m_hand[i].getNumber() == Two)
				{
					selectCards(4, i);
				}
			}
		}
	}

	// If we get to here that means we have to pass because we have no good cards
	if (!areCardsSelected())
		pass();

	return m_hand;
}

std::vector<Card> GameMove::playHighSingle()
{
	selectCards(1, m_hand.size()-1);

	// If we get to here that means we have to pass because we have no good cards
	if (!areCardsSelected())
		pass();

	return m_hand;
}

std::vector<Card> GameMove::playHighPair()
{
	for(int i = m_hand.size() - 2; i >= 0 ; i--)
	{
		if (m_hand[i].getNumber() == m_hand[i+1].getNumber())
		{
			selectCards(2, i);
		}
	}

	// If we get to here that means we have to pass because we have no good cards
	if (!areCardsSelected())
		pass();

	return m_hand;
}

std::vector<Card> GameMove::playHighTriple()
{
	for (int i = m_hand.size() - 3; i >= 0; i--)
	{
		if (m_hand[i].getNumber() == m_hand[i+1].getNumber() && 
			m_hand[i].getNumber() == m_hand[i+2].getNumber())
		{
			selectCards(3, i);
		}
	}

	// If we get to here that means we have to pass because we have no good cards
	if (!areCardsSelected())
		pass();

	return m_hand;
}

std::vector<Card> GameMove::playHighStraight()
{
	for (int i = m_hand.size() - 3; i >= 0; i--)
	{
		if (m_hand[i+2].getNumber() == m_hand[i+1].getNumber() + 1 && 
			m_hand[i+1].getNumber() == m_hand[i].getNumber() + 1)
		{
			selectCards(3, i);
		}
		else if (m_hand[i+2].getNumber() == Ace && m_hand[1+1].getNumber() == King && m_hand[i].getNumber() == Queen)
		{
			selectCards(3, i);
		}
	}
		

	// If we get to here that means we have to pass because we have no good cards
	if (!areCardsSelected())
		pass();

	return m_hand;
}

std::vector<Card> GameMove::playHighPairStraight()
{
	for (int i = m_hand.size() - 6; i >= 0; i--)
	{
		// 33; 44; 55;
		// i,1; 2,3; 4,5;
		if (m_hand[i].getNumber() == m_hand[i+1].getNumber() &&
			m_hand[i+2].getNumber() == m_hand[i+3].getNumber() &&
			m_hand[i+2].getNumber() == m_hand[i].getNumber() + 1 &&
			m_hand[i+4].getNumber() == m_hand[i+5].getNumber() &&
			m_hand[i+4].getNumber() == m_hand[i+2].getNumber() + 1)
		{
			selectCards(6, i);
		}
	}

	if (!areCardsSelected())
		pass();

	return m_hand;
}

std::vector<Card> GameMove::playHighFourKind()
{
	for (int i = m_hand.size() - 4; i >= 0; i--)
	{
		if (m_hand[i].getNumber() == m_hand[i+1].getNumber() && 
			m_hand[i].getNumber() == m_hand[i+2].getNumber() && 
			m_hand[i].getNumber() == m_hand[i+3].getNumber())
		{
			selectCards(4, i);
		}
	}

	// If we get to here that means we have to pass because we have no good cards
	if (!areCardsSelected())
		pass();

	return m_hand;
}

bool GameMove::hasTwoInHand()
{
	for (int i = m_hand.size()-1; i >= 0; i--)
	{
		if (m_hand[i].getNumber() == Two)
			return true;
	}

	return false;
}

// ----------------------------------------
// playSingle
// 
// Plays the hand of type Single
// Input:
//  none
// Output:
//  none
// Conditions:
//  none
std::vector<Card> GameMove::playSingle()
{
	selectCards(1, 0);
	return m_hand;
}

// ----------------------------------------
// playPair
// 
// Plays the hand of type Pair
// Input:
//  none
// Output:
//  none
// Conditions:
//  none
std::vector<Card> GameMove::playPair()
{
	// 01234; 5-1=4; 4+1=5;
	for(int i = 0; i < m_hand.size() - 1; i++)
	{
		if (m_hand[i].getNumber() == m_hand[i+1].getNumber())
		{
			selectCards(2, i);
		}
	}

	return m_hand;
}

// ----------------------------------------
// playTriple
// 
// Plays the hand of type Triple
// Input:
//  none
// Output:
//  none
// Conditions:
//  none
// TODO: Implement bombing of an Ace
std::vector<Card> GameMove::playTriple()
{
	// 01234; 5-3=2;
	for (int i = 0; i < m_hand.size() - 2; i++)
	{
		if (m_hand[i].getNumber() == m_hand[i+1].getNumber() && 
			m_hand[i].getNumber() == m_hand[i+2].getNumber())
		{
			selectCards(3, i);
		}
	}

	return m_hand;
}

// ----------------------------------------
// playFourKind
// 
// Plays the hand of type FourKind
// Input:
//  none
// Output:
//  none
// Conditions:
//  none
std::vector<Card> GameMove::playFourKind()
{
	// 01234; 5-4=1
	for (int i = 0; i < m_hand.size() - 3; i++)
	{
		if (m_hand[i].getNumber() == m_hand[i+1].getNumber() && 
			m_hand[i].getNumber() == m_hand[i+2].getNumber() && 
			m_hand[i].getNumber() == m_hand[i+3].getNumber())
		{
			selectCards(4, i);
		}
	}

	return m_hand;
}

// ----------------------------------------
// playStraight
// 
// Plays the hand of type Straight
// Input:
//  none
// Output:
//  none
// Conditions:
//  none
// TODO: Implment DRAGON (3spade-2anysuit)
std::vector<Card> GameMove::playStraight()
{
	int szHand = m_hand.size();

	if (szHand == 13)
	{
			if (m_hand[12].getNumber() == Two && 
				m_hand[11].getNumber() == Ace && 
				m_hand[10].getNumber() == m_hand[9].getNumber() + 1 && 
				m_hand[9].getNumber() == m_hand[8].getNumber() + 1 && 
				m_hand[8].getNumber() == m_hand[7].getNumber() + 1 && 
				m_hand[7].getNumber() == m_hand[6].getNumber() + 1 && 
				m_hand[6].getNumber() == m_hand[5].getNumber() + 1 && 
				m_hand[5].getNumber() == m_hand[4].getNumber() + 1 && 
				m_hand[4].getNumber() == m_hand[3].getNumber() + 1 && 
				m_hand[3].getNumber() == m_hand[2].getNumber() + 1 && 
				m_hand[2].getNumber() == m_hand[1].getNumber() + 1 && 
				m_hand[1].getNumber() == m_hand[0].getNumber() + 1)
			{
				selectCards(13, 0);
				return m_hand;
			}
	}
	if (szHand >= 12)
	{
		for (int i = 0; i < m_hand.size() - 11; i++)
		{
			if (m_hand[i+11].getNumber() == Ace && 
				m_hand[i+10].getNumber() == m_hand[i+9].getNumber() + 1 && 
				m_hand[i+9].getNumber() == m_hand[i+8].getNumber() + 1 && 
				m_hand[i+8].getNumber() == m_hand[i+7].getNumber() + 1 && 
				m_hand[i+7].getNumber() == m_hand[i+6].getNumber() + 1 && 
				m_hand[i+6].getNumber() == m_hand[i+5].getNumber() + 1 && 
				m_hand[i+5].getNumber() == m_hand[i+4].getNumber() + 1 && 
				m_hand[i+4].getNumber() == m_hand[i+3].getNumber() + 1 && 
				m_hand[i+3].getNumber() == m_hand[i+2].getNumber() + 1 && 
				m_hand[i+2].getNumber() == m_hand[i+1].getNumber() + 1 && 
				m_hand[i+1].getNumber() == m_hand[i].getNumber() + 1)
			{
				selectCards(12, i);
				return m_hand;
			}
			else if (m_hand[i+11].getNumber() == Ace && 
					 m_hand[1+10].getNumber() == King && 
					 m_hand[i+9].getNumber() == Queen &&
					 m_hand[i+8].getNumber() == Jack &&
					 m_hand[i+7].getNumber() == Ten &&
					 m_hand[i+6].getNumber() == Nine &&
					 m_hand[i+5].getNumber() == Eight &&
					 m_hand[i+4].getNumber() == Seven &&
					 m_hand[i+3].getNumber() == Six &&
					 m_hand[i+2].getNumber() == Five &&
					 m_hand[i+1].getNumber() == Four &&
					 m_hand[i].getNumber() == Three)
			{
				selectCards(12, i);
				return m_hand;
			}
		}
	}
	if (szHand >= 11)
	{
		for (int i = 0; i < m_hand.size() - 10; i++)
		{
			if (m_hand[i+10].getNumber() == m_hand[i+9].getNumber() + 1 && 
				m_hand[i+9].getNumber() == m_hand[i+8].getNumber() + 1 && 
				m_hand[i+8].getNumber() == m_hand[i+7].getNumber() + 1 && 
				m_hand[i+7].getNumber() == m_hand[i+6].getNumber() + 1 && 
				m_hand[i+6].getNumber() == m_hand[i+5].getNumber() + 1 && 
				m_hand[i+5].getNumber() == m_hand[i+4].getNumber() + 1 && 
				m_hand[i+4].getNumber() == m_hand[i+3].getNumber() + 1 && 
				m_hand[i+3].getNumber() == m_hand[i+2].getNumber() + 1 && 
				m_hand[i+2].getNumber() == m_hand[i+1].getNumber() + 1 && 
				m_hand[i+1].getNumber() == m_hand[i].getNumber() + 1)
			{
				selectCards(11, i);
				return m_hand;
			}
			else if (m_hand[i+10].getNumber() == Ace && 
					 m_hand[1+9].getNumber() == King && 
					 m_hand[i+8].getNumber() == Queen &&
					 m_hand[i+7].getNumber() == Jack &&
					 m_hand[i+6].getNumber() == Ten &&
					 m_hand[i+5].getNumber() == Nine &&
					 m_hand[i+4].getNumber() == Eight &&
					 m_hand[i+3].getNumber() == Seven &&
					 m_hand[i+2].getNumber() == Six &&
					 m_hand[i+1].getNumber() == Five &&
					 m_hand[i].getNumber() == Four)
			{
				selectCards(11, i);
				return m_hand;
			}
		}
	}
	if (szHand >= 10)
	{
		for (int i = 0; i < m_hand.size() - 9; i++)
		{
			if (m_hand[i+9].getNumber() == m_hand[i+8].getNumber() + 1 && 
				m_hand[i+8].getNumber() == m_hand[i+7].getNumber() + 1 && 
				m_hand[i+7].getNumber() == m_hand[i+6].getNumber() + 1 && 
				m_hand[i+6].getNumber() == m_hand[i+5].getNumber() + 1 && 
				m_hand[i+5].getNumber() == m_hand[i+4].getNumber() + 1 && 
				m_hand[i+4].getNumber() == m_hand[i+3].getNumber() + 1 && 
				m_hand[i+3].getNumber() == m_hand[i+2].getNumber() + 1 && 
				m_hand[i+2].getNumber() == m_hand[i+1].getNumber() + 1 && 
				m_hand[i+1].getNumber() == m_hand[i].getNumber() + 1)
			{
				selectCards(10, i);
				return m_hand;
			}
			else if (m_hand[i+9].getNumber() == Ace && 
					 m_hand[1+8].getNumber() == King && 
					 m_hand[i+7].getNumber() == Queen &&
					 m_hand[i+6].getNumber() == Jack &&
					 m_hand[i+5].getNumber() == Ten &&
					 m_hand[i+4].getNumber() == Nine &&
					 m_hand[i+3].getNumber() == Eight &&
					 m_hand[i+2].getNumber() == Seven &&
					 m_hand[i+1].getNumber() == Six &&
					 m_hand[i].getNumber() == Five)
			{
				selectCards(10, i);
				return m_hand;
			}
		}
	}
	if (szHand >= 9)
	{
		for (int i = 0; i < m_hand.size() - 8; i++)
		{
			if (m_hand[i+8].getNumber() == m_hand[i+7].getNumber() + 1 && 
				m_hand[i+7].getNumber() == m_hand[i+6].getNumber() + 1 && 
				m_hand[i+6].getNumber() == m_hand[i+5].getNumber() + 1 && 
				m_hand[i+5].getNumber() == m_hand[i+4].getNumber() + 1 && 
				m_hand[i+4].getNumber() == m_hand[i+3].getNumber() + 1 && 
				m_hand[i+3].getNumber() == m_hand[i+2].getNumber() + 1 && 
				m_hand[i+2].getNumber() == m_hand[i+1].getNumber() + 1 && 
				m_hand[i+1].getNumber() == m_hand[i].getNumber() + 1)
			{
				selectCards(9, i);
				return m_hand;
			}
			else if (m_hand[i+8].getNumber() == Ace && 
					 m_hand[1+7].getNumber() == King && 
					 m_hand[i+6].getNumber() == Queen &&
					 m_hand[i+5].getNumber() == Jack &&
					 m_hand[i+4].getNumber() == Ten &&
					 m_hand[i+3].getNumber() == Nine &&
					 m_hand[i+2].getNumber() == Eight &&
					 m_hand[i+1].getNumber() == Seven &&
					 m_hand[i].getNumber() == Six)
			{
				selectCards(9, i);
				return m_hand;
			}
		}
	}
	if (szHand >= 8)
	{
		for (int i = 0; i < m_hand.size() - 7; i++)
		{
			if (m_hand[i+7].getNumber() == m_hand[i+6].getNumber() + 1 && 
				m_hand[i+6].getNumber() == m_hand[i+5].getNumber() + 1 && 
				m_hand[i+5].getNumber() == m_hand[i+4].getNumber() + 1 && 
				m_hand[i+4].getNumber() == m_hand[i+3].getNumber() + 1 && 
				m_hand[i+3].getNumber() == m_hand[i+2].getNumber() + 1 && 
				m_hand[i+2].getNumber() == m_hand[i+1].getNumber() + 1 && 
				m_hand[i+1].getNumber() == m_hand[i].getNumber() + 1)
			{
				selectCards(8, i);
				return m_hand;
			}
			else if (m_hand[i+7].getNumber() == Ace && 
					 m_hand[1+6].getNumber() == King && 
					 m_hand[i+5].getNumber() == Queen &&
					 m_hand[i+4].getNumber() == Jack &&
					 m_hand[i+3].getNumber() == Ten &&
					 m_hand[i+2].getNumber() == Nine &&
					 m_hand[i+1].getNumber() == Eight &&
					 m_hand[i].getNumber() == Seven)
			{
				selectCards(8, i);
				return m_hand;
			}
		}
	}
	if (szHand >= 7)
	{
		for (int i = 0; i < m_hand.size() - 6; i++)
		{
			if (m_hand[i+6].getNumber() == m_hand[i+5].getNumber() + 1 && 
				m_hand[i+5].getNumber() == m_hand[i+4].getNumber() + 1 && 
				m_hand[i+4].getNumber() == m_hand[i+3].getNumber() + 1 && 
				m_hand[i+3].getNumber() == m_hand[i+2].getNumber() + 1 && 
				m_hand[i+2].getNumber() == m_hand[i+1].getNumber() + 1 && 
				m_hand[i+1].getNumber() == m_hand[i].getNumber() + 1)
			{
				selectCards(7, i);
				return m_hand;
			}
			else if (m_hand[i+6].getNumber() == Ace && 
					 m_hand[1+5].getNumber() == King && 
					 m_hand[i+4].getNumber() == Queen &&
					 m_hand[i+3].getNumber() == Jack &&
					 m_hand[i+2].getNumber() == Ten &&
					 m_hand[i+1].getNumber() == Nine &&
					 m_hand[i].getNumber() == Eight)
			{
				selectCards(7, i);
				return m_hand;
			}
		}
	}
	if (szHand >= 6)
	{
		for (int i = 0; i < m_hand.size() - 5; i++)
		{
			if (m_hand[i+5].getNumber() == m_hand[i+4].getNumber() + 1 && 
				m_hand[i+4].getNumber() == m_hand[i+3].getNumber() + 1 && 
				m_hand[i+3].getNumber() == m_hand[i+2].getNumber() + 1 && 
				m_hand[i+2].getNumber() == m_hand[i+1].getNumber() + 1 && 
				m_hand[i+1].getNumber() == m_hand[i].getNumber() + 1)
			{
				selectCards(6, i);
				return m_hand;
			}
			else if (m_hand[i+5].getNumber() == Ace && 
					 m_hand[1+4].getNumber() == King && 
					 m_hand[i+3].getNumber() == Queen &&
					 m_hand[i+2].getNumber() == Jack &&
					 m_hand[i+1].getNumber() == Ten &&
					 m_hand[i].getNumber() == Nine)
			{
				selectCards(6, i);
				return m_hand;
			}
		}
	}
	if (szHand >= 5)
	{
		for (int i = 0; i < m_hand.size() - 4; i++)
		{
			if (m_hand[i+4].getNumber() == m_hand[i+3].getNumber() + 1 && 
				m_hand[i+3].getNumber() == m_hand[i+2].getNumber() + 1 && 
				m_hand[i+2].getNumber() == m_hand[i+1].getNumber() + 1 && 
				m_hand[i+1].getNumber() == m_hand[i].getNumber() + 1)
			{
				selectCards(5, i);
				return m_hand;
			}
			else if (m_hand[i+4].getNumber() == Ace && 
					 m_hand[1+3].getNumber() == King && 
					 m_hand[i+2].getNumber() == Queen &&
					 m_hand[i+1].getNumber() == Jack &&
					 m_hand[i].getNumber() == Ten)
			{
				selectCards(5, i);
				return m_hand;
			}
		}
	}
	if (szHand >= 4)
	{
		for (int i = 0; i < m_hand.size() - 3; i++)
		{
			if (m_hand[i+3].getNumber() == m_hand[i+2].getNumber() + 1 && 
				m_hand[i+2].getNumber() == m_hand[i+1].getNumber() + 1 && 
				m_hand[i+1].getNumber() == m_hand[i].getNumber() + 1)
			{
				selectCards(4, i);
				return m_hand;
			}
			else if (m_hand[i+3].getNumber() == Ace && 
					 m_hand[1+2].getNumber() == King && 
					 m_hand[i+1].getNumber() == Queen &&
					 m_hand[i].getNumber() == Jack)
			{
				selectCards(4, i);
				return m_hand;
			}
		}
	}
	if (szHand >= 3)
	{
		//5; 5-3=2; 012 34; 
		for (int i = 0; i < m_hand.size() - 2; i++)
		{
			if (m_hand[i+2].getNumber() == m_hand[i+1].getNumber() + 1 && 
				m_hand[i+1].getNumber() == m_hand[i].getNumber() + 1)
			{
				selectCards(3, i);
				return m_hand;
			}
			else if (m_hand[i+2].getNumber() == Ace && m_hand[1+1].getNumber() == King && m_hand[i].getNumber() == Queen)
			{
				selectCards(3, i);
				return m_hand;
			}
		}
	}
	
	

	return m_hand;
}

// ----------------------------------------
// playPairStraight
// 
// Plays the hand of type PairStraight
// Input:
//  none
// Output:
//  none
// Conditions:
//  none
// TODO: Implement bombing of Pair 2's [Done elsewhere]
// TODO: Add in special case for QKA [Done]
std::vector<Card> GameMove::playPairStraight()
{
	// Need min 6, max 13
	int szHand = m_hand.size();
	if (szHand >= 12)
	{
		for (int i = 0; i < m_hand.size() - 11; i++)
		{
			// 33; 44; 55;
			// i,1; 2,3; 4,5; 6,7; 8,9; 10,11
			if (m_hand[i].getNumber() == m_hand[i+1].getNumber() &&
				m_hand[i+2].getNumber() == m_hand[i+3].getNumber() &&
				m_hand[i+2].getNumber() == m_hand[i].getNumber() + 1 &&
				m_hand[i+4].getNumber() == m_hand[i+5].getNumber() &&
				m_hand[i+4].getNumber() == m_hand[i+2].getNumber() + 1 &&
				m_hand[i+6].getNumber() == m_hand[i+7].getNumber() &&
				m_hand[i+6].getNumber() == m_hand[i+4].getNumber() + 1 &&
				m_hand[i+8].getNumber() == m_hand[i+9].getNumber() &&
				m_hand[i+8].getNumber() == m_hand[i+6].getNumber() + 1 &&
				m_hand[i+10].getNumber() == m_hand[i+11].getNumber() &&
				m_hand[i+10].getNumber() == m_hand[i+8].getNumber() + 1)
			{
				selectCards(12, i);
				return m_hand;
			}
			else if (m_hand[i].getNumber() == m_hand[i+1].getNumber() &&
				m_hand[i+2].getNumber() == m_hand[i+3].getNumber() &&
				m_hand[i+2].getNumber() == m_hand[i].getNumber() + 1 &&
				m_hand[i+4].getNumber() == m_hand[i+5].getNumber() &&
				m_hand[i+4].getNumber() == m_hand[i+2].getNumber() + 1 &&
				m_hand[i+6].getNumber() == m_hand[i+7].getNumber() &&
				m_hand[i+6].getNumber() == m_hand[i+4].getNumber() + 1 &&
				m_hand[i+8].getNumber() == m_hand[i+9].getNumber() &&
				m_hand[i+8].getNumber() == m_hand[i+6].getNumber() + 1 &&
				m_hand[i+10].getNumber() == m_hand[i+11].getNumber() &&
				m_hand[i+10].getNumber() == King)
			{
				selectCards(12, i);
				return m_hand;
			}
		}
	}
	if (szHand >= 10)
	{
		for (int i = 0; i < m_hand.size() - 9; i++)
		{
			// 33; 44; 55;
			// i,1; 2,3; 4,5; 6,7;
			if (m_hand[i].getNumber() == m_hand[i+1].getNumber() &&
				m_hand[i+2].getNumber() == m_hand[i+3].getNumber() &&
				m_hand[i+2].getNumber() == m_hand[i].getNumber() + 1 &&
				m_hand[i+4].getNumber() == m_hand[i+5].getNumber() &&
				m_hand[i+4].getNumber() == m_hand[i+2].getNumber() + 1 &&
				m_hand[i+6].getNumber() == m_hand[i+7].getNumber() &&
				m_hand[i+6].getNumber() == m_hand[i+4].getNumber() + 1 &&
				m_hand[i+8].getNumber() == m_hand[i+9].getNumber() &&
				m_hand[i+8].getNumber() == m_hand[i+6].getNumber() + 1)
			{
				selectCards(10, i);
				return m_hand;
			}
			else if (m_hand[i].getNumber() == m_hand[i+1].getNumber() &&
				m_hand[i+2].getNumber() == m_hand[i+3].getNumber() &&
				m_hand[i+2].getNumber() == m_hand[i].getNumber() + 1 &&
				m_hand[i+4].getNumber() == m_hand[i+5].getNumber() &&
				m_hand[i+4].getNumber() == m_hand[i+2].getNumber() + 1 &&
				m_hand[i+6].getNumber() == m_hand[i+7].getNumber() &&
				m_hand[i+6].getNumber() == m_hand[i+4].getNumber() + 1 &&
				m_hand[i+8].getNumber() == m_hand[i+9].getNumber() &&
				m_hand[i+8].getNumber() == King)
			{
				selectCards(10, i);
				return m_hand;
			}
		}
	}
	if (szHand >= 8)
	{
		for (int i = 0; i < m_hand.size() - 7; i++)
		{
			// 33; 44; 55;
			// i,1; 2,3; 4,5; 6,7;
			if (m_hand[i].getNumber() == m_hand[i+1].getNumber() &&
				m_hand[i+2].getNumber() == m_hand[i+3].getNumber() &&
				m_hand[i+2].getNumber() == m_hand[i].getNumber() + 1 &&
				m_hand[i+4].getNumber() == m_hand[i+5].getNumber() &&
				m_hand[i+4].getNumber() == m_hand[i+2].getNumber() + 1 &&
				m_hand[i+6].getNumber() == m_hand[i+7].getNumber() &&
				m_hand[i+6].getNumber() == m_hand[i+4].getNumber() + 1)
			{
				selectCards(8, i);
				return m_hand;
			}
			else if (m_hand[i].getNumber() == m_hand[i+1].getNumber() &&
				m_hand[i+2].getNumber() == m_hand[i+3].getNumber() &&
				m_hand[i+2].getNumber() == m_hand[i].getNumber() + 1 &&
				m_hand[i+4].getNumber() == m_hand[i+5].getNumber() &&
				m_hand[i+4].getNumber() == m_hand[i+2].getNumber() + 1 &&
				m_hand[i+6].getNumber() == m_hand[i+7].getNumber() &&
				m_hand[i+6].getNumber() == King)
			{
				selectCards(8, i);
				return m_hand;
			}
		}
	}
	if (szHand >= 6)
	{
		for (int i = 0; i < m_hand.size() - 5; i++)
		{
			// 33; 44; 55;
			// i,1; 2,3; 4,5;
			if (m_hand[i].getNumber() == m_hand[i+1].getNumber() &&
				m_hand[i+2].getNumber() == m_hand[i+3].getNumber() &&
				m_hand[i+2].getNumber() == m_hand[i].getNumber() + 1 &&
				m_hand[i+4].getNumber() == m_hand[i+5].getNumber() &&
				m_hand[i+4].getNumber() == m_hand[i+2].getNumber() + 1)
			{
				selectCards(6, i);
				return m_hand;
			}
			else if (m_hand[i].getNumber() == m_hand[i+1].getNumber() &&
				m_hand[i+2].getNumber() == m_hand[i+3].getNumber() &&
				m_hand[i+2].getNumber() == m_hand[i].getNumber() + 1 &&
				m_hand[i+4].getNumber() == m_hand[i+5].getNumber() &&
				m_hand[i+4].getNumber() == King)
			{
				selectCards(6, i);
				return m_hand;
			}
		}
	}
	
	return m_hand;
}

// To bomb an ace you need a triple
std::vector<Card> GameMove::playBombAce()
{
	playTriple();
	if (!areCardsSelected())
		pass();

	return m_hand;
}

// To bomb a two you need a four kind or pair straight
std::vector<Card> GameMove::playBombTwo()
{
	HandEvaluator handEval;
	if (handEval.fourKindExists(m_hand))
		playFourKind();
	else if (handEval.pairStraightExists(m_hand))
		playPairStraight();

	if (!areCardsSelected())
		pass();

	return m_hand;
}

std::vector<Card> GameMove::selectMove(std::vector<Card> move)
{
	HandEvaluator handEval;
	for (int i = 0; i < move.size(); i++)
	{
		for (int j = 0; j < m_hand.size(); j++)
		{
			if (handEval.compareCards(move[i], m_hand[j]))
			{
				m_hand[j].setSelected(true);
			}
		}
	}
	return m_hand;
}

std::vector<Card> GameMove::playFirstTurn()
{
	HandEvaluator eval;
	auto possibleHands = eval.findPossibleHandsFirstTurn(m_hand);
	auto bestHand = possibleHands.back();
	
	switch (bestHand)
	{
		case HAND_TYPES::Single:
				return playSingle();
			break;
		case HAND_TYPES::Pair:
				return playPair();
			break;
		case HAND_TYPES::Triple:
				return playTriple();
			break;
		case HAND_TYPES::Straight:
				return playStraight();
			break;
		case HAND_TYPES::FourKind:
				return playFourKind();
			break;
		case HAND_TYPES::PairStraight:
				return playPairStraight();
			break;
		default:
			// Should never be here.. we will always at least play a single
			return playSingle();
			break;
	}
	
}

#pragma region TestingFunctions
std::vector<Card> GameMove::playSingleAt(int position)
{
	selectCards(1, position);
	return m_hand;
}

std::vector<Card> GameMove::playPairAt(int position)
{
	selectCards(2, position);
	return m_hand;
}

std::vector<Card> GameMove::playTripleAt(int position)
{
	selectCards(3, position);
	return m_hand;
}

std::vector<Card> GameMove::playFourKindAt(int position)
{
	selectCards(4, position);
	return m_hand;
}

std::vector<Card> GameMove::playStraightAt(int position, int size)
{
	selectCards(size, position);
	return m_hand;
}

std::vector<Card> GameMove::playPairStraightAt(int position, int size)
{
	selectCards(size, position);
	return m_hand;
}

void GameMove::deselectAllCards()
{
	deselectCards(m_hand.size(), 0);
}
#pragma endregion