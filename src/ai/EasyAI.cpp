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
// Filename:	EasyAI.cpp
// Description: An extention of Player to include AI decisions on how to play the game
//				as a Rule-based AI Agent
// Author:		Danny Sok
// Date Created: 17/04/2013
// Date Last Modified: 19/06/2013

#include "EasyAI.h"
#include "HandEvaluator.h"
#include "GameMove.h"

EasyAI::EasyAI()
{
}

// ----------------------------------------
// ~AI
// 
// Deletes the AI class (Deconstructor)
// Input:
//  none
// Output:
//  none
// Conditions:
//  none
EasyAI::~EasyAI()
{
}

void EasyAI::setup(string name, PLAYER_TYPES type, int pos)
{
	Human::setup(name, type, pos);
}

// ----------------------------------------
// Play
// 
// Determines the cards to play based on the cards last played
// Input:
//  lastPlayed is a vector that contains the cards that have been played last
// Output:
//  cardsToPlay, which contains the cards to play
// Conditions:
//  none

void EasyAI::threadedFunction()
{
	HandEvaluator eval;
	GameMove gm;

	if (isFinished())
	{
		stopThread();
	}
	
	while (isThreadRunning())
	{
		try
		{
			// Find out the hand type that was recently played
			auto ht = eval.findHandType(m_lastPlayed);

			// Easy type will be a hardcoded AI bot
			// For this, it will never pass unless it cannot play a card
			// If it has a card that is larger than one in the pile, then it will play it
			// 1. Find the card(s) that have been recently played on the pile
			// 2. Check to see if any card(s) the bot has is bigger than the previously played card(s)
			// 3. If it is then select this/these card(s) to play
			// if not, pass()
			if (m_cards.size() >= m_lastPlayed.size())
			{
				gm.init(m_cards);
				// Check to see if it has the lowest card
				//if (cards[0].getNumber() == Three && cards[0].getSuite() == Spade)
				if (m_firstTurn)
				{
					m_firstTurn = false;
					if (lock())
					{
						m_cards = gm.playSingle();
						unlock();
					}
					stop();
				}
				else
				{
					play(ht, true);
					stop();
				}
			}
			else
				stop(); // Have to pass since we can't beat it
		} 
		catch (exception e) 
		{
			stop();
			m_cards = gm.deselectCards(m_cards);
		}
	}
}

void EasyAI::play(HAND_TYPES ht, bool isLP)
{
	GameMove gm;
	gm.init(m_cards);
	if (lock())
	{
		// Find the lowest card that is higher than the played card
		if (ht == Single)
		{
			m_cards = gm.playSingle(m_lastPlayed);
		}
		
		// Find the lowest pair that is higher than the played pair
		else if(ht == Pair)
		{
			m_cards = gm.playPair(m_lastPlayed);
		}

		// Find the lowest triple that is higher than the played triple
		else if (ht == Triple)
		{
			m_cards = gm.playTriple(m_lastPlayed);
		}

		// Find the lowest four kind that is higher than the played four kind
		else if (ht == FourKind)
		{
			m_cards = gm.playFourKind(m_lastPlayed);
		}

		// Find the lowest straight that is higher than the played straight
		else if (ht == Straight)
		{
			m_cards = gm.playStraight(m_lastPlayed);
		}

		// Find the lowest pair straight that is higher than the played pair straight
		else if (ht == PairStraight)
		{
			m_cards = gm.playPairStraight(m_lastPlayed);
		}

		// With NoHand, the AI has free reign on choosing what type of plays to do e.g. if it has 345 it will play 345. If it just has 3, then play 3
		// for now just play the lowest card because I suck
		else if (ht == NoHand)
		{
			HAND_TYPES possibleHt = NoHand;
			HandEvaluator eval;
			if (m_cards.size() >= 4)
			{
				auto possibleHands = eval.findPossibleHands(m_cards);
				possibleHt = possibleHands.size() > 0 ? possibleHands[0] : NoHand;
			}

			if (possibleHt != NoHand)
				play(possibleHt);
			else
				m_cards = gm.playSingle();
			//cards[0].setSelected(true);
		}
		unlock();
	}
}

void EasyAI::play(HAND_TYPES ht)
{
	GameMove gm;
	gm.init(m_cards);
	if(lock())
	{
		// Find the lowest card that is higher than the played card
		if (ht == Single)
		{
			m_cards	= gm.playSingle();
		}
		
		// Find the lowest pair that is higher than the played pair
		else if(ht == Pair)
		{
			m_cards	= gm.playPair();
		}

		// Find the lowest triple that is higher than the played triple
		else if (ht == Triple)
		{
			m_cards	= gm.playTriple();
		}

		// Find the lowest four kind that is higher than the played four kind
		else if (ht == FourKind)
		{
			m_cards	= gm.playFourKind();
		}

		// Find the lowest straight that is higher than the played straight
		else if (ht == Straight)
		{
			m_cards	= gm.playStraight();
		}

		else if (ht == PairStraight)
		{
			m_cards	= gm.playPairStraight();
		}
		unlock();
	}
}