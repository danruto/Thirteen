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
// Filename: GameMove.h
// Description: Represents the possible moves that can be played.
// Author:		Danny Sok
// Date Created: 9/08/2013
// Date Last Modified: 27/09/2013
#ifndef GAME_MOVE_H
#define GAME_MOVE_H

#include "..\game\Globals.h"
#include "..\game\Card.h"

class GameMove
{
public:
	GameMove() {}
	~GameMove() {}
	void init(std::vector<Card> hand);

	// Free Regin
	std::vector<Card> playSingle();
	std::vector<Card> playPair();
	std::vector<Card> playTriple();
	std::vector<Card> playFourKind();
	std::vector<Card> playStraight();
	std::vector<Card> playPairStraight();
	std::vector<Card> playBombAce();
	std::vector<Card> playBombTwo();

	std::vector<Card> playHighSingle();
	std::vector<Card> playHighPair();
	std::vector<Card> playHighTriple();
	std::vector<Card> playHighStraight();
	std::vector<Card> playHighPairStraight();
	std::vector<Card> playHighFourKind();

	// Against Last Played
	std::vector<Card> playSingle(std::vector<Card> lastPlayed);
	std::vector<Card> playPair(std::vector<Card> lastPlayed);
	std::vector<Card> playTriple(std::vector<Card> lastPlayed);
	std::vector<Card> playFourKind(std::vector<Card> lastPlayed);
	std::vector<Card> playStraight(std::vector<Card> lastPlayed);
	std::vector<Card> playPairStraight(std::vector<Card> lastPlayed);
	std::vector<Card> playBombAce(std::vector<Card> lastPlayed);
	std::vector<Card> playBombTwo(std::vector<Card> lastPlayed);

	std::vector<Card> playLowestPossibleHand(HAND_TYPES hand, std::vector<Card> lastPlayed);
	std::vector<Card> playHighestPossibleHand(HAND_TYPES hand, std::vector<Card> lastPlayed);
	
	std::vector<Card> playActionD(std::vector<Card> lastPlayed);
	std::vector<Card> playActionF(bool opponentHasOneCardLeft);
	std::vector<Card> playActionG();
	std::vector<Card> playHighSingle(std::vector<Card> lastPlayed);
	std::vector<Card> playHighPair(std::vector<Card> lastPlayed);
	std::vector<Card> playHighTriple(std::vector<Card> lastPlayed);
	std::vector<Card> playHighStraight(std::vector<Card> lastPlayed);
	std::vector<Card> playHighPairStraight(std::vector<Card> lastPlayed);
	std::vector<Card> playHighFourKind(std::vector<Card> lastPlayed);

	std::vector<Card> playFirstTurn();

	bool hasTwoInHand();
	/*std::vector<Card> playSingle(std::vector<Card> hand, std::vector<Card> lastPlayed);
	std::vector<Card> playPair(std::vector<Card> hand, std::vector<Card> lastPlayed);
	std::vector<Card> playTriple(std::vector<Card> hand, std::vector<Card> lastPlayed);
	std::vector<Card> playFourKind(std::vector<Card> hand, std::vector<Card> lastPlayed);
	std::vector<Card> playStraight(std::vector<Card> hand, std::vector<Card> lastPlayed);
	std::vector<Card> playPairStraight(std::vector<Card> hand, std::vector<Card> lastPlayed);*/

	std::vector<Card> selectMove(std::vector<Card> move);
	bool GameMove::areCardsSelected(std::vector<Card> hand);
	std::vector<Card> deselectCards(std::vector<Card> phand);

	// --------- Testing ---------- //
	std::vector<Card> playSingleAt(int position);
	std::vector<Card> playPairAt(int position);
	std::vector<Card> playTripleAt(int position);
	std::vector<Card> playFourKindAt(int position);
	std::vector<Card> playStraightAt(int position, int size);
	std::vector<Card> playPairStraightAt(int position, int size);
	void deselectAllCards();
	// --------- Testing ---------- //

private:
	void selectCards(int size, int i);
	void deselectCards(int size, int i);
	bool areCardsSelected();
	std::vector<Card> pass();

	std::vector<Card> m_hand;

};

#endif