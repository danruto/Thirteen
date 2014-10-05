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
// Filename: ActionSelector.h
// Description: Selects the appropriate Action(Strategy) based on a MDP
// Author:		Danny Sok
// Date Created: 9/08/2013
// Date Last Modified: 4/09/2013

#pragma once

#ifndef ACTION_SELECTOR_H
#define ACTION_SELECTOR_H

#include "..\game\Card.h"
#include "..\game\Globals.h"
#include "..\lib\Matrix.h"
#include "..\lib\Database.h"
#include "HandEvaluator.h"
#include "OpponentModeller.h"

class ActionSelector
{
public:
	PLAYER_ACTIONS getNextStrategy(std::vector<Card> hand, std::vector<Card> lastPlayed);
private:
	std::vector<HAND_TYPES> findPossibleHands(std::vector<Card> hand);
	PLAYER_ACTIONS chooseStrategy(vector<HAND_TYPES> possibleHands, std::vector<Card> lastPlayed);
	void calculatePayoff(std::vector<PLAYER_ACTIONS> strategies, std::vector<Card> lastPlayed);
	void calculateUtility(std::vector<PLAYER_ACTIONS> strategies);
	PLAYER_ACTIONS chooseStrategyUsingMatrix();
	PLAYER_ACTIONS selectStrat(std::vector<PLAYER_ACTIONS> strategies);
	double calculateNewUtility(std::vector<PLAYER_ACTIONS> strategies, int position);
	double getUtilityUsingScalar(double scalar, double util, std::vector<PLAYER_ACTIONS> strategies, int position);
	int binarySearch (std::vector<double> probabilities, int key);
	void addToStrategies(std::vector<PLAYER_ACTIONS> & strat, PLAYER_ACTIONS a);
	
	std::vector<Card> m_hand;
	Matrix<int> m_AgentMatrix, m_SystemMatrix;
	vector<double> m_expectedUtilities;
	//HandEvaluator handEvaluator;

	int ACTION_A_PAYOFF;
	int ACTION_B_PAYOFF;
	int ACTION_C_PAYOFF;
	int ACTION_D_PAYOFF;
	int ACTION_E_PAYOFF;
	int ACTION_F_PAYOFF;
	int ACTION_G_PAYOFF;
	int ACTION_H_PAYOFF;
	int ACTION_I_PAYOFF;
};

#endif