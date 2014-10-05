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
// Filename:	OpponentModeller.h
// Description: A database interface to model the opponent
// Author:		Danny Sok
// Date Created: 26/06/2013
// Date Last Modified: 26/06/2013

#pragma once

#ifndef OPPONENTMODELLER_H
#define OPPONENTMODELLER_H

#include "ofMain.h"
#include "..\lib\Database.h"
#include "..\game\Card.h"
#include "..\game\Globals.h"

class OpponentModeller
{
public:
	static OpponentModeller& getInstance()
        {
            static OpponentModeller    instance; // Guaranteed to be destroyed.
                                  // Instantiated on first use.
            return instance;
        }

	void create(string filename);
	void connect(string filename);
	bool open(string filename);
	vector< vector<string> > getResult();
	vector< vector<string> > getResult(string query);
	void close();

	// New test functions
	void insertSession(int playerCount);
	void updateSession(int winner);
	void insertPlayer(int playerId, int playerType, int turns, int playCount);
	void updatePlayer(int playerId, int turns, int playCount);
	void insertHand(vector<Card> played, HAND_TYPES ht, int playerId, bool isWinningHand);

	string getCurrentSession();
	string getPlayerId(int pId);

	double getUtility(PLAYER_ACTIONS strat);
	void setUtility(PLAYER_ACTIONS strat, double value);
	double getWinRateOf(PLAYER_ACTIONS strat);

	std::vector<Card> getCardsPlayedInLatestSession();

private:
	Database *_db;

	OpponentModeller() {};
	OpponentModeller(OpponentModeller const&);
	void operator=(OpponentModeller const&);

	void createUtilities();
	bool utilitiesExist();
	void updateHand();
	int getTotalWinsFor(PLAYER_ACTIONS strat);
	int getTotalHands();
	bool isWinningHand(PLAYER_ACTIONS strat);
	PLAYER_ACTIONS handTypeToAction(HAND_TYPES ht);
	void debug();
	
};

#endif 