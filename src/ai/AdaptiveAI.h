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
// Filename:	AdaptiveAI.h
// Description: An extention of Player to include AI decisions on how to play the game
//				according to a Formula-based Strategy(FBS) mixture with E-Nash with
//				a chance to fallback to a defaulting strategy. It will incorporate
//				opponent modelling to adjust its selection and probabilitiy criterias
//				for this mixture
// Author:		Danny Sok
// Date Created: 25/06/2013
// Date Last Modified: 17/08/2013

#pragma once

#ifndef ADAPTIVEAI_H
#define ADAPTIVEAI_H

#include "AI.h"
#include "..\lib\Matrix.h"
#include "ActionSelector.h"
#include "GameTree.h"

typedef struct 
{
	vector< vector<int> > v;
} payoff_matrix_t;



class AdaptiveAI : public AI
{
public:
	AdaptiveAI();
	~AdaptiveAI();

	// Function overrides
	//std::vector<Card> play(std::vector<Card> lastPlayed);
	void playMCTS();
	void play(HAND_TYPES ht, std::vector<Card> lastPlayed);
	void threadedFunction();

	
private:
	ActionSelector m_actionSelector;
	HandEvaluator m_eval;
	Matrix<int> PlayerPayoffMatrix;
	Matrix<int> SystemPayoffMatrix;
	GameMove m_gm;
	bool m_godMode;
	bool m_useUct;
	

	void play(PLAYER_ACTIONS strat, std::vector<Card> lastPlayed);
	
};

#endif ADAPTIVEAI_H