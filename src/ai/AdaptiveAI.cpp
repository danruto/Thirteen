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
// Filename:	AdaptiveAI.cpp
// Description: An extention of Player to include AI decisions on how to play the game
//				according to a Formula-based Strategy(FBS) mixture with E-Nash with
//				a chance to fallback to a defaulting strategy. It will incorporate
//				opponent modelling to adjust its selection and probabilitiy criterias
//				for this mixture
// Author:		Danny Sok
// Date Created: 25/06/2013
// Date Last Modified: 17/08/2013
#include "AdaptiveAI.h"
#include "..\util\Util.h"

AdaptiveAI::AdaptiveAI()
{
	m_gm.init(m_cards);
	m_godMode = false;
	m_useUct = true;
}

AdaptiveAI::~AdaptiveAI()
{
}

void AdaptiveAI::threadedFunction()
{
	Util::Log("AdaptiveAI::threadedFunction()");
	while (isThreadRunning())
	{
		m_gm.init(m_cards);
		if (isFinished())
		{
			stopThread();
		}
		else if (m_cards.size() >= m_lastPlayed.size())
		{
			// Check to see if it has the lowest card
			//if (cards[0].getNumber() == Three && cards[0].getSuite() == Spade)
			if (m_firstTurn)
			{
				m_firstTurn = false;

				if (lock())
				{
					//m_cards = m_gm.playSingle();
					m_cards = m_gm.playFirstTurn();
					unlock();
				}

				stop();
			}
			else
			{
				playMCTS();
				stop();
			}
		}
		else
			stop(); // PASS if we cannot beat the last card
	}
	//Util::Log("AdaptiveAI::threadedFunction() - crashed?");
}


void AdaptiveAI::playMCTS() 
{
	if (DEBUG_MODE)
		Util::Log("AdaptiveAI::playMCTS()");

	//GameTree gt;// = GameTree();
	

	//m_cards.clear();
	
		unique_ptr<GameTree> gt(new GameTree());
		gt->init(m_pos, m_useUct, m_godMode); // we do want the UCT style MCTS search
		auto c = gt->getUCTBestMove3();
	if (lock())
	{
		//m_cards = gt->getUCTBestMove3();
		
		//m_cards.clear();
		m_cards = c;
		
		/*GameTree gt;
		gt.init(m_pos, m_useUct, m_godMode);
		m_cards = gt.getUCTBestMove3();*/

		m_gm.init(m_cards); // Need to update the move for the new set of cards
		auto selectedCards = m_eval.getSelectedCards(m_cards);
		Util::Log("AdaptiveAI::playMCTS() - SelectedCards = " + handToString(selectedCards));

		unlock();
	}
	
	//m_cards = m_gm.selectMove(move);

	
	//gt->destroy();
	//gt.reset();	
}