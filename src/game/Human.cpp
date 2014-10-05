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
// Filename: Human.cpp
// Description: Representation of a player for the game
// Author: Danny Sok
// Date Created: 10/04/2013
// Date Last Modified: 9/05/2013

#include "Human.h"
#include "..\ai\HandEvaluator.h"
#include "..\util\Util.h"

// ----------------------------------------
// 
// 
// 
// Input:
//  
// Output:
//  
// Conditions:
//  
Human::Human()
{
}

// ----------------------------------------
// Player (constructor)
// 
// Setup the player according to the input and setup other variables
// Input:
//  name is a string of the player name
// Output:
//  
// Conditions:
//  
Human::Human(string name)
{
	//setup(name, HUMAN);
}

// ----------------------------------------
// ~Player (deconstructor)
// 
// Deletes the player and cleaup routines when player is deleted
// Input:
//  
// Output:
//  
// Conditions:
//  
Human::~Human()
{

}

void Human::setup(string name, PLAYER_TYPES type, int pos)
{
	m_name = name;
	m_type = type;
	m_turnCount = 0;
	m_playCount = 0;
	m_bTurn = false;
	m_firstTurn = false;
	m_pos = pos;
	m_madeMove = false;
}

// ----------------------------------------
// pass
// 
// Pass the players turn when they do not wish to play the current turn
// Input:
//  
// Output:
//   
// Conditions:
//  
void Human::pass()
{
	m_turnCount++;
	m_bTurn = false;
	//Util::Log("Human::pass()");
}


// ----------------------------------------
// play
// 
// Play the cards that have been selected
// Input:
//  lastPlayed is a vector containing the cards last played
// Output:
//  cardsToPlay which is a vector containing the cards to play
// Conditions:
//  
/*----------------------------------------
Logic:
1. Count how many cards selected
2. Use the count to check if the cards are valid for that count i.e.
One card will be checked against the single rule, Two cards will be checked against the Pair rule, Three cards will be checked against the 
Triple and Straight rules, Four or more will be checked as a straight, pair straight, quad bomb etc
----------------------------------------*/
std::vector<Card> Human::play(std::vector<Card> lastPlayed)
{
	HandEvaluator eval;
	vector<Card> cardsToPlay;
	if (m_pos == 0 && m_type == PLAYER_TYPES::HUMAN)
	{
		m_madeMove = true;
	}

	if (m_madeMove)
	{
		std::vector<Card> selectedCards;
		//if (lock())
		//{
		lock();
			selectedCards = eval.getSelectedCards(m_cards);
			if (eval.isValidPlay(selectedCards, lastPlayed))
			{
				for (int i = 0; i < m_cards.size(); i++)
				{	
					if (m_cards[i].isSelected())
					{
						m_cards[i].setSelected(false);
						cardsToPlay.push_back(m_cards[i]);
						m_cards.erase(m_cards.begin() + i);
						i--; // Need to go back 1 due to us having deleted one
					}
				}
			}
			unlock();
		//}

		if (cardsToPlay.size() > 0)
		{
			pass(); // setting turn to false is done here
			m_playCount++;
			m_lastPlayed.clear();
			Util::Log("Human::play() - Cardtoplay > 0");
		}

		if (selectedCards.size() == 0 && m_type != PLAYER_TYPES::HUMAN)
		{
			pass();
			m_lastPlayed.clear();
			Util::Log("Human::pass() - selectedCards = 0");
		}
		

		m_madeMove = false;
	}

	// determine if the cards are valid combo
	// determine if it can be played into the table
	// if they can, delete from hand
	// add to table
	return cardsToPlay;
}

void Human::quit()
{

}

// ----------------------------------------
// sortCards
// 
// Sorts the cards using bubble sort. The lowest card to highest card.
// Input:
//  
// Output:
//  
// Conditions:
//  
void Human::sortCards()
{
	//std::sort(cards.begin(), cards.end(), cmp());
	int swapped;
    int i;
    for (i = 1; i < m_cards.size(); i++)
    {
        swapped = 0;    //this flag is to check if the array is already sorted
        int j;
		Card temp;
        for(j = 0; j < m_cards.size() - i; j++)
        {
			if (m_cards[j].getNumber() > m_cards[j + 1].getNumber())
			{
				if (! (m_cards[j + 1].getNumber() == Ace || m_cards[j + 1].getNumber() == Two) )
				{
					temp = m_cards[j];
					m_cards[j] = m_cards[j+1];
					m_cards[j+1] = temp;
					swapped = 1;
				}
			}
			else if (m_cards[j].getNumber() < m_cards[j + 1].getNumber())
			{
				if (m_cards[j].getNumber() == Ace || m_cards[j].getNumber() == Two)
				{
					temp = m_cards[j];
					m_cards[j] = m_cards[j+1];
					m_cards[j+1] = temp;
					swapped = 1;
				}
			}
			
			else //if left.Number == right.Number
			{
				if (m_cards[j].getSuite() > m_cards[j + 1].getSuite())
				{
					temp = m_cards[j];
					m_cards[j] = m_cards[j+1];
					m_cards[j+1] = temp;
					swapped = 1;
				}
			}
			
			if (m_cards[j].getNumber() == Two && m_cards[j + 1].getNumber() == Ace)
			{
				temp = m_cards[j];
				m_cards[j] = m_cards[j+1];
				m_cards[j+1] = temp;
				swapped = 1;
			}
        }
        if(!swapped){
            break; //if it is sorted then stop
        }
    }

	// Print out what cards it has just to see
	if (DEBUG_MODE)
	{
		Util::Log(toString() + handToString());
		Util::ResultsLog(toString() + handToString());
	}
}

// ----------------------------------------
// drawName
// 
// Draws the name of the player onto the game screen
// Input:
//  x is an int for the x position value
//	y is an int for the y position value
//	vagRounded is the ofTrueTypeFont that we will use to draw the string
// Output:
//  
// Conditions:
//  
void Human::drawName(int x, int y, ofTrueTypeFont * vagRounded)
{
	vagRounded->drawString(m_name, x, y);
}

void Human::setupCardImg()
{
	for (int i = 0; i < m_cards.size(); i++)
	{
		m_cards[i].setupImg();
	}
}

void Human::setCard(int pos, Card c)
{
	m_cards[pos] = c;
}

Card * Human::getCardAt(int pos)
{
	return &m_cards[pos];
}

std::vector<Card> Human::getCards()
{
	return m_cards;
}

std::vector<Card> & Human::getCardsPtr()
{
	return m_cards;
}

void Human::setCards(vector<Card> cards)
{
	if (!m_madeMove)
		m_cards = cards;
}

int Human::getTurn()
{
	return m_turnCount;
}

void Human::incTurn()
{
	m_turnCount++;
}

std::string Human::getName()
{
	return m_name;
}

bool Human::isTurn()
{
	return isFinished() ? false : m_bTurn;
}

void Human::setTurn(bool turn)
{
	m_bTurn = isFinished() ? false : turn;
}

bool Human::isFinished()
{
	return m_cards.size() == 0;
}

int Human::getCardPos(string name)
{
	for (int i = 0; i < m_cards.size(); i++)
	{
		if (m_cards[i].toString() == name)
			return i;
	}
	return -1;
}

void Human::setFirst(bool b)
{
	m_firstTurn = b;
}

PLAYER_TYPES Human::getType()
{
	return m_type;
}

int Human::getPlayCount()
{
	return m_playCount;
}

void Human::incWinCount()
{
	m_winCount++;
}

void Human::incLossCount()
{
	m_lossCount++;
}

int Human::getWinCount()
{
	return m_winCount;
}

int Human::getLossCount()
{
	return m_lossCount;
}

bool Human::isMakingMove()
{
	return isThreadRunning();
}

bool Human::isTurnToMakeMove()
{
	return isTurn() && !isMakingMove();
}

bool Human::hasMadeMove()
{
	return m_madeMove;
}

void Human::start()
{
	if (!isMakingMove() && !m_madeMove)
	{
		Util::Log("Human::Start Thread");
		
		m_startThreadTime = ofGetElapsedTimef();
		try
		{
			startThread(true, false);
		}
		catch (Poco::SystemException e)
		{
			Util::Log( "Exception in start(): " + e.message() );
			Util::Log( "hasMadeMove: " + ofToString(m_madeMove) + " isThreadRunning: " + ofToString(isThreadRunning()) + " now stopping thread!");
			// Since we can't actually stop the thread.. we'll try to and then uhh pretend we have made a move [Although this doesn't work because thee thread never starts anymore]
			//stop();
			//m_madeMove = true;
			while (isThreadRunning())
			{
				stopThread();
				Util::Log("Trying to stop thread ");
			}
		}
	}
}

void Human::stop()
{
	if(isMakingMove())
	{
		m_madeMove = true;
		stopThread();
		float stopTime = ofGetElapsedTimef() - m_startThreadTime;
		Util::Log("Human::Stop Thread - Took: " + ofToString(stopTime) + " seconds " + ofToString(isThreadRunning()));
		Util::ResultsLog("Human::Stop Thread - Took: " + ofToString(stopTime) + " seconds " + ofToString(isThreadRunning()));
		
	}
}

void Human::setLastPlayed(std::vector<Card> lastPlayed)
{
	if (lock())
	{
		m_lastPlayed = lastPlayed;
		unlock();
	}
}

std::string Human::handToString()
{
	string tCard;

	if (m_cards.size() > 0)
	{
		for (int i = 0; i < m_cards.size(); i++)
		{
			if (i == m_cards.size()-1)
				tCard += m_cards[i].toString();
			else
				tCard += m_cards[i].toString() + " ";
		}
	}
	else
		tCard = "";

	return tCard;
}

std::string Human::handToString(std::vector<Card> h)
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

std::string Human::playerTypeStr()
{
	std::string s = "";
	switch (m_type)
	{
		case AI_EASY:
			s = "Easy AI";
			break;
		case AI_MEDIUM:
			s = "Medium AI";
			break;
		case AI_HARD:
			s = "Hard AI";
			break;
		case HUMAN:
			s = "Human";
			break;
	}
	return s;
}

std::string Human::toString()
{
	return "Name: " + m_name + " AI Type: " + playerTypeStr() + " Pos: " + ofToString(m_pos) + " ";
}