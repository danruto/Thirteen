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
// Filename: ActionSelector.cpp
// Description: Selects the appropriate Action(Strategy) based on a MDP
// Author:		Danny Sok
// Date Created: 9/08/2013
// Date Last Modified: 27/09/2013

#include "ActionSelector.h"
#include "..\util\Util.h"

PLAYER_ACTIONS ActionSelector::getNextStrategy(std::vector<Card> hand, std::vector<Card> lastPlayed)
{
	//Util::Log("ActionSelector::getNextStrategy");
	m_hand = hand;
	unique_ptr<HandEvaluator> handEvaluator;
	auto possibleHands = handEvaluator->findPossibleHands(m_hand, lastPlayed);
	return chooseStrategy(possibleHands, lastPlayed);
}

PLAYER_ACTIONS ActionSelector::chooseStrategy(vector<HAND_TYPES> possibleHands, std::vector<Card> lastPlayed)
{
	//Util::Log("ActionSelector::chooseStrategy");
	std::vector<HAND_TYPES>::iterator it = possibleHands.begin();

	PLAYER_ACTIONS strat;
	std::vector<PLAYER_ACTIONS> strategies;

	// Search possibleHands and determine which strategies can be used
	for (int i = 0; i < possibleHands.size(); i++)
	//for (auto possibleHand:possibleHands) //C++11
	{
		switch (possibleHands[i])
		//switch (possibleHand)
		{
			case Single:
				if (lastPlayed.size() > 0)
					addToStrategies(strategies, PLAYER_ACTIONS::ACTION_A);
				addToStrategies(strategies, PLAYER_ACTIONS::ACTION_B);
				addToStrategies(strategies, PLAYER_ACTIONS::ACTION_C);
				break;
				//strategies.push_back(PLAYER_ACTIONS::ACTION_A);
				//strategies.push_back(PLAYER_ACTIONS::ACTION_B);
				//strategies.push_back(PLAYER_ACTIONS::ACTION_C);
			case Pair:
				if (lastPlayed.size() > 0)
					addToStrategies(strategies, PLAYER_ACTIONS::ACTION_A);
				addToStrategies(strategies, PLAYER_ACTIONS::ACTION_B);
				addToStrategies(strategies, PLAYER_ACTIONS::ACTION_C);
				addToStrategies(strategies, PLAYER_ACTIONS::ACTION_D);
				break;
				/*strategies.push_back(PLAYER_ACTIONS::ACTION_A);
				strategies.push_back(PLAYER_ACTIONS::ACTION_B);
				strategies.push_back(PLAYER_ACTIONS::ACTION_C);*/
			case Triple:
				if (lastPlayed.size() > 0)
					addToStrategies(strategies, PLAYER_ACTIONS::ACTION_A);
				addToStrategies(strategies, PLAYER_ACTIONS::ACTION_B);
				addToStrategies(strategies, PLAYER_ACTIONS::ACTION_C);
				addToStrategies(strategies, PLAYER_ACTIONS::ACTION_H);
				break;
				/*strategies.push_back(PLAYER_ACTIONS::ACTION_A);
				strategies.push_back(PLAYER_ACTIONS::ACTION_B);
				strategies.push_back(PLAYER_ACTIONS::ACTION_C);
				strategies.push_back(PLAYER_ACTIONS::ACTION_H);*/
			case Straight:
				if (lastPlayed.size() > 0)
					addToStrategies(strategies, PLAYER_ACTIONS::ACTION_A);
				addToStrategies(strategies, PLAYER_ACTIONS::ACTION_B);
				addToStrategies(strategies, PLAYER_ACTIONS::ACTION_C);
				addToStrategies(strategies, PLAYER_ACTIONS::ACTION_D);
				addToStrategies(strategies, PLAYER_ACTIONS::ACTION_I);
				break;
				/*strategies.push_back(PLAYER_ACTIONS::ACTION_A);
				strategies.push_back(PLAYER_ACTIONS::ACTION_B);
				strategies.push_back(PLAYER_ACTIONS::ACTION_C);
				strategies.push_back(PLAYER_ACTIONS::ACTION_I);*/
			case FourKind:
				if (lastPlayed.size() > 0)
					addToStrategies(strategies, PLAYER_ACTIONS::ACTION_A);
				addToStrategies(strategies, PLAYER_ACTIONS::ACTION_B);
				addToStrategies(strategies, PLAYER_ACTIONS::ACTION_C);
				break;
				/*strategies.push_back(PLAYER_ACTIONS::ACTION_A);
				strategies.push_back(PLAYER_ACTIONS::ACTION_B);
				strategies.push_back(PLAYER_ACTIONS::ACTION_C);*/
			case PairStraight:
				if (lastPlayed.size() > 0)
					addToStrategies(strategies, PLAYER_ACTIONS::ACTION_A);
				addToStrategies(strategies, PLAYER_ACTIONS::ACTION_B);
				addToStrategies(strategies, PLAYER_ACTIONS::ACTION_C);
				break;
				/*strategies.push_back(PLAYER_ACTIONS::ACTION_A);
				strategies.push_back(PLAYER_ACTIONS::ACTION_B);
				strategies.push_back(PLAYER_ACTIONS::ACTION_C);*/
			//case NoHand:
				//strategies.push_back(PLAYER_ACTIONS::ACTION_A);
			default:
				addToStrategies(strategies, PLAYER_ACTIONS::ACTION_A);
				//strategies.push_back(PLAYER_ACTIONS::ACTION_A);
				break;
		}
	}

	if (strategies.size() == 0)
		addToStrategies(strategies, PLAYER_ACTIONS::ACTION_A);

	// Remove the chance to pass if it is free reign!
	if (lastPlayed.size() == 0)
	{
		std::vector<PLAYER_ACTIONS>::iterator it;
		it = std::find(strategies.begin(), strategies.end(), ACTION_A);
		if (it != strategies.end())
			strategies.erase(it);
	}
	// calculate the payoffs for the strategies
	calculatePayoff(strategies, lastPlayed);
	
	// calculate the expected utility for the strategies
	calculateUtility(strategies);
	strat = selectStrat(strategies);
	// select a strategy from the probabilities calculated from previous step
	//strat = chooseStrategyUsingMatrix();

	// return the selected strategy
	return strat;
}

void ActionSelector::addToStrategies(std::vector<PLAYER_ACTIONS> & strat, PLAYER_ACTIONS a)
{
	if (std::find(strat.begin(), strat.end(), a) != strat.end())
	{
		//does contain a
	}
	else
	{
		//doesn't contain a
		strat.push_back(a);
	}
}

void ActionSelector::calculatePayoff(std::vector<PLAYER_ACTIONS> strategies, std::vector<Card> lastPlayed)
{
	//Util::Log("ActionSelector::calculatePayoff");
	// Fill Matrix using algorithm based on strategies and database (do a game tree search for the TURN ONLY)
	// Do backward induction
	 
	// Find the probability each outcome occurs in equilibrium
	// Do this buy multiplying probability of playing strats

	// For each outcome, multiply that probability by a particular player's payoff

	// Sum up all numbers

	//---- New idea --- //
	// use FBS adjusted by opponent modeller //
	// e.g. each strat has a default probability/payoff. That will get adjusted based on the opponent e.g if opponent plays singles alot then try and force a bomb //
	// Use Roulette Wheel Selection for final strat selection //
	
	//--
	// Adjust payoffs if there is info from database

	for (int i = 0; i < strategies.size(); i++)
	{
		switch (strategies[i])
		{
			case ACTION_B:
				if (lastPlayed.size() > 0)
					ACTION_B_PAYOFF = lastPlayed.back().getNumberInt() < 10 ? 2 : 1;
				else
					ACTION_B_PAYOFF = 2;
				break;
			case ACTION_C:
				if (lastPlayed.size() > 0)
					ACTION_C_PAYOFF = lastPlayed.back().getNumberInt() < 10 ? 1 : 2;
				else
					ACTION_C_PAYOFF = 2;
				break;
			case ACTION_D:
				if (lastPlayed.size() > 0)
					ACTION_D_PAYOFF = lastPlayed.back().getNumberInt() < 10 ? 1 : 4;
				else
					ACTION_D_PAYOFF = 4;
				break;
			case ACTION_E:
				ACTION_E_PAYOFF = m_hand.size() < 4 ? 2 : 1;
				break;
			case ACTION_F:
				ACTION_F_PAYOFF = m_hand.size() < 4 ? 4 : 2;
				break;
			case ACTION_G:
				ACTION_G_PAYOFF = m_hand.size() < 6 ? 1 : 3;
				break;
			case ACTION_H:
				if (lastPlayed.size() > 0)
					ACTION_H_PAYOFF = lastPlayed.back().getSuiteInt() > 1 ? 3 : 2;
				else
					ACTION_H_PAYOFF = 3;
				break;
			case ACTION_I:
				if (lastPlayed.size() > 0)
					ACTION_I_PAYOFF = lastPlayed.back().getSuiteInt() > 1 ? 6 : 4;
				else
					ACTION_I_PAYOFF = 6;
				break;
			default:
				break;
		}
	}
	

}

void ActionSelector::calculateUtility(std::vector<PLAYER_ACTIONS> strategies)
{
	// Fill up the vector. At position 0 should be for first strat, position 1 should be for second strat etc
	//m_expectedUtilities.push_back(1);

	//Expected Utility = prob(Payoff of Strat(first)) + (1 - prob(Payoff of strat(first)) * Payoff of Strat (second)

	
		// still need to do adjustment factors
	double defaultUtility = 1.0 / strategies.size();
	for (int i = 0; i < strategies.size(); i++)
	{
		if (OpponentModeller::getInstance().getUtility(strategies[i]) == 0)
		{
			OpponentModeller::getInstance().setUtility(strategies[i], defaultUtility);
		}
	}

	// Adjust probability based on opponent model
	// Clear out the utilities due to previous moves
	m_expectedUtilities.clear();
	for (int i = 0; i < strategies.size(); i++)
	{
		//double util = OpponentModeller::getInstance().getUtility(strategies[i]) == 0 ? defaultUtility : OpponentModeller::getInstance().getUtility(strategies[i]);
		// Get current utility, then have a function calculateNewUtility(); which will scale the utilty based on how many wins it has and update the utility
		double util = calculateNewUtility(strategies, i);
		m_expectedUtilities.push_back( util );
	}

}

double ActionSelector::calculateNewUtility(std::vector<PLAYER_ACTIONS> strategies, int position)
{
	Util::Log("ActionSelector::calculateNewUtility");
	double winRate = OpponentModeller::getInstance().getWinRateOf(strategies[position]);
	double util = OpponentModeller::getInstance().getUtility(strategies[position]);
	double newUtil = 0;
	int numStrats = strategies.size();

	// If WinRate is in range of 0-49 leave as is
	if (winRate < 50)
	{
		return util;
	}
	
	// If WinRate is in range of 50-69, increase chances by 0.2 while decreasing the others by 0.2/(numStrats)
	else if (winRate >= 50 && winRate < 70)
	{
		newUtil = getUtilityUsingScalar(0.2, util, strategies, position);
	}

	// If WinRate is in range of 70-79, increase chances by 0.4 while decreasing the others by 0.4/(numStrats)
	else if (winRate >= 70 && winRate < 80)
	{
		newUtil = getUtilityUsingScalar(0.4, util, strategies, position);
	}

	// If WinRate is in range of 80-89, increase chances by 0.6 while decreasing the others by 0.6/(numStrats)
	else if (winRate >= 80 && winRate < 90)
	{
		newUtil = getUtilityUsingScalar(0.6, util, strategies, position);
	}

	// If WinRate is in range of 90-100 increase chances by 0.8 while decreasing the others by 0.8/(numStrats)
	else if (winRate >= 90 && winRate <= 100)
	{
		newUtil = getUtilityUsingScalar(0.8, util, strategies, position);
	}

	return newUtil;

}

double ActionSelector::getUtilityUsingScalar(double scalar, double util, std::vector<PLAYER_ACTIONS> strategies, int position)
{
	Util::Log("ActionSelector::getUtilityUsingScalar()");
	int numStrats = strategies.size();
	double newUtil = ((util * scalar) / numStrats) + util;
	double value = util - ((util * scalar) / numStrats);

	for (int i = 0; i < numStrats; i++)
	{
		if (i != position)
		{
			OpponentModeller::getInstance().setUtility(strategies[i], value);
		}
	}
	Util::Log("ActionSelector::getUtilityUsingScalar() fin");
	return newUtil;
}

PLAYER_ACTIONS ActionSelector::chooseStrategyUsingMatrix()
{
	// using m_expectedUtilities we will choose a strategy at random with the probabilties contained in the vector

	// return the randomly selected strategy
	return ACTION_A;
}


PLAYER_ACTIONS ActionSelector::selectStrat(std::vector<PLAYER_ACTIONS> strategies)
{
	Util::Log("ActionSelector::selectStrat");
	std::vector<double> probabilties;
	probabilties.push_back(m_expectedUtilities[0]);

	for (int i = 1; i < m_expectedUtilities.size(); ++i)
	{
		probabilties.push_back( probabilties[i-1] + m_expectedUtilities[i] );
	}

	int randRange = rand() % 100;

	int index = binarySearch(probabilties, randRange);

	Util::Log("Selected Strat Index: " + ofToString(index) + " Strat: " + ofToString(strategies[index]));
	//Util::Log("RandRange: " + ofToString(randRange));
	//Util::Log("Probabilities.size(): " + ofToString(probabilties.size()));

	return strategies[index];
}

int ActionSelector::binarySearch (std::vector<double> probabilities, int key)
{
	//Util::Log("ActionSelector::binarySearch");
	int low = 0;
	int high = probabilities.size() - 1;
	int mid = 0;
	double newk = key/100.0;
    while(low <= high)
	{
        mid  = (low + high)/2;
		// Break the loop if it gets stuck
		if (low == 0 && high == 0)
		{
			break;
		}
		// Greater than Previous, Less than self
		if (mid == 0)
		{
			if (newk >= 0 && newk <= probabilities[mid])
			{
				return mid;
				break;
			}
			//HACK
			if (high == 1)
				break;
		}
		else if (newk >= probabilities[mid-1] && newk <= probabilities[mid])
		{
			return mid;
			break;
		}
		else if(newk < probabilities[mid])
		{
            high = mid - 1;
        }
		
		else
		{
            low = mid + 1;
        }
    }

    return 0;
}
