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
// Filename: AIGame.h
// Description: Represents the state of the game from the AI perspective
// Author:		Danny Sok
// Date Created: 9/08/2013
// Date Last Modified: 27/09/2013
#ifndef AI_GAME_H
#define AI_GAME_H

#include "..\game\Card.h"

class AIGame
{
public:
	~AIGame();
	void init();
	void init(std::vector<Card> deck, std::vector<Card> playerHand, int playerPos, int passCount, bool isFreeReign);
	void init(std::vector< std::vector<Card> > hands, int playerPos, int passCount, bool isFreeReign = false);
	void destroy();

	void deal();
	void setDeck(std::vector<Card> deck);
	void dealPlayer(int pos);
	void addHand(std::vector<Card> hand);
	void initAddedHands(int playerPos, int passCount, int originalPlayerPos, bool guessMode = true, bool isFreeReign = false, int depth = 0);
	void setGuessMode(bool b);

	int getPlayerCount();
	int getPassCount();
	int getOriginalPlayerPos();
	std::vector<Card> getHand(int pos);
	bool isOver();
	bool isWinner(int playerNum);
	bool isWinner();
	bool isFreeReign(int playerNum);
	bool isFreeReign();
	int getDepth();

private:
	std::vector< std::vector<Card> > m_hands;
	std::vector<Card> m_deck, m_playerHand;
	int m_playerPos;
	int m_passCount;
	int m_playerCount;
	int m_originalPlayerPos;
	bool m_isGuessMode;
	bool m_isFreeReign;
	int m_depthCount;
	int m_turnCount;

	std::vector<Card> sortCards(std::vector<Card> pHand);
};

#endif // AI_GAME_H