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
// Filename: Human.h
// Description: Representation of a player for the game
// Author: Danny Sok
// Date Created: 10/04/2013
// Date Last Modified: 9/05/2013

#pragma once

#ifndef HUMAN_H
#define HUMAN_H

#define SIZE_HAND  13

#include "ofMain.h"
#include "IPlayer.h"
#include "Card.h"

/*struct CardComparer {
	const vector<Card>& c;
	CardComparer(const vector<Card>& card) : c(card) {}
	bool operator() (const Card& l, const Card& r) const {
		// Want to sort asc. Smallest on the left
		if (left.Number > right.Number)
		{
			if (right.Number == Ace || right.Number == Two)
				return false;
			else
				return true;
		}
		else if (left.Number < right.Number)
		{
			if (left.Number == Ace || left.Number == Two)
				return true;
			else 
				return false;
		}
		else //if left.Number == right.Number
		{
			if (left.Suite > right.Suite)
				return true;
			else
				return false;
		}
	}
};*/

class Human :  public IPlayer {
public:
	Human();
	~Human();
	Human(string name);

	//Function Overrides
	virtual void setup(string name, PLAYER_TYPES type, int pos);

	virtual std::vector<Card> play(vector<Card> lastPlayed);
	virtual void pass();
	virtual void quit();

	virtual void setCard(int pos, Card c);
	virtual Card * getCardAt(int pos);
	virtual vector<Card> & getCardsPtr();
	virtual vector<Card> getCards();
	virtual void setCards(vector<Card> cards);
	virtual void sortCards();
	virtual void setupCardImg();
	
	virtual int getTurn();
	virtual void incTurn();
	virtual bool isTurn();
	virtual void setTurn(bool turn);
	virtual void setFirst(bool b);
	virtual PLAYER_TYPES getType();
	virtual int getPlayCount();
	virtual void incWinCount();
	virtual void incLossCount();
	virtual int getWinCount();
	virtual int getLossCount();

	virtual std::string getName();
	virtual void drawName(int x, int y, ofTrueTypeFont * vagRounded);
	
	virtual int getCardPos(string name);

	virtual bool isFinished();

	virtual bool isMakingMove();
	virtual bool isTurnToMakeMove();
	virtual bool hasMadeMove();
	virtual void start();
	virtual void stop();
	virtual void setLastPlayed(std::vector<Card> lastPlayed);

	virtual std::string toString();

protected:
	std::string m_name;
	std::vector<Card> m_cards;
	std::vector<Card> m_lastPlayed;
	int m_turnCount, m_playCount, m_winCount, m_lossCount;
	bool m_bTurn;
	PLAYER_TYPES m_type;
	bool m_firstTurn;
	int m_pos;
	bool m_madeMove;
	int m_startThreadTime;

	std::string handToString();
	std::string handToString(std::vector<Card> h);
	
	std::string playerTypeStr();
};


#endif /* HUMAN_H */

