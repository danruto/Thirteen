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
// Filename: HandEvaluator.cpp
// Description: This will determine what kind of playable hands exist in the Player's Hand
// Author: Danny Sok
// Date Created: 12/08/2013
// Date Last Modified: 27/09/2013

#include "HandEvaluator.h"
#include "..\util\Util.h"

std::vector<HAND_TYPES> HandEvaluator::findPossibleHands(std::vector<Card> hand, std::vector<Card> lastPlayed)
{
	//Util::Log("HandEvaluator::findPossibleHands");
	vector<HAND_TYPES> possibleHands;
	auto ht = findHandType(lastPlayed);

	switch (ht)
	{
		case HAND_TYPES::Single:
			if (hand.size() >= 1)
				possibleHands.push_back(Single);
			if (tripleExists(hand) && lastPlayed.back().getNumber() == Ace)
				possibleHands.push_back(Triple);
			if (pairStraightExists(hand) && lastPlayed.back().getNumber() == Two)
				possibleHands.push_back(PairStraight);
			break;
		case HAND_TYPES::Pair:
			if (pairExists(hand))
				possibleHands.push_back(Pair);
			break;
		case HAND_TYPES::Straight:
			if (straightExists(hand))
				possibleHands.push_back(Straight);
			break;
		case HAND_TYPES::Triple:
			if (tripleExists(hand))
				possibleHands.push_back(Triple);
			break;
		case HAND_TYPES::FourKind:
			if (fourKindExists(hand))
				possibleHands.push_back(FourKind);
			break;
		case HAND_TYPES::PairStraight:
			if (pairStraightExists(hand))
				possibleHands.push_back(PairStraight);
			break;
		default:
			possibleHands = findPossibleHands(hand);
			break;
	}

	return possibleHands;
}

std::vector<HAND_TYPES> HandEvaluator::findPossibleHands(std::vector<Card> hand)
{
	std::vector<HAND_TYPES> possibleHands;

	if (hand.size() >= 1)
		possibleHands.push_back(Single);
	if (pairExists(hand))
		possibleHands.push_back(Pair);
	if (tripleExists(hand))
		possibleHands.push_back(Triple);
	if (straightExists(hand))
		possibleHands.push_back(Straight);
	if (fourKindExists(hand))
		possibleHands.push_back(FourKind);
	if (pairStraightExists(hand))
		possibleHands.push_back(PairStraight);

	return possibleHands;
}

std::vector<Card> HandEvaluator::findMoveFrom(HAND_TYPES ht, std::vector<Card> hand, std::vector<Card> lastPlayed)
{
	std::vector<Card> c;

	return c;
}

bool HandEvaluator::pairExists(std::vector<Card> hand)
{
	if (hand.size() < 2)
		return false;

	for (int i = 0; i < hand.size()-1; i++)
	{
		if (hand[i].getNumber() == hand[i+1].getNumber())
			return true;
	}

	return false;
}

bool HandEvaluator::tripleExists(std::vector<Card> hand)
{
	if (hand.size() < 3)
		return false;

	for (int i = 0; i < hand.size()-2; i++)
	{
		if (hand[i].getNumber() == hand [i+1].getNumber() &&
			hand[i+1].getNumber() == hand[i+2].getNumber())
			return true;
	}

	return false;
}

bool HandEvaluator::fourKindExists(std::vector<Card> hand)
{
	if (hand.size() < 4)
		return false;

	for (int i = 0; i < hand.size()-3; i++)
	{
		if (hand[i].getNumber() == hand [i+1].getNumber() &&
			hand[i+1].getNumber() == hand[i+2].getNumber() &&
			hand[i+2].getNumber() == hand[i+3].getNumber())
			return true;
	}

	return false;
}


bool HandEvaluator::straightExists(std::vector<Card> hand)
{
	return straightExists(hand, false);
	/*if (hand.size() < 3)
		return false;

	// -------- Testing ------------- //
	int ele = 0;
	int length = 0;

	// size = 3
	// ele = 0
	// ++
	// ele = 1
	// ++
	// ele = 2
	// ++
	// ele == 3
	while (ele < hand.size()-1)
	{
		if (hand[hand.size() - 1].getNumber() == Ace &&
				 hand[hand.size() - 2].getNumber() == King &&
				 hand[hand.size() - 3].getNumber() == Queen)
		{
			return true;
		}

		// Sequence found
		if (hand[ele+1].getNumber() == hand[ele].getNumber() + 1)
		{
			length++;
			ele++;
		}
		else if (hand[ele+1].getNumber == Ace && hand[ele].getNumber() == King)
		{
			length++;
			ele++;
		}
		// Sequence ended/not found
		else
		{
			// We check here to ensure we get the full length of the straight
			// length = 2 means a straight of 3
			// e.g. 345
			// length++ on 4==3+1, and length++ on 5==4+1
			if (length == 2 || length == 3 || length == 4 || length == 5 || length == 6 || length == 7 || length == 8 || length == 9 || length == 10 || length == 11 || length == 12)
				return true;

			length = 0;
			ele++;
		}
	}

	// Need one here to check if we break before we want to, i.e. ele == hand.size()
	if (length == 2 || length == 3 || length == 4 || length == 5 || length == 6 || length == 7 || length == 8 || length == 9 || length == 10 || length == 11 || length == 12)
		return true;*/

	// -------- EndTesting ------------- //

	// 3-Card Straight
	/*if (hand.size() >= 3)
	{
		for (int i = 0; i < hand.size()-2; i++)
		{
			if (hand[i+2].getNumber()  == hand[i+1].getNumber() + 1 &&
				hand[i+1].getNumber()  == hand[i].getNumber() + 1)
			{
				return true;
			}
		}
	}

	// 4-Card Straight
	if (hand.size() >= 4)
	{
		for (int i = 0; i < hand.size()-3; i++)
		{
			if (hand[i+3].getNumber()  == hand[i+2].getNumber() + 1 &&
				hand[i+2].getNumber()  == hand[i+1].getNumber() + 1 &&
				hand[i+1].getNumber()  == hand[i].getNumber() + 1)
			{
				return true;
			}
		}
	}

	// 5-Card Straight
	if (hand.size() >= 5)
	{
		for (int i = 0; i < hand.size()-4; i++)
		{
			if (hand[i+4].getNumber()  == hand[i+3].getNumber() + 1 &&
				hand[i+3].getNumber()  == hand[i+2].getNumber() + 1 &&
				hand[i+2].getNumber()  == hand[i+1].getNumber() + 1 &&
				hand[i+1].getNumber()  == hand[i].getNumber() + 1)
			{
				return true;
			}
		}
	}

	// 6-Card Straight
	if (hand.size() >= 6)
	{
		for (int i = 0; i < hand.size()-5; i++)
		{
			if (hand[i+5].getNumber()  == hand[i+4].getNumber() + 1 &&
				hand[i+4].getNumber()  == hand[i+3].getNumber() + 1 &&
				hand[i+3].getNumber()  == hand[i+2].getNumber() + 1 &&
				hand[i+2].getNumber()  == hand[i+1].getNumber() + 1 &&
				hand[i+1].getNumber()  == hand[i].getNumber() + 1)
			{
				return true;
			}
		}
	}

	// 7-Card Straight
	if (hand.size() >= 7)
	{
		for (int i = 0; i < hand.size()-6; i++)
		{
			if (hand[i+6].getNumber()  == hand[i+5].getNumber() + 1 &&
				hand[i+5].getNumber()  == hand[i+4].getNumber() + 1 &&
				hand[i+4].getNumber()  == hand[i+3].getNumber() + 1 &&
				hand[i+3].getNumber()  == hand[i+2].getNumber() + 1 &&
				hand[i+2].getNumber()  == hand[i+1].getNumber() + 1 &&
				hand[i+1].getNumber()  == hand[i].getNumber() + 1)
			{
				return true;
			}
		}
	}

	// 8-Card Straight
	if (hand.size() >= 8)
	{
		for (int i = 0; i < hand.size()-7; i++)
		{
			if (hand[i+7].getNumber()  == hand[i+6].getNumber() + 1 &&
				hand[i+6].getNumber()  == hand[i+5].getNumber() + 1 &&
				hand[i+5].getNumber()  == hand[i+4].getNumber() + 1 &&
				hand[i+4].getNumber()  == hand[i+3].getNumber() + 1 &&
				hand[i+3].getNumber()  == hand[i+2].getNumber() + 1 &&
				hand[i+2].getNumber()  == hand[i+1].getNumber() + 1 &&
				hand[i+1].getNumber()  == hand[i].getNumber() + 1)
			{
				return true;
			}
		}
	}

	// 9-Card Straight
	if (hand.size() >= 9)
	{
		for (int i = 0; i < hand.size()-8; i++)
		{
			if (hand[i+8].getNumber()  == hand[i+7].getNumber() + 1 &&
				hand[i+7].getNumber()  == hand[i+6].getNumber() + 1 &&
				hand[i+6].getNumber()  == hand[i+5].getNumber() + 1 &&
				hand[i+5].getNumber()  == hand[i+4].getNumber() + 1 &&
				hand[i+4].getNumber()  == hand[i+3].getNumber() + 1 &&
				hand[i+3].getNumber()  == hand[i+2].getNumber() + 1 &&
				hand[i+2].getNumber()  == hand[i+1].getNumber() + 1 &&
				hand[i+1].getNumber()  == hand[i].getNumber() + 1)
			{
				return true;
			}
		}
	}

	// 10-Card Straight
	if (hand.size() >= 10)
	{
		for (int i = 0; i < hand.size()-9; i++)
		{
			if (hand[i+9].getNumber()  == hand[i+8].getNumber() + 1 &&
				hand[i+8].getNumber()  == hand[i+7].getNumber() + 1 &&
				hand[i+7].getNumber()  == hand[i+6].getNumber() + 1 &&
				hand[i+6].getNumber()  == hand[i+5].getNumber() + 1 &&
				hand[i+5].getNumber()  == hand[i+4].getNumber() + 1 &&
				hand[i+4].getNumber()  == hand[i+3].getNumber() + 1 &&
				hand[i+3].getNumber()  == hand[i+2].getNumber() + 1 &&
				hand[i+2].getNumber()  == hand[i+1].getNumber() + 1 &&
				hand[i+1].getNumber()  == hand[i].getNumber() + 1)
			{
				return true;
			}
		}
	}

	// 11-Card Straight
	if (hand.size() >= 11)
	{
		for (int i = 0; i < hand.size()-10; i++)
		{
			if (hand[i+10].getNumber() == hand[i+9].getNumber() + 1 &&
				hand[i+9].getNumber()  == hand[i+8].getNumber() + 1 &&
				hand[i+8].getNumber()  == hand[i+7].getNumber() + 1 &&
				hand[i+7].getNumber()  == hand[i+6].getNumber() + 1 &&
				hand[i+6].getNumber()  == hand[i+5].getNumber() + 1 &&
				hand[i+5].getNumber()  == hand[i+4].getNumber() + 1 &&
				hand[i+4].getNumber()  == hand[i+3].getNumber() + 1 &&
				hand[i+3].getNumber()  == hand[i+2].getNumber() + 1 &&
				hand[i+2].getNumber()  == hand[i+1].getNumber() + 1 &&
				hand[i+1].getNumber()  == hand[i].getNumber() + 1)
			{
				return true;
			}
		}
	}

	// 12-Card Straight
	if (hand.size() >= 12)
	{
		for (int i = 0; i < hand.size()-11; i++)
		{
			if (hand[i+11].getNumber() == hand[i+10].getNumber() + 1 &&
				hand[i+10].getNumber() == hand[i+9].getNumber() + 1 &&
				hand[i+9].getNumber()  == hand[i+8].getNumber() + 1 &&
				hand[i+8].getNumber()  == hand[i+7].getNumber() + 1 &&
				hand[i+7].getNumber()  == hand[i+6].getNumber() + 1 &&
				hand[i+6].getNumber()  == hand[i+5].getNumber() + 1 &&
				hand[i+5].getNumber()  == hand[i+4].getNumber() + 1 &&
				hand[i+4].getNumber()  == hand[i+3].getNumber() + 1 &&
				hand[i+3].getNumber()  == hand[i+2].getNumber() + 1 &&
				hand[i+2].getNumber()  == hand[i+1].getNumber() + 1 &&
				hand[i+1].getNumber()  == hand[i].getNumber() + 1)
			{
				return true;
			}
		}
	}

	// Dragon
	if (hand.size() == 13)
	{
		if (hand[12].getNumber() == Two &&
			hand[11].getNumber() == Ace &&
			hand[10].getNumber() == King &&
			hand[9].getNumber()  == Queen &&
			hand[8].getNumber()  == Jack &&
			hand[7].getNumber()  == hand[6].getNumber() + 1 &&
			hand[6].getNumber()  == hand[5].getNumber() + 1 &&
			hand[5].getNumber()  == hand[4].getNumber() + 1 &&
			hand[4].getNumber()  == hand[3].getNumber() + 1 &&
			hand[3].getNumber()  == hand[2].getNumber() + 1 &&
			hand[2].getNumber()  == hand[1].getNumber() + 1 &&
			hand[1].getNumber()  == hand[0].getNumber() + 1)
		{
			return true;
		}

	}*/

	//return false;
}

bool HandEvaluator::straightExists(std::vector<Card> hand, bool firstTurn)
{
	if (hand.size() < 3)
		return false;

	int ele = 0;
	int length = 0;
	while (ele < hand.size()-1)
	{
		if (hand[hand.size() - 1].getNumber() == Ace &&
				 hand[hand.size() - 2].getNumber() == King &&
				 hand[hand.size() - 3].getNumber() == Queen)
		{
			return true;
		}

		// Sequence found
		if (hand[ele+1].getNumber() == hand[ele].getNumber() + 1)
		{
			length++;
			ele++;
		}
		else if (hand[ele+1].getNumber() == Ace && hand[ele].getNumber() == King)
		{
			length++;
			ele++;
		}
		// Sequence ended/not found
		else
		{
			if (length == 2 || length == 3 || length == 4 || length == 5 || length == 6 || length == 7 || length == 8 || length == 9 || length == 10 || length == 11 || length == 12)
			{
				if (firstTurn)
				{
					if ((ele - length) == 0)
						return true;
					return false;
				}
				return true;
			}

			length = 0;
			ele++;
		}
	}

	// Need one here to check if we break before we want to, i.e. ele == hand.size()
	if (length == 2 || length == 3 || length == 4 || length == 5 || length == 6 || length == 7 || length == 8 || length == 9 || length == 10 || length == 11 || length == 12)
	{
		if (firstTurn)
		{
			if ((ele - length) == 0)
				return true;
			return false;
		}
		return true;
	}

	return false;
}

bool HandEvaluator::pairStraightExists(std::vector<Card> hand)
{
	return pairStraightExists(hand, false);
	/*if (hand.size() < 6)
		return false;

	// -------- Testing ------------- //
	int ele = 0;
	int length = 0;

	// size = 3
	// ele = 0
	// ++
	// ele = 1
	// ++
	// ele = 2
	// ++
	// ele == 3
	while (ele < hand.size()-1)
	{
		if (hand[hand.size() - 1].getNumber() == Ace &&
				 hand[hand.size() - 3].getNumber() == King &&
				 hand[hand.size() - 5].getNumber() == Queen)
		{
			return true;
		}

		// Sequence found
		// 01 23 45 67 89 1011 12
		// 33 44 55 66 77 88 9
		// 0- 2- 4-
		// 2- 4- 
		// ele 9, size == 11, last ele = 10
		// [0][ele][2] 

		// 1 3 5 7 9 11
		// 0 2 4 6 8 10 12
		
		//[0][1] [2][3] [4][5] [6][7] [8][9] [10][11] [12]
		//[o][o] [o][o] [o][o] [o][o] [o][o] [x] [x]	[x] [error]
		
		//if (ele >= (hand.size() - 4))
		// If hand size is odd i.e. 13 AND ele will error, then break
		if ((hand.size() % 2) && ele >= (hand.size() - 4))
			break;
		// if it is even
		else if (!(hand.size() % 2) && ele >= (hand.size() - 3))
			break;

		if (hand[ele+2].getNumber() == hand[ele].getNumber() + 1 &&
			hand[ele+1].getNumber() == hand[ele].getNumber() &&
			hand[ele+3].getNumber() == hand[ele+2].getNumber())
		//if (hand[ele+1].getNumber() == hand[ele].getNumber() + 1 &&
			//hand[ele-1].getNumber() == hand[ele].getNumber() &&
			//hand[ele+2].getNumber() == hand[ele+1].getNumber())
		{
			length += 2;
			ele += 2;
		}
		else if (hand[ele+3].getNumber() == Ace &&
			hand[ele+2].getNumber() == Ace &&
			hand[ele+1].getNumber() == hand[ele].getNumber() &&
			hand[ele].getNumber() == King)
		{
			length += 2;
			ele += 2;
		}
		// Sequence ended/not found
		else
		{
			// We check here to ensure we get the full length of the straight
			// length = 2 means a straight of 3
			// e.g. 345
			// length++ on 4==3+1, and length++ on 5==4+1
			if (length == 4 || length == 6 || length == 8 || length == 10)
				return true;

			length = 0;
			ele += 2;
		}
	}

	// Need one here to check if we break before we want to, i.e. ele == hand.size()
	if (length == 4 || length == 6 || length == 8 || length == 10)
		return true;*/

	// -------- EndTesting ------------- //

	/*

	// 6-Card Pair Straight (e.g. 334455)
	if (hand.size() >= 6)
	{
		for (int i = 0; i < hand.size()-5; i++)
		{
			// 01 23 45
			if (hand[i+4].getNumber() == hand[i+2].getNumber() + 1 &&
				hand[i+2].getNumber() == hand[i].getNumber() + 1 &&
				hand[i+5].getNumber() == hand[i+4].getNumber() && 
				hand[i+3].getNumber() == hand[i+2].getNumber() &&
				hand[i+1].getNumber() == hand[i].getNumber())
			{
				return true;
			}
		}
	}

	// 8-Card Pair Straight
	if (hand.size() >= 8)
	{
		for (int i = 0; i < hand.size()-7; i++)
		{
			// 01 23 45
			if (hand[i+6].getNumber() == hand[i+4].getNumber() + 1 &&
				hand[i+4].getNumber() == hand[i+2].getNumber() + 1 &&
				hand[i+2].getNumber() == hand[i].getNumber() + 1 &&
				hand[i+7].getNumber() == hand[i+6].getNumber() && 
				hand[i+5].getNumber() == hand[i+4].getNumber() && 
				hand[i+3].getNumber() == hand[i+2].getNumber() &&
				hand[i+1].getNumber() == hand[i].getNumber())
			{
				return true;
			}
		}
	}

	// 10-Card Pair Straight
	if (hand.size() >= 10)
	{
		for (int i = 0; i < hand.size()-9; i++)
		{
			// 01 23 45
			if (hand[i+8].getNumber() == hand[i+6].getNumber() + 1 &&
				hand[i+6].getNumber() == hand[i+4].getNumber() + 1 &&
				hand[i+4].getNumber() == hand[i+2].getNumber() + 1 &&
				hand[i+2].getNumber() == hand[i].getNumber() + 1 &&
				hand[i+9].getNumber() == hand[i+8].getNumber() && 
				hand[i+7].getNumber() == hand[i+6].getNumber() && 
				hand[i+5].getNumber() == hand[i+4].getNumber() && 
				hand[i+3].getNumber() == hand[i+2].getNumber() &&
				hand[i+1].getNumber() == hand[i].getNumber())
			{
				return true;
			}
		}
	}

	// 12-Card Pair Straight
	if (hand.size() >= 12)
	{
		for (int i = 0; i < hand.size()-11; i++)
		{
			// 01 23 45
			if (hand[i+10].getNumber() == hand[i+8].getNumber() + 1 &&
				hand[i+8].getNumber() == hand[i+6].getNumber() + 1 &&
				hand[i+6].getNumber() == hand[i+4].getNumber() + 1 &&
				hand[i+4].getNumber() == hand[i+2].getNumber() + 1 &&
				hand[i+2].getNumber() == hand[i].getNumber() + 1 &&
				hand[i+11].getNumber() == hand[i+10].getNumber() && 
				hand[i+9].getNumber() == hand[i+8].getNumber() && 
				hand[i+7].getNumber() == hand[i+6].getNumber() && 
				hand[i+5].getNumber() == hand[i+4].getNumber() && 
				hand[i+3].getNumber() == hand[i+2].getNumber() &&
				hand[i+1].getNumber() == hand[i].getNumber())
			{
				return true;
			}
		}
	}*/

	//return false;
}

bool HandEvaluator::pairStraightExists(std::vector<Card> hand, bool firstTurn)
{
	if (hand.size() < 6)
		return false;

	int ele = 0;
	int length = 0;
	while (ele < hand.size()-3)
	{
		//if (hand[hand.size() - 1].getNumber() == Ace &&
		//		 hand[hand.size() - 3].getNumber() == King &&
		//		 hand[hand.size() - 5].getNumber() == Queen)
		//{
		//	return true;
		//}

		// Sequence found
		// If hand size is odd i.e. 13 AND ele will error, then break
		if ((hand.size() % 2) && ele >= (hand.size() - 4))
			break;
		// if it is even
		else if (!(hand.size() % 2) && ele >= (hand.size() - 3))
			break;

		if (hand[ele+2].getNumber() == hand[ele].getNumber() + 1 &&
			hand[ele+1].getNumber() == hand[ele].getNumber() &&
			hand[ele+3].getNumber() == hand[ele+2].getNumber())
		{
			length += 2;
			ele += 2;
		}
		else if (hand[ele+3].getNumber() == Ace &&
			hand[ele+2].getNumber() == Ace &&
			hand[ele+1].getNumber() == hand[ele].getNumber() &&
			hand[ele].getNumber() == King)
		{
			length += 2;
			ele += 2;
		}
		// Sequence ended/not found
		else
		{
			// We check here to ensure we get the full length of the straight
			// length = 2 means a straight of 3
			// e.g. 345
			// length++ on 4==3+1, and length++ on 5==4+1
			if (length == 4 || length == 6 || length == 8 || length == 10)
			{
				if (firstTurn)
				{
					if ((ele - length) == 0)
						return true;
					return false;
				}
				return true;
			}

			length = 0;
			ele += 2;
		}
	}

	// Need one here to check if we break before we want to, i.e. ele == hand.size()
	if (length == 4 || length == 6 || length == 8 || length == 10)
	{
		if (firstTurn)
		{
			if ((ele - length) == 0)
				return true;
			return false;
		}
		return true;
	}

	return false;
}

// ----------------------------------------
// findHandTypes
// 
// Determine the type of hand that has been played or that the player holds
// Input:
//  hand is a vector containing the cards last played or hand to check
// Output:
//  A value of HAND_TYPES
// Conditions:
//  
/*HAND_TYPES HandEvaluator::findHandType(std::vector<Card> hand)
{
	if (hand.size() == 0)
		return NoHand;
	else if (hand.size() == 1)
		return Single;
	else if (hand.size() == 2)
		return Pair;
	

	std::vector<Card> c;
	int count = hand.size();

	// Find type from current hand selection
	for (int i = 0; i < count; i++)
	{
		c.push_back(hand[i]);
	}
	
	// Check for Straights, Triples (Card length: 3)
	if (count >= 3)
	{
		if (c[2].getNumber() == Ace && c[1].getNumber() == King && c[0].getNumber() == Queen) 
		{
			return Straight;
		}
		else if (c[2].getNumber() == c[1].getNumber() + 1 && 
					c[1].getNumber() == c[0].getNumber() + 1)
		{
			// Straight e.g. 3456
			return Straight;
		}
		else if (c[2].getNumber() == c[1].getNumber() && 
				c[1].getNumber() == c[0].getNumber() && 
				count == 3)
		{
			// triple e.g. 333
			return Triple;
		}
	}

	if (count >= 4)
	{
		// Check for FourKinds (Card length: 4)
		if (c[3].getNumber() == c[2].getNumber() &&
				c[2].getNumber() == c[1].getNumber() && 
				c[1].getNumber() == c[0].getNumber() && 
				count == 4)
		{
			// quad e.g. 4444
			return FourKind;
		}


	}

	if (count >= 6)
	{
		// Check for PairStraights (Card length: 6)
		for (int i = 0; i < count-5; i++)
		{
			if (c[i].getNumber() == c[i+1].getNumber() &&
				 c[i+2].getNumber() == c[i+3].getNumber() &&
				 c[i+2].getNumber() == c[i].getNumber() + 1 &&
				 c[i+4].getNumber() == c[i+5].getNumber() &&
				 c[i+4].getNumber() == c[i+2].getNumber() + 1)
			{
				// bomb e.g 334455
				return PairStraight;
			}
		}
	}

	// 4-Card Straight
	if (hand.size() >= 4)
	{
		for (int i = 0; i < hand.size()-3; i++)
		{
			if (hand[i+3].getNumber()  == hand[i+2].getNumber() + 1 &&
				hand[i+2].getNumber()  == hand[i+1].getNumber() + 1 &&
				hand[i+1].getNumber()  == hand[i].getNumber() + 1)
			{
				return Straight;
			}
		}
	}

	// 5-Card Straight
	if (hand.size() >= 5)
	{
		for (int i = 0; i < hand.size()-4; i++)
		{
			if (hand[i+4].getNumber()  == hand[i+3].getNumber() + 1 &&
				hand[i+3].getNumber()  == hand[i+2].getNumber() + 1 &&
				hand[i+2].getNumber()  == hand[i+1].getNumber() + 1 &&
				hand[i+1].getNumber()  == hand[i].getNumber() + 1)
			{
				return Straight;
			}
		}
	}

	// 6-Card Straight
	if (hand.size() >= 6)
	{
		for (int i = 0; i < hand.size()-5; i++)
		{
			if (hand[i+5].getNumber()  == hand[i+4].getNumber() + 1 &&
				hand[i+4].getNumber()  == hand[i+3].getNumber() + 1 &&
				hand[i+3].getNumber()  == hand[i+2].getNumber() + 1 &&
				hand[i+2].getNumber()  == hand[i+1].getNumber() + 1 &&
				hand[i+1].getNumber()  == hand[i].getNumber() + 1)
			{
				return Straight;
			}
		}
	}

	// 7-Card Straight
	if (hand.size() >= 7)
	{
		for (int i = 0; i < hand.size()-6; i++)
		{
			if (hand[i+6].getNumber()  == hand[i+5].getNumber() + 1 &&
				hand[i+5].getNumber()  == hand[i+4].getNumber() + 1 &&
				hand[i+4].getNumber()  == hand[i+3].getNumber() + 1 &&
				hand[i+3].getNumber()  == hand[i+2].getNumber() + 1 &&
				hand[i+2].getNumber()  == hand[i+1].getNumber() + 1 &&
				hand[i+1].getNumber()  == hand[i].getNumber() + 1)
			{
				return Straight;
			}
		}
	}

	// 8-Card Straight
	if (hand.size() >= 8)
	{
		for (int i = 0; i < hand.size()-7; i++)
		{
			if (hand[i+7].getNumber()  == hand[i+6].getNumber() + 1 &&
				hand[i+6].getNumber()  == hand[i+5].getNumber() + 1 &&
				hand[i+5].getNumber()  == hand[i+4].getNumber() + 1 &&
				hand[i+4].getNumber()  == hand[i+3].getNumber() + 1 &&
				hand[i+3].getNumber()  == hand[i+2].getNumber() + 1 &&
				hand[i+2].getNumber()  == hand[i+1].getNumber() + 1 &&
				hand[i+1].getNumber()  == hand[i].getNumber() + 1)
			{
				return Straight;
			}
		}
	}

	// 9-Card Straight
	if (hand.size() >= 9)
	{
		for (int i = 0; i < hand.size()-8; i++)
		{
			if (hand[i+8].getNumber()  == hand[i+7].getNumber() + 1 &&
				hand[i+7].getNumber()  == hand[i+6].getNumber() + 1 &&
				hand[i+6].getNumber()  == hand[i+5].getNumber() + 1 &&
				hand[i+5].getNumber()  == hand[i+4].getNumber() + 1 &&
				hand[i+4].getNumber()  == hand[i+3].getNumber() + 1 &&
				hand[i+3].getNumber()  == hand[i+2].getNumber() + 1 &&
				hand[i+2].getNumber()  == hand[i+1].getNumber() + 1 &&
				hand[i+1].getNumber()  == hand[i].getNumber() + 1)
			{
				return Straight;
			}
		}
	}

	// 10-Card Straight
	if (hand.size() >= 10)
	{
		for (int i = 0; i < hand.size()-9; i++)
		{
			if (hand[i+9].getNumber()  == hand[i+8].getNumber() + 1 &&
				hand[i+8].getNumber()  == hand[i+7].getNumber() + 1 &&
				hand[i+7].getNumber()  == hand[i+6].getNumber() + 1 &&
				hand[i+6].getNumber()  == hand[i+5].getNumber() + 1 &&
				hand[i+5].getNumber()  == hand[i+4].getNumber() + 1 &&
				hand[i+4].getNumber()  == hand[i+3].getNumber() + 1 &&
				hand[i+3].getNumber()  == hand[i+2].getNumber() + 1 &&
				hand[i+2].getNumber()  == hand[i+1].getNumber() + 1 &&
				hand[i+1].getNumber()  == hand[i].getNumber() + 1)
			{
				return Straight;
			}
		}
	}

	// 11-Card Straight
	if (hand.size() >= 11)
	{
		for (int i = 0; i < hand.size()-10; i++)
		{
			if (hand[i+10].getNumber() == hand[i+9].getNumber() + 1 &&
				hand[i+9].getNumber()  == hand[i+8].getNumber() + 1 &&
				hand[i+8].getNumber()  == hand[i+7].getNumber() + 1 &&
				hand[i+7].getNumber()  == hand[i+6].getNumber() + 1 &&
				hand[i+6].getNumber()  == hand[i+5].getNumber() + 1 &&
				hand[i+5].getNumber()  == hand[i+4].getNumber() + 1 &&
				hand[i+4].getNumber()  == hand[i+3].getNumber() + 1 &&
				hand[i+3].getNumber()  == hand[i+2].getNumber() + 1 &&
				hand[i+2].getNumber()  == hand[i+1].getNumber() + 1 &&
				hand[i+1].getNumber()  == hand[i].getNumber() + 1)
			{
				return Straight;
			}
		}
	}

	// 12-Card Straight
	if (hand.size() >= 12)
	{
		for (int i = 0; i < hand.size()-11; i++)
		{
			if (hand[i+11].getNumber() == hand[i+10].getNumber() + 1 &&
				hand[i+10].getNumber() == hand[i+9].getNumber() + 1 &&
				hand[i+9].getNumber()  == hand[i+8].getNumber() + 1 &&
				hand[i+8].getNumber()  == hand[i+7].getNumber() + 1 &&
				hand[i+7].getNumber()  == hand[i+6].getNumber() + 1 &&
				hand[i+6].getNumber()  == hand[i+5].getNumber() + 1 &&
				hand[i+5].getNumber()  == hand[i+4].getNumber() + 1 &&
				hand[i+4].getNumber()  == hand[i+3].getNumber() + 1 &&
				hand[i+3].getNumber()  == hand[i+2].getNumber() + 1 &&
				hand[i+2].getNumber()  == hand[i+1].getNumber() + 1 &&
				hand[i+1].getNumber()  == hand[i].getNumber() + 1)
			{
				return Straight;
			}
		}
	}

	// Dragon
	if (hand.size() == 13)
	{
		if (hand[12].getNumber() == hand[11].getNumber() + 1 &&
			hand[11].getNumber() == hand[10].getNumber() + 1 &&
			hand[10].getNumber() == hand[9].getNumber() + 1 &&
			hand[9].getNumber()  == hand[8].getNumber() + 1 &&
			hand[8].getNumber()  == hand[7].getNumber() + 1 &&
			hand[7].getNumber()  == hand[6].getNumber() + 1 &&
			hand[6].getNumber()  == hand[5].getNumber() + 1 &&
			hand[5].getNumber()  == hand[4].getNumber() + 1 &&
			hand[4].getNumber()  == hand[3].getNumber() + 1 &&
			hand[3].getNumber()  == hand[2].getNumber() + 1 &&
			hand[2].getNumber()  == hand[1].getNumber() + 1 &&
			hand[1].getNumber()  == hand[0].getNumber() + 1)
		{
			return Straight;
		}

	}

	// Check for Straights (Card length > 3)
	// 334455
	// 3 4 5
	// 345678
	// 3  6 8
	// 0-3-6
	// 3
	// 5+3=8
	// 3-0 = 3;
	// 3+3 = 6 != 5...
	// 
	int low = 0;
	int high = count-1;
	int mid = (high + low) / 2;

	if (c[high].getNumber() == Ace && c[high-1].getNumber() == King && c[high-2].getNumber() == Queen) 
	{
		return Straight;
	}
	else if (c[high].getNumber() == c[mid].getNumber() + (high-mid) && 
				c[mid].getNumber() == c[low].getNumber() + (mid-low))
	{
		// Straight e.g. 3456
		return Straight;
	}

	return NoHand;
}*/

// hand is the lastPlayed hand to evaluate
HAND_TYPES HandEvaluator::findHandType(std::vector<Card> hand)
{
	if (hand.size() == 0)
		return NoHand;
	if (hand.size() == 1)
		return Single;
	if (pairExists(hand) && hand.size() == 2)
		return Pair;
	if (tripleExists(hand) && hand.size() == 3)
		return Triple;
	if (straightExists(hand))
		return Straight;
	if (fourKindExists(hand) && hand.size() == 4)
		return FourKind;
	if (pairStraightExists(hand) && hand.size() > 5)
		return PairStraight;
	else
		return NoHand; // Invalid hand
}

#include "OpponentModeller.h"
bool HandEvaluator::isHighestCardInGame(Card * c)
{
	bool b = false;
	auto hand = OpponentModeller::getInstance().getCardsPlayedInLatestSession();
	int highestIndex = 0;

	if (c->getNumber() == Two && c->getSuite() == Heart)
		return true;

	for (int i = 0; i < hand.size(); i++)
	{
		// Special case checks
		if (hand[i].getNumber() == Two)
		{
			if (hand[highestIndex].getNumber() != Two)
				highestIndex = i;
			else if (hand[highestIndex].getNumber() == Two)
			{
				if (hand[i].getSuite() > hand[highestIndex].getSuite())
					highestIndex = i;
			}
		}
		if (hand[i].getNumber() == Ace)
		{
			if (hand[highestIndex].getNumber() != Two && hand[highestIndex].getNumber() != Ace)
				highestIndex = i;
			else if (hand[highestIndex].getNumber() == Ace)
			{
				if (hand[i].getSuite() > hand[highestIndex].getSuite())
					highestIndex = i;
			}
		}
		// Normal case checks
		else if (hand[i].getNumber() > hand[highestIndex].getNumber())
		{
			highestIndex = i;
		}
		// Suite checks
		else if (hand[i].getNumber() == hand[highestIndex].getNumber())
		{
			if (hand[i].getSuite() > hand[highestIndex].getSuite())
				highestIndex = i;
		}
	}

	if (c->getNumber() == Two)
	{
		if (hand[highestIndex].getNumber() != Two)
			b = false;
		else if (hand[highestIndex].getNumber() == Two)
		{
			if (c->getSuite() > hand[highestIndex].getSuite())
				b = true;
		}
	}
	else if (c->getNumber() == Ace)
	{
		if (hand[highestIndex].getNumber() != Two && hand[highestIndex].getNumber() != Ace)
			b = false;
		else if (hand[highestIndex].getNumber() == Ace)
		{
			if (c->getSuite() > hand[highestIndex].getSuite())
				b = true;
		}
	}
	else if (c->getNumber() > hand[highestIndex].getNumber())
	{
		b = true;
	}
	else if (c->getNumber() == hand[highestIndex].getNumber())
	{
		if (c->getSuite() > hand[highestIndex].getSuite())
			b = true;
	}

	return b;
}

bool HandEvaluator::compareCards(Card c1, Card c2)
{
	return (c1.getNumber() == c2.getNumber() && 
			c1.getSuite()  == c2.getSuite());
}

// ----------------------------------------
// isValidPlay
// 
// Returns if the cards selected can be played against the cards already played
// Input:
//	count is an int which is the current amount of cards played
//  lastPlayed is a vector containing the cards last played
//	pos is a vector of ints containing the element positions of the selected cards
// Output:
//  TRUE or FALSE depending on the tests below
// Conditions:
//  
bool HandEvaluator::isValidPlay(std::vector<Card> move, std::vector<Card> lastPlayed)
{
	int count = move.size();
	// If this is the first card then any combo should be fine provided the lowest card is involved
	// As such, needs to be cleaned up later. Stub for now
	if (lastPlayed.size() == 0)
	{
		// Need to actually check if it is a valid move
		return isValidMove(move);
	}
	
	// Check that the current count of cards is the same as the lastPlayed count
	// Then check to see what the lastPlayed type was
	// Then check to see if the selected cards will make the same type
	// If it does then compare the highest cards between the two vectors
	// if not, return false
	// To check type: Check the last 3 cards (or all the cards if there are only 3) to see
	// if it is sequential (bomb/straight) or if there are irregularities (bomb/combo)
	// Valid Cards: Triple, Combos (Both singluar and Pair [Straights]), Quads
	else if (count >= 3)// && lastPlayed.size() >= 3 && count == lastPlayed.size())
	{
		HAND_TYPES currentHand = findHandType(move);
		HAND_TYPES lastHand = findHandType(lastPlayed);
		if (currentHand == lastHand)
			return isValidMove(move, lastPlayed);
		else if (currentHand == Triple && lastHand == Single) 
		{
			if (lastPlayed.back().getNumber() == Ace)
				return true;
		}
		else if (currentHand == FourKind && lastHand == Single)
		{
			if (lastPlayed.back().getNumber() == Two)
				return true;
		}
		else if (currentHand == PairStraight && lastHand == Single)
		{
			if (lastPlayed.back().getNumber() == Two)
				return true;
		}
		else
			return false;
	}
	
	return isValidMove(move, lastPlayed);
}

bool HandEvaluator::isValidMove(std::vector<Card> move, std::vector<Card> lastPlayed)
{
	if (move.size() != lastPlayed.size())
		return false;

	if (move.back().getNumber() > lastPlayed.back().getNumber())
	{
		if ((lastPlayed.back().getNumber() == Ace && move.back().getNumber() != Two) )
			return false;
		else if ((lastPlayed.back().getNumber() == Two))
			return false;
		else
			return true;

	}
	else if (move.back().getNumber() == lastPlayed.back().getNumber())
	{
		if (move.back().getSuite() > lastPlayed.back().getSuite())
			return true;
	}
	else if (move.back().getNumber() < lastPlayed.back().getNumber())
	{
		if (move.back().getNumber() == Two)
			return true;
		else if (move.back().getNumber() == Ace && lastPlayed.back().getNumber() != Two)
			return true;
	}
	
	return false;
}

bool HandEvaluator::isValidMove(std::vector<Card> move)
{
	return findHandType(move) != NoHand;
}

std::vector<Card> HandEvaluator::getSelectedCards(std::vector<Card> hand)
{
	std::vector<Card> selectedCards;
	for (int i = 0; i < hand.size(); i++)
	{	
		if (hand[i].isSelected())
		{
			selectedCards.push_back(hand[i]);
			//selectedCards.back().setSelected(false);
		}
	}
	return selectedCards;
}

std::vector<Card> HandEvaluator::deselectAllCards(std::vector<Card> hand)
{
	std::vector<Card> selectedCards;
	for (int i = 0; i < hand.size(); i++)
	{
		selectedCards.push_back(hand[i]);
		selectedCards.back().setSelected(false);
	}
	return selectedCards;
}

std::string HandEvaluator::handToString(std::vector<Card> h)
{
	string tCard;

	if (h.size() > 0)
	{
		for (int i = 0; i < h.size(); i++)
		{
			if (i == h.size()-1)
				tCard += h[i].toString();
			else
				tCard += h[i].toString() + " ";
		}
	}
	else
		tCard = "";

	return tCard;
}

#pragma region FirstTurnStuff

std::vector<HAND_TYPES> HandEvaluator::findPossibleHandsFirstTurn(std::vector<Card> hand)
{
	std::vector<HAND_TYPES> possibleHands;

	if (hand.size() >= 1)
		possibleHands.push_back(Single);
	if (pairExistsFirstTurn(hand))
		possibleHands.push_back(Pair);
	if (tripleExistsFirstTurn(hand))
		possibleHands.push_back(Triple);
	if (straightExists(hand, true))
		possibleHands.push_back(Straight);
	if (fourKindExistsFirstTurn(hand))
		possibleHands.push_back(FourKind);
	if (pairStraightExists(hand, true))
		possibleHands.push_back(PairStraight);

	return possibleHands;
}

bool HandEvaluator::pairExistsFirstTurn(std::vector<Card> hand)
{
	if (hand.size() < 2)
		return false;

	if (hand[0].getNumber() == hand[1].getNumber())
		return true;

	return false;
}

bool HandEvaluator::tripleExistsFirstTurn(std::vector<Card> hand)
{
	if (hand.size() < 3)
		return false;

	if (hand[0].getNumber() == hand[1].getNumber() &&
		hand[1].getNumber() == hand[2].getNumber())
		return true;

	return false;
}

bool HandEvaluator::fourKindExistsFirstTurn(std::vector<Card> hand)
{
	if (hand.size() < 4)
		return false;

	if (hand[0].getNumber() == hand[1].getNumber() &&
		hand[1].getNumber() == hand[2].getNumber() &&
		hand[2].getNumber() == hand[3].getNumber())
		return true;

	return false;
}

#pragma endregion FirstTurnStuff

#pragma region TestingStuff
std::multimap<HAND_TYPES, int> HandEvaluator::findAllPossibleHands(std::vector<Card> hand)
{
	std::multimap<HAND_TYPES, int> possibleHands;

	// Add all singles
	for (int i = 0; i < hand.size(); i++)
	{
		// Add Single, At position i
		possibleHands.insert(std::pair<HAND_TYPES, int>(HAND_TYPES::Single, i));
	}

	// Add all pairs
	addAllPairs(possibleHands, hand);
	
	// Add all triples
	addAllTriples(possibleHands, hand);

	// Add all four of a kinds
	addAllFourKinds(possibleHands, hand);

	// Add all straights
	addAllStraights(possibleHands, hand);

	// Add all pairstraights
	addAllPairStraights(possibleHands, hand);

	return possibleHands;
}

int HandEvaluator::findSizeOfStraightAt(int position, std::vector<Card> hand)
{
	int ele = position;
	int length = 0;
	while (ele < hand.size()-1)
	{
		if (hand[hand.size() - 1].getNumber() == Ace &&
				 hand[hand.size() - 2].getNumber() == King &&
				 hand[hand.size() - 3].getNumber() == Queen)
		{
			return 3;
		}

		// Sequence found
		if (hand[ele+1].getNumber() == hand[ele].getNumber() + 1)
		{
			length++;
			ele++;
		}
		else if (hand[ele+1].getNumber() == Ace && hand[ele].getNumber() == King)
		{
			length++;
			ele++;
		}
		// Sequence ended/not found
		else
		{
			if (length >= 2)
			{
				// break here
				return length+1;
			}
		}
	}

	return length;
}

int HandEvaluator::findSizeOfPairStraightAt(int position, std::vector<Card> hand)
{
	int ele = position;
	int length = 0;
	while (ele < hand.size()-1)
	{
		if (hand[hand.size() - 1].getNumber() == Ace &&
				 hand[hand.size() - 3].getNumber() == King &&
				 hand[hand.size() - 5].getNumber() == Queen)
		{
			return 6;
		}

		// Sequence found
		// If hand size is odd i.e. 13 AND ele will error, then break
		if ((hand.size() % 2) && ele >= (hand.size() - 4))
			break;
		// if it is even
		else if (!(hand.size() % 2) && ele >= (hand.size() - 3))
			break;

		if (hand[ele+2].getNumber() == hand[ele].getNumber() + 1 &&
			hand[ele+1].getNumber() == hand[ele].getNumber() &&
			hand[ele+3].getNumber() == hand[ele+2].getNumber())
		{
			length += 2;
			ele += 2;
		}
		else if (hand[ele+3].getNumber() == Ace &&
			hand[ele+2].getNumber() == Ace &&
			hand[ele+1].getNumber() == hand[ele].getNumber() &&
			hand[ele].getNumber() == King)
		{
			length += 2;
			ele += 2;
		}
		// Sequence ended/not found
		else if (length >= 4)
			return length+2;
	}

	return length;
}

void HandEvaluator::addAllPairs(std::multimap<HAND_TYPES, int> & possibleHands, std::vector<Card> hand)
{
	if (hand.size() >= 2)
	{
		for (int i = 0; i < hand.size()-1; i++)
		{
			if (hand[i].getNumber() == hand[i+1].getNumber())
				possibleHands.insert(std::pair<HAND_TYPES, int>(HAND_TYPES::Pair, i));
		}
	}
}

void HandEvaluator::addAllTriples(std::multimap<HAND_TYPES, int> & possibleHands, std::vector<Card> hand)
{
	if (hand.size() >= 3)
	{
		for (int i = 0; i < hand.size()-2; i++)
		{
			if (hand[i].getNumber() == hand [i+1].getNumber() &&
				hand[i+1].getNumber() == hand[i+2].getNumber())
				possibleHands.insert(std::pair<HAND_TYPES, int>(HAND_TYPES::Triple, i));
		}
	}
}

void HandEvaluator::addAllFourKinds(std::multimap<HAND_TYPES, int> & possibleHands, std::vector<Card> hand)
{
	if (hand.size() >= 4)
	{
		for (int i = 0; i < hand.size()-3; i++)
		{
			if (hand[i].getNumber() == hand [i+1].getNumber() &&
				hand[i+1].getNumber() == hand[i+2].getNumber() &&
				hand[i+2].getNumber() == hand[i+3].getNumber())
				possibleHands.insert(std::pair<HAND_TYPES, int>(HAND_TYPES::FourKind, i));
		}
	}
}

void HandEvaluator::addAllStraights(std::multimap<HAND_TYPES, int> & possibleHands, std::vector<Card> hand)
{
	if (hand.size() <= 2)
		return;

	int ele = 0;
	int length = 0;
	while (ele < hand.size()-1)
	{
		/*if (hand[hand.size() - 1].getNumber() == Ace &&
				 hand[hand.size() - 2].getNumber() == King &&
				 hand[hand.size() - 3].getNumber() == Queen)
		{
			possibleHands.insert(std::pair<HAND_TYPES, int>(HAND_TYPES::Straight, hand.size()-3));
		}*/

		// Sequence found
		if (hand[ele+1].getNumber() == hand[ele].getNumber() + 1)
		{
			// Just to make sure we register ALL possible straights
			if (length >= 2)
				possibleHands.insert(std::pair<HAND_TYPES, int>(HAND_TYPES::Straight, ele-length)); 

			length++;
			ele++;
		}
		else if (hand[ele+1].getNumber() == Ace && hand[ele].getNumber() == King)
		{
			if (length >= 2)
				possibleHands.insert(std::pair<HAND_TYPES, int>(HAND_TYPES::Straight, ele-length)); 

			length++;
			ele++;
		}
		// Sequence ended/not found
		else
		{
			if (length == 2 || length == 3 || length == 4 || length == 5 || length == 6 || length == 7 || length == 8 || length == 9 || length == 10 || length == 11 || length == 12)
			{
				possibleHands.insert(std::pair<HAND_TYPES, int>(HAND_TYPES::Straight, ele-length)); 
			}

			length = 0;
			ele++;
		}
	}

	// Need one here to check if we break before we want to, i.e. ele == hand.size()
	if (length == 2 || length == 3 || length == 4 || length == 5 || length == 6 || length == 7 || length == 8 || length == 9 || length == 10 || length == 11 || length == 12)
	{
		possibleHands.insert(std::pair<HAND_TYPES, int>(HAND_TYPES::Straight, ele-length));
	}
}

void HandEvaluator::addAllPairStraights(std::multimap<HAND_TYPES, int> & possibleHands, std::vector<Card> hand)
{
	if (hand.size() <= 6)
		return;

	int ele = 0;
	int length = 0;

	while (ele < hand.size()-4)
	{
		// Sequence found
		// If hand size is odd i.e. 13 AND ele will error, then break
		if ((hand.size() % 2) && ele >= (hand.size() - 4))
			break;
		// if it is even
		else if (!(hand.size() % 2) && ele >= (hand.size() - 3))
			break;

		if (hand[ele+2].getNumber() == hand[ele].getNumber() + 1 &&
			hand[ele+1].getNumber() == hand[ele].getNumber() &&
			hand[ele+3].getNumber() == hand[ele+2].getNumber())
		{
			length += 2;
			ele += 2;
		}
		else if (hand[ele+3].getNumber() == Ace &&
			hand[ele+2].getNumber() == Ace &&
			hand[ele+1].getNumber() == hand[ele].getNumber() &&
			hand[ele].getNumber() == King)
		{
			length += 2;
			ele += 2;
		}
		// Sequence ended/not found
		else
		{
			// We check here to ensure we get the full length of the straight
			// length = 2 means a straight of 3
			// e.g. 345
			// length++ on 4==3+1, and length++ on 5==4+1
			if (length == 4 || length == 6 || length == 8 || length == 10)
			{
				possibleHands.insert(std::pair<HAND_TYPES, int>(HAND_TYPES::PairStraight, ele-length)); 
			}

			length = 0;
			ele += 2;
		}
	}

	// Need one here to check if we break before we want to, i.e. ele == hand.size()
	if (length == 4 || length == 6 || length == 8 || length == 10)
	{
		possibleHands.insert(std::pair<HAND_TYPES, int>(HAND_TYPES::PairStraight, ele-length));
	}
}
#pragma endregion