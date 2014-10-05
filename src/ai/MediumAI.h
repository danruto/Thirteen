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
// Filename:	MediumAI.h
// Description: An extention of Player to include AI decisions on how to play the game
//				according to a Formula-based Strategy(FBS) mixture with E-Nash with
//				a chance to fallback to a defaulting strategy. It will incorporate
//				opponent modelling to adjust its selection and probabilitiy criterias
//				for this mixture
// Author:		Danny Sok
// Date Created: 22/09/2013
// Date Last Modified: 22/09/2013

#pragma once

#ifndef MEDIUMAI_H
#define MEDIUMAI_H

#include "AI.h"
#include "..\ai\ActionSelector.h"
#include "..\game\Game.h"
#include "GameMove.h"


class MediumAI : public AI
{
public:
	MediumAI();
	MediumAI(Game * game);
	~MediumAI();

	// Function overrides
	//std::vector<Card> play(std::vector<Card> lastPlayed);
	void play(PLAYER_ACTIONS strat);
	void threadedFunction();

	
private:
	ActionSelector m_actionSelector;
	//HandEvaluator m_eval;
	GameMove m_gm;

	void calculateNextStrategy();
	void playNextStrat();
};

#endif MEDIUMAI_H