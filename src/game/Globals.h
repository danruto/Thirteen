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
// Filename: Globals.h
// Description: Contains the global variables for the application
// Author: Danny Sok
// Date Created: 10/04/2013
// Date Last Modified: 12/04/2013

#pragma once

#ifndef GLOBALS_H
#define GLOBALS_H

#include "ofMain.h"

/*
namespace HAND_TYPES
{ 
	enum {
		Single,
		Pair,
		Triple,
		FourKind,
		Straight,
		PairStraight,
		NoHand
	};
}

namespace PLAYER_TYPES
{
	enum {
		HUMAN,
		AI_EASY,
		AI_HARD,
		NONE,
		AI_MEDIUM
	};
}

namespace PLAYER_ACTIONS
{
	enum {
		ACTION_A,
		ACTION_B,
		ACTION_C,
		ACTION_D,
		ACTION_E,
		ACTION_F
	};
}*/

enum HAND_TYPES {
	Single,
	Pair,
	Triple,
	FourKind,
	Straight,
	PairStraight,
	NoHand
};

enum PLAYER_TYPES {
	HUMAN,
	AI_EASY,
	AI_HARD,
	NONE,
	AI_MEDIUM
};

enum GAME_HAND {
	MyHand,
	LastPlayedHand
};

enum PLAYER_ACTIONS {
	// Pass
	ACTION_A = 0,

	// Play LOWEST hand GREATER than last played
	ACTION_B = 1,

	// Play HIGHEST hand GREATER than last played
	ACTION_C = 2,

	// Break the Pair Straights to beat last hand if GREATER than last played AND no 2's are in hand
	ACTION_D = 3,

	// Play lowest single in hand
	ACTION_E = 4,

	// Play a Pair or higher if opponent(s) have only one card in hand
	ACTION_F = 5,

	// Break a Hand IF you cannot get a free reign turn
	ACTION_G = 6,

	// Bomb a Ace with a Triple
	ACTION_H = 7,

	// Bomb a Two with a Pair Straight
	ACTION_I = 8
};


struct PLAYER_ACTIONS_T {
	string name;
	int prob;
};

extern int ACTION_A_PAYOFF;
extern int ACTION_B_PAYOFF;
extern int ACTION_C_PAYOFF;
extern int ACTION_D_PAYOFF;
extern int ACTION_E_PAYOFF;
extern int ACTION_F_PAYOFF;
extern int ACTION_G_PAYOFF;
extern int ACTION_H_PAYOFF;
extern int ACTION_I_PAYOFF;


#endif