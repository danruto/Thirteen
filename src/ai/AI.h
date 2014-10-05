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
// Filename:	AI.h
// Description: An extention of Player to include AI decisions on how to play the game
//				based on the cards that have been played
// Author:		Danny Sok
// Date Created: 25/06/2013
// Date Last Modified: 25/06/2013

#pragma once

#ifndef AI_H
#define AI_H

#include "..\game\Human.h"

class AI : public Human

{
public:
	AI();
	~AI();
	//void setup(string name, PLAYER_TYPES type);
	
	//void setFirst(bool b);
	
protected:
	PLAYER_TYPES m_type;
	// _firstTurn;
};

#endif AI_H