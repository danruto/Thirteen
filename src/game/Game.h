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
// Filename: Game.h
// Description: The main game instance. It will deal with setting up the cards and players
// Author: Danny Sok
// Date Created: 10/04/2013
// Date Last Modified: 17/04/2013

#pragma once

#ifndef GAME_H
#define GAME_H

#include "PlayerFactory.h"
#include "Deck.h"
#include "Globals.h"

typedef std::shared_ptr<IPlayer> IPlayerPtr;

class Game {
public:
	//Game();
	static Game& getInstance()
        {
            static Game    instance; // Guaranteed to be destroyed.
                                  // Instantiated on first use.
            return instance;
        }
	~Game();
	void setup();
	void setup(int Card_W, int Card_H);
	void start(string name);
	void start(std::string name, int numPlayers);
	void start(std::string name, int numPlayers, PLAYER_TYPES ai);
	void start(string name, std::vector<int> aiAmount);
	void createPlayers(string name, PLAYER_TYPES type, int pos);
	void finish();
	void shuffle();
	void deal();
	void addAIPlayers(int number);
	
	IPlayerPtr getPlayer(int number);
	std::vector< IPlayerPtr > getPlayers();
	int getPlayerCount();
	int getAICount();
	int getPassCount();
	void setPassCount(int c);
	
	void setLastPlayed(std::vector<Card> c);
	std::vector<Card> getLastPlayed();


	void updateWinner(int pNum);

	void setFieldState(HAND_TYPES ht) { m_fieldState = ht; }
	HAND_TYPES getFieldState();

	void clear();

private:
	//http://ootips.org/yonat/4dev/smart-pointers.html
	Game() {};
	Game(Game const&);
	void operator=(Game const&);

	std::vector< IPlayerPtr > m_players;
	Deck m_deck;
	HAND_TYPES m_fieldState;
	std::vector<Card> m_lastPlayed;
	int m_passCount;
	
	std::string makeAIName();
};


#endif /* GAME_H */