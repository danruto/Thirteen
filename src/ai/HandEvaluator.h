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
// Filename: HandEvaluator.h
// Description: This will determine what kind of playable hands exist in the Player's Hand
// Author: Danny Sok
// Date Created: 12/08/2013
// Date Last Modified: 12/08/2013

#pragma once

#ifndef HAND_EVALUATOR_H
#define HAND_EVALUATOR_H

#include "..\game\Globals.h"
#include "..\game\Card.h"

class HandEvaluator
{
public:
	std::vector<HAND_TYPES> findPossibleHands(std::vector<Card> hand, std::vector<Card> lastPlayed);
	std::vector<HAND_TYPES> findPossibleHands(std::vector<Card> hand);
	std::vector<HAND_TYPES> findPossibleHandsFirstTurn(std::vector<Card> hand);
	HAND_TYPES findHandType(std::vector<Card> lastPlayed);
	std::vector<Card> findMoveFrom(HAND_TYPES ht, std::vector<Card> hand, std::vector<Card> lastPlayed);
	bool isHighestCardInGame(Card * c);
	bool compareCards(Card c1, Card c2);
	bool isValidPlay(std::vector<Card> move, std::vector<Card> lastPlayed);
	bool isValidMove(std::vector<Card> move, std::vector<Card> lastPlayed); 
	bool isValidMove(std::vector<Card> move);
	std::vector<Card> getSelectedCards(std::vector<Card> hand);
	std::vector<Card> deselectAllCards(std::vector<Card> hand);

	bool pairExists(std::vector<Card> hand);
	bool tripleExists(std::vector<Card> hand);
	bool fourKindExists(std::vector<Card> hand);
	bool straightExists(std::vector<Card> hand);
	bool straightExists(std::vector<Card> hand, bool firstTurn);
	bool pairStraightExists(std::vector<Card> hand);
	bool pairStraightExists(std::vector<Card> hand, bool firstTurn);

	std::string handToString(std::vector<Card> h);

	// --------- Testing ---------- //
	std::multimap<HAND_TYPES, int> findAllPossibleHands(std::vector<Card> hand);
	int findSizeOfStraightAt(int position, std::vector<Card> hand);
	int findSizeOfPairStraightAt(int position, std::vector<Card> hand);
	void addAllPairs(std::multimap<HAND_TYPES, int> & possibleHands, std::vector<Card> hand);
	void addAllTriples(std::multimap<HAND_TYPES, int> & possibleHands, std::vector<Card> hand);
	void addAllFourKinds(std::multimap<HAND_TYPES, int> & possibleHands, std::vector<Card> hand);
	void addAllStraights(std::multimap<HAND_TYPES, int> & possibleHands, std::vector<Card> hand);
	void addAllPairStraights(std::multimap<HAND_TYPES, int> & possibleHands, std::vector<Card> hand);
	// --------- Testing ---------- //
private:
	bool pairExistsFirstTurn(std::vector<Card> hand);
	bool tripleExistsFirstTurn(std::vector<Card> hand);
	bool fourKindExistsFirstTurn(std::vector<Card> hand);

	
};

#endif