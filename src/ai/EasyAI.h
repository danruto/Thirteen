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
// Filename:	EasyAI.h
// Description: An extention of Player to include AI decisions on how to play the game
//				as a Rule-based AI Agent
// Author:		Danny Sok
// Date Created: 17/04/2013
// Date Last Modified: 18/04/2013

#pragma once

#ifndef EASYAI_H
#define EASYAI_H

#include "AI.h"

class EasyAI : public AI {
public:
	EasyAI();
	EasyAI(string name, PLAYER_TYPES type);
	~EasyAI();
	
	// Function Overrides
	void setup(string name, PLAYER_TYPES type, int pos);
	//std::vector<Card> play(std::vector<Card> lastPlayed);
	void play(HAND_TYPES ht, bool isLastPlayed);
	void play(HAND_TYPES ht);
	//void play(HAND_TYPES ht);

	void threadedFunction();
};

#endif EASYAI_H