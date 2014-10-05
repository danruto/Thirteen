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
// Filename: Deck.h
// Description: Represents a deck of cards and the modification of the deck
// Author: Danny Sok
// Date Created: 10/04/2013
// Date Last Modified: 12/04/2013

#pragma once

#ifndef DECK_H
#define DECK_H

#define NUM_CARDS  52
#define NUM_SUITES 4
#define SIZE_HAND  13

#include "Card.h"
#include "PlayerFactory.h"
#include "..\ai\AI.h"


class Deck {
public:
	Deck();
	~Deck();
	void setup();
	void setup(bool b);
	void setup(int Card_W, int Card_H);
	void createCards();
	void createCards(bool neededByAI);
	void createCards(int Card_W, int Card_H);
	void shuffle();
	std::vector<Card> getDeck();
	int getSize();
	void deal(shared_ptr<IPlayer> p);
	void deal(AI *p);
	void clear();
private:
	std::vector<Card> m_cards;
	
};


#endif /* DECK_H */

