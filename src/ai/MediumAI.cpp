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
// Filename:	MediumAI.cpp
// Description: An extention of Player to include AI decisions on how to play the game
//				according to a Formula-based Strategy(FBS) mixture with E-Nash with
//				a chance to fallback to a defaulting strategy. It will incorporate
//				opponent modelling to adjust its selection and probabilitiy criterias
//				for this mixture
// Author:		Danny Sok
// Date Created: 22/09/2013
// Date Last Modified: 27/09/2013
#include "MediumAI.h"
#include "..\util\Util.h"

MediumAI::MediumAI()
{
}

MediumAI::~MediumAI()
{
}


void MediumAI::playNextStrat() 
{
	calculateNextStrategy();
}

void MediumAI::threadedFunction()
{
	while (isThreadRunning())
	{
		try
		{
			m_gm.init(m_cards);

			if (isFinished())
			{
				stopThread();
			}

			// Find out the hand type that was recently played
			//auto ht = m_eval.findHandType(m_lastPlayed);

			if (m_cards.size() >= m_lastPlayed.size())
			{
				// Check to see if it has the lowest card
				//if (cards[0].getNumber() == Three && cards[0].getSuite() == Spade)
				if (m_firstTurn)
				{
					m_firstTurn = false;
					if (lock())
					{
						m_cards = m_gm.playSingle();
						unlock();
					}
					stop();
				}
				else
				{
					if (lock())
					{
						playNextStrat();
						unlock();
					}
					stop();
				}
			}
			else
				stop(); // Have to pass since we can't beat it
		} 
		catch (exception e) 
		{ 
			stop (); 
			m_cards = m_gm.deselectCards(m_cards);
		}
	}
}

/*
> Randomly choose a strategy from the probability calculated from the utility
> Search to see if the strat can be palyed
> If not, check if strat A can be played
> If not, play strat F
*/
void MediumAI::calculateNextStrategy()
{
	//Util::Log("MediumAI::calculateNextStrategy");
	ActionSelector aS;
	auto strat = aS.getNextStrategy(m_cards, m_lastPlayed);
	//unique_ptr<ActionSelector> aS;
	//auto strat = aS->getNextStrategy(m_cards, m_lastPlayed);
	//PLAYER_ACTIONS strat = m_actionSelector.getNextStrategy(m_cards, m_lastPlayed);
	//HandType lastPlayedHandType = findHandType(lastPlayed);
	//PLAYER_ACTIONS strat = actionSelector.getNextStrategy(cards, lastPlayedHandType);
	play(strat);
}

void MediumAI::play(PLAYER_ACTIONS strat) 
{
	//HandEvaluator eval;
	//auto hand = eval.findHandType(m_lastPlayed);

	//cout << "MediumAI::play - " << m_name << " - " << strat << endl;
	if(lock())
	{
		Util::Log("MediumAI::play()");
		// Test because it keeps crashing 'vector iterator not incrementable'. From what I can see, seems to be a pass by value error? But I put HandEvaluator to be created here so nfi
		unique_ptr<HandEvaluator> evalPtr;
		auto hand = evalPtr->findHandType(m_lastPlayed);
		switch(strat)
		{
			case ACTION_B:
				m_cards	= m_gm.playLowestPossibleHand(hand, m_lastPlayed);
				break;
			case ACTION_C:
				m_cards	= m_gm.playHighestPossibleHand(hand, m_lastPlayed);
				break;
			case ACTION_D:
				if (m_lastPlayed.size() > 0)
					m_cards	= m_gm.playActionD(m_lastPlayed);
				else
					m_cards	= m_gm.playSingle();
				break;
			case ACTION_E:
				if (m_lastPlayed.size() > 0)
				{
					m_cards	= m_gm.playSingle(m_lastPlayed);
				}
				else
				{
					m_cards	= m_gm.playSingle();
				}
				break;
			case ACTION_F:
				if (m_lastPlayed.size() > 0)
					m_cards	= m_gm.playActionF(false);
				else
					m_cards	= m_gm.playSingle();
				break;
			case ACTION_G:
				if (m_lastPlayed.size() > 0)
					m_cards	= m_gm.playActionG();
				else
					m_cards	= m_gm.playSingle();
				break;
			case ACTION_H:
				m_cards	= m_gm.playBombAce();
				break;
			case ACTION_I:
				m_cards	= m_gm.playBombTwo();
				break;
			default:
				break;
		}

		auto selectedCards = evalPtr->getSelectedCards(m_cards);

		// Bug fix for when a hand cannot be played during its free reign turn.
		if (strat != PLAYER_ACTIONS::ACTION_A)
		{
			if (evalPtr->getSelectedCards(m_cards).size() == 0)
				m_cards = m_gm.playSingle();
		}

		// Bug fix check - oddly a lower single gets chosen than last played. No idea how, so need this to find out when and fix it.
		// This could possibly be due to the thing above
		if (selectedCards.size() > 0)
		{
			if (!evalPtr->isValidPlay(selectedCards, m_lastPlayed))
				m_cards = evalPtr->deselectAllCards(m_cards);
		}
		unlock();
	}
}
