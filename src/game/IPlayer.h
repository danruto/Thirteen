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
// Filename:	IPlayer.h
// Description: An interface to cover players for the game
// Author:		Danny Sok
// Date Created: 25/06/2013
// Date Last Modified: 25/06/2013

#pragma once

#ifndef IPLAYER_H
#define IPLAYER_H

#include "Card.h"
#include "Globals.h"

class IPlayer : public ofThread
{
public:
	virtual ~IPlayer()  {}
	virtual void setup(string name, PLAYER_TYPES type, int pos) = 0;
	virtual std::vector<Card> play(vector<Card> lastPlayed) = 0;
	virtual void pass() = 0;
	virtual void quit() = 0;
	virtual void setCard(int pos, Card c) = 0;
	virtual Card * getCardAt(int pos) = 0;
	virtual vector<Card> & getCardsPtr() = 0;
	virtual vector<Card> getCards() = 0;
	virtual void setCards(vector<Card> hand) = 0;
	virtual void sortCards() = 0;
	virtual void setupCardImg() = 0;
	
	virtual int getTurn() = 0;
	virtual void incTurn() = 0;
	virtual bool isTurn() = 0;
	virtual void setTurn(bool turn) = 0;
	virtual std::string getName() = 0;
	virtual void drawName(int x, int y, ofTrueTypeFont * vagRounded) = 0;

	virtual bool isFinished() = 0;
	virtual int getCardPos(string name) = 0;

	virtual void setFirst(bool b) = 0;
	virtual PLAYER_TYPES getType() = 0;
	virtual int getPlayCount() = 0;

	virtual void incWinCount() = 0;
	virtual void incLossCount() = 0;
	virtual int getWinCount() = 0;
	virtual int getLossCount() = 0;

	virtual bool isMakingMove() = 0;
	virtual bool isTurnToMakeMove() = 0;
	virtual bool hasMadeMove() = 0;
	virtual void start() = 0;
	virtual void stop() = 0;
	virtual void setLastPlayed(std::vector<Card> lastPlayed) = 0;

	virtual std::string toString() = 0;
};

#endif IPLAYER_H