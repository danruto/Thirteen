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
// Filename: GameTree.cpp
// Description: Represents a the game tree. Houses many nodes and potential possibilities for said nodes
// Author:		Danny Sok
// Date Created: 9/08/2013
// Date Last Modified: 27/09/2013
#include "GameTree.h"
#include "..\util\Util.h"
//#include <vld.h>

GameTree::~GameTree()
{
	//destroy();
	Util::Log("GameTree::~GameTree() - complete");
}

void GameTree::destroy()
{
	Util::Log("GameTree:: There are: " + ofToString(children.size()) + " children at start of destroy()");
	/*for (auto c: children)
	{
		c.destroy();
	}*/
	children.clear();
	//children.shrink_to_fit();

	//delete m_AIGame;
	//m_AIGame.reset();
	//m_AIGame = NULL;
	m_AIGameList.clear();

	Util::Log("GameTree:: There are: " + ofToString(children.size()) + " children at end of destroy()");
}

void GameTree::init(int playerNum, bool uct, bool godMode)
{
	if (DEBUG_MODE)
		Util::Log("GameTree::init");

	m_numOpponentCardGuesses = 1;
	m_playerNum = playerNum;
	m_uct = uct;
	m_childIndex = 0;

	if (Game::getInstance().getPlayerCount() == 2)
	{
		m_childMax = 5;
		m_simCount = 5;
	}
	else if (Game::getInstance().getPlayerCount() == 3)
	{
		m_childMax = 5;
		m_simCount = 3;
	}
	else
	{
		m_childMax = 2;
		m_simCount = 2;
	}

	if (godMode)
	{
		saveGameState();
		//generateChildren(uct);

		// For smarter implementation
		generatePossibleMoves(uct);
		saveRootNode();
	}
	else
	{
		for (int i = 0; i < m_numOpponentCardGuesses; i++)
		{
			m_AIGameList.push_back( make_shared<AIGame>(generateGameBasedOnCurrentState()) );
			//generateChildren(uct);
			
			// For smarter implementation
			generatePossibleMoves(uct);
			saveRootNode();
		}
		
	}
	

}

void GameTree::saveGameState()
{
	if (DEBUG_MODE)
		Util::Log("GameTree::saveGameState()");

	m_AIGame = make_shared<AIGame>();
	for (int i = 0; i < Game::getInstance().getPlayerCount(); i++)
	{
		IPlayerPtr p = Game::getInstance().getPlayer(i);
		auto hand = p->getCards();
		m_AIGame->addHand(hand);
	}

	m_AIGame->initAddedHands(m_playerNum, Game::getInstance().getPassCount(), m_playerNum, false, Game::getInstance().getLastPlayed().size() == 0);
	m_AIGameList.push_back(m_AIGame);
}

void GameTree::saveRootNode()
{
	std::unique_ptr<GameTreeNode> p (new GameTreeNode(children, m_AIGameList, m_playerNum, m_uct));
	parents.push_back(std::move(p));
	children.clear();
	//m_AIGameList.clear();
}

void GameTree::generateChildren(bool uct)
{
	if (DEBUG_MODE)
		Util::Log("GameTree::generateChildren()");
	//int nextPlayerNum = m_playerNum++ % Game::getInstance().getPlayerCount()-1;
	// Generate from the next player turn
	//for (int i = 0; i < Game::getInstance().getPlayerCount(); i++)
	//{
		// Get next player. But we want to generate for the whole turn (e.g. 4 turns all the way back to the player
		// so we need to do this here so that the loop will run the 4 turns rather than quitting at playerCount()
		int nextPlayerNum = m_playerNum + 1;
		if (nextPlayerNum >= Game::getInstance().getPlayerCount())
			nextPlayerNum = 0;

		IPlayerPtr p = Game::getInstance().getPlayer(m_playerNum);
		auto hand = p->getCards();
		auto lastPlayed = Game::getInstance().getLastPlayed();
		std::vector<HAND_TYPES> possibleHands = m_handEvaluator.findPossibleHands(hand, lastPlayed);
		//possibleHands.insert(possibleHands.begin(), NoHand); // Make sure we try to pass as well!
		
		if (lastPlayed.size() > 0)
			possibleHands.push_back(NoHand);

		for (int j = 0; j < possibleHands.size(); j++)
		{
			//auto move = m_handEvaluator.findMoveFrom(possibleHands[j], hand, lastPlayed);

			// For the first set of childern we need to generate what we can do, and then the children can generate
			// what the other players can do etc until it reaches itself again
			// TODO [8/11/2013]: 
			// Improve this to play every POSSIBLE possible hand, e.g. every card to play as single not just lowest
			// Basically std::map<HAND_TYPES, int> possibleHands; //where int is the count of the hand type
			// But that means the game tree needs to be re-written so that moves are generated NOW then passed into
			// the node, which generates all the moves it can make against this move and passes it onto its nodes etc.
			// so instead of passing in possibleHands[j], we pass in a std::vector<Card> move
			children.push_back( GameTreeNode(possibleHands[j], lastPlayed, m_AIGameList.back(), m_playerNum, uct) );
		}
	//}
}



// Bugging out, only choosing children[0] for some reason?
std::vector<Card> GameTree::getUCTBestMove3()
{
	Util::Log("GameTree::getUCTBestMove3()");

	// If we cannot play anything then there will be no children, so return what we have to pass
	//if (children.size() == 0 || children[m_childIndex].getMove().size() == 0)
		//return Game::getInstance().getPlayer(m_playerNum)->getCards();
	
	// If its in god mode, generate the children for it
	//if ( !(m_AIGameList.size() > 1) )
			//generateChildren(m_uct);

	findBestChildVisits();

	//return children[m_childIndex].getMove();
	//return parent->getMoveAt(m_childIndex);
	return parents[m_bestParent]->getMoveAt(m_childIndex);

}

// THOUGHT:
/*
So, generate say 10 games and play the possible strats for each of those 10 games.
Do a UCT MCTS search to see if we can find a good node. Say, if the payoff is like 5 or 10? we say we won that game? so play that
Maybe if 10 games is quick, we can expand the possible games to generate?
*/
#include "OpponentModeller.h"
#include "..\game\Deck.h"
AIGame GameTree::generateGameBasedOnCurrentState()
{
	if (DEBUG_MODE)
		Util::Log("GameTree::generateGameBasedOnCurrentState()");

	AIGame game;
	GameMove gm;
	// So we have cards played from OpponentModeller
	// We have the player cards
	std::vector<Card> cardsAlreadyUsed;
	std::vector<Card> deck;
	std::vector<Card> playerCards;
	auto usedFromOpModeller = OpponentModeller::getInstance().getCardsPlayedInLatestSession();
	if (Game::getInstance().getPlayer(m_playerNum)->lock())
	{
		playerCards = Game::getInstance().getPlayer(m_playerNum)->getCards();
		playerCards = m_handEvaluator.deselectAllCards(playerCards);
		Game::getInstance().getPlayer(m_playerNum)->unlock();
	}

	// Add the cards most recently played
	/*cardsAlreadyUsed.insert(cardsAlreadyUsed.end(), 
		Game::getInstance().getLastPlayed().begin(), 
		Game::getInstance().getLastPlayed().end());*/

	// Add the cards played already during the session which makes the above redundant I guess
	cardsAlreadyUsed.insert(cardsAlreadyUsed.end(), 
		usedFromOpModeller.begin(), 
		usedFromOpModeller.end());

	// Add player cards because only one person should have any card
	cardsAlreadyUsed.insert(cardsAlreadyUsed.end(), 
		playerCards.begin(), 
		playerCards.end());

	// Create the deck
	Deck d;

	// Setup the deck (Create cards and then shuffle them)
	d.setup();
	deck = d.getDeck();

	// Remove cards already used
	for (int i = 0; i < deck.size(); i++)
	{
		for (int j = 0; j < cardsAlreadyUsed.size(); j++)
		{
			if ( m_handEvaluator.compareCards(deck[i], cardsAlreadyUsed[j]) )
			{
				deck.erase(deck.begin() + i);
				if (i > 0) 
					i--;
			}
		}
	}

	// Allocate the rest of the cards randomly to the other players
	game.init(deck, playerCards, m_playerNum, Game::getInstance().getPassCount(), Game::getInstance().getLastPlayed().size() == 0);
	game.setGuessMode(true);
	return game;
}



// TODO:
void GameTree::findBestChildVisits()
{
	Util::Log("GameTree::findBestChildVisits()");

	//int childVisits = 0;
	m_childIndex = 0;
	m_bestParent = 0;
	std::vector<int> tmpVisits1;

	// Addin some temp values to overwrite
	std::vector<int> tmpVisits2;
	tmpVisits2.push_back(-1);
	tmpVisits2.push_back(-1);

	do
	{
		for (int i = 0; i < parents.size(); i++)
		{
			parents[i]->UCTSelectAction();
			//childVisits = parent->getBestChildVisits();
			tmpVisits1 = parents[i]->getBestChildInfo();
			if (tmpVisits1[1] > tmpVisits2[1])
			{
				tmpVisits2 = tmpVisits1;
				m_bestParent = i;
			}
		}

	} while (tmpVisits2[1] < m_childMax);
	//m_childIndex = parent->getBestChildIndex();
	m_childIndex = tmpVisits2[0];
	Util::Log("GameTree::findBestChildVisits() - ChildVisits: " + ofToString(tmpVisits2) + " ChildIndex: " + ofToString(m_childIndex));
	Util::ResultsLog("GameTree::findBestChildVisits() - ChildVisits: " + ofToString(tmpVisits2));// + " ChildIndex: " + ofToString(m_childIndex));
	for (int i = 0; i < parents.size(); i++)
	{
		parents[i]->printChildVisits();
	}
}

#pragma region SmarterAITest

// GameTree: Generate OUR hands
// GameTreeNode: Generate THEIR hands
void GameTree::generatePossibleMoves(bool uct)
{
	if (children.size() > 0)
		return;

	unique_ptr<HandEvaluator> eval;
	GameMove gm;
	
	std::vector< std::vector<Card> > moves;

	auto hand = Game::getInstance().getPlayer(m_playerNum)->getCards();
	auto lastPlayed = Game::getInstance().getLastPlayed();
	auto possibleHands = eval->findAllPossibleHands(hand);
	gm.init(hand);

	// Generate the moves
	for (multimap<HAND_TYPES, int>::iterator it = possibleHands.begin(); it != possibleHands.end(); ++it)
	{
		if ( (*it).first == HAND_TYPES::Single )
		{
			auto m = gm.playSingleAt((*it).second);
			auto selectedCards = eval->getSelectedCards(m);
			if (eval->isValidPlay(selectedCards, lastPlayed))
				//moves.push_back(selectedCards);
				moves.push_back(m);
			gm.deselectAllCards();
		}
		else if ( (*it).first == HAND_TYPES::Pair )
		{
			auto m = gm.playPairAt((*it).second);
			auto selectedCards = eval->getSelectedCards(m);
			if (eval->isValidPlay(selectedCards, lastPlayed))
				//moves.push_back(selectedCards);
				moves.push_back(m);
			gm.deselectAllCards();
		}
		else if ( (*it).first == HAND_TYPES::Triple )
		{
			auto m = gm.playTripleAt((*it).second);
			auto selectedCards = eval->getSelectedCards(m);
			if (eval->isValidPlay(selectedCards, lastPlayed))
				//moves.push_back(selectedCards);
				moves.push_back(m);
			gm.deselectAllCards();
		}
		else if ( (*it).first == HAND_TYPES::FourKind )
		{
			auto m = gm.playFourKindAt((*it).second);
			auto selectedCards = eval->getSelectedCards(m);
			if (eval->isValidPlay(selectedCards, lastPlayed))
				//moves.push_back(selectedCards);
				moves.push_back(m);
			gm.deselectAllCards();
		}
		else if ( (*it).first == HAND_TYPES::Straight )
		{
			auto m = gm.playStraightAt((*it).second, eval->findSizeOfStraightAt((*it).second, hand));
			auto selectedCards = eval->getSelectedCards(m);
			if (eval->isValidPlay(selectedCards, lastPlayed))
				//moves.push_back( selectedCards );
				moves.push_back(m);
			gm.deselectAllCards();
		}
		else if ( (*it).first == HAND_TYPES::PairStraight )
		{
			auto m = gm.playPairStraightAt( (*it).second, eval->findSizeOfPairStraightAt((*it).second, hand));
			auto selectedCards = eval->getSelectedCards(m);
			if (eval->isValidPlay(selectedCards, lastPlayed))
				//moves.push_back( selectedCards );
				moves.push_back(m);
			gm.deselectAllCards();
		}
	}

	if (lastPlayed.size() > 0)
		moves.push_back(hand); // We want a chance to pass as well

	// generate children
	generateChildrenSmart(moves, uct);
	
}

void GameTree::generateChildrenSmart(std::vector< std::vector<Card> > moves, bool uct)
{
	if (DEBUG_MODE)
		Util::Log("GameTree::generateChildrenSmart() - moves.size(): " + ofToString(moves.size()));
	// Get next player. But we want to generate for the whole turn (e.g. 4 turns all the way back to the player
	// so we need to do this here so that the loop will run the 4 turns rather than quitting at playerCount()

	for (int j = 0; j < moves.size(); j++)
	{
		//auto move = m_handEvaluator.findMoveFrom(possibleHands[j], hand, lastPlayed);

		// For the first set of childern we need to generate what we can do, and then the children can generate
		// what the other players can do etc until it reaches itself again
		// TODO [8/11/2013]: 
		// Improve this to play every POSSIBLE possible hand, e.g. every card to play as single not just lowest
		// Basically std::map<HAND_TYPES, int> possibleHands; //where int is the count of the hand type
		// But that means the game tree needs to be re-written so that moves are generated NOW then passed into
		// the node, which generates all the moves it can make against this move and passes it onto its nodes etc.
		// so instead of passing in possibleHands[j], we pass in a std::vector<Card> move
		// 9/11/2013 90% implemented
		children.push_back( GameTreeNode(moves[j], m_AIGameList.back(), m_playerNum, uct) );
	}
}
#pragma endregion

#pragma region Unused/Old functions
PLAYER_ACTIONS GameTree::findBestMove()
{
	if (DEBUG_MODE)
		Util::Log("GameTree::findBestMove()");

	PLAYER_ACTIONS strat = ACTION_A; // Pass if nothing else can be found
	int winningPayoff = 0;
	int winningChild = 0;

	for (int i = 0; i < children.size(); i++)
	{
		int payoff = children[i].findBestPayoff();
		if (payoff > winningPayoff)
		{
			winningPayoff = payoff;
			strat = children[i].getStrategy();
			winningChild = i;
		}
	}

	// then need to find the parent so we can get the first move to make not the final move

	return strat;
}

//----------------------------------------
// UCT Implementation
//----------------------------------------

// This is UCTSearch essentually
PLAYER_ACTIONS GameTree::getUCTBestMove()
{
	// generate children
	
	// play simulation
	for (int i = 0; i < children.size(); i++)
	{
		children[i].UCTPlaySimulation();
	}

	// get the best node
	int childIndex = -1;
	GameTreeNode * child = &children[0];
	for (int i = 0; i < children.size(); i++)
	{
		GameTreeNode * c = children[i].getBestChild();
		if (c->getVisits() > child->getVisits())
		{
			child = c;
			childIndex = i;
		}
	}

	// return the best strat to play from the best child of this (which will search for its best child)
	return children[childIndex].getStrategy();
}

PLAYER_ACTIONS GameTree::getUCTBestMove2()
{
	if (DEBUG_MODE)
		Util::Log("GameTree::getUCTBestMove2()");

	for (int i = 0; i < children.size(); i++)
	{
		children[i].UCTSelectAction();
	}

	int childIndex = 0;
	GameTreeNode * child = &children[0];
	for (int i = 0; i < children.size(); i++)
	{
		GameTreeNode * c = children[i].getBestChild();
		if (c->getVisits() > child->getVisits())
		{
			child = c;
			childIndex = i;
		}
	}

	// Should replace with the move played instead!
	// For this instance (Generate ALL in GOD mode)
	return children[childIndex].getStrategy();
}

void GameTree::findBestChild()
{
	// Find best node
	for (int i = 0; i < children.size(); i++)
	{
		children[i].UCTSelectAction();
	}

	m_childIndex = 0;
	if (children.size() > 0)
	{
		GameTreeNode * child = &children[0];
		for (int i = 0; i < children.size(); i++)
		{
			GameTreeNode * c = children[i].getBestChild();
			if (c->getVisits() > child->getVisits())
			{
				child = c;
				m_childIndex = i;
			}
		}
	}
}

void GameTree::findBestChildSmartPtr()
{
	// Find best node
	for (int i = 0; i < children.size(); i++)
	{
		children[i].UCTSelectAction();
	}

	m_childIndex = 0;
	if (children.size() > 0)
	{
		for (int i = 0; i < children.size(); i++)
		{
			shared_ptr<GameTreeNode> c (children[i].getBestChildSmartPtr()); // Copy pointer
			if (c->getVisits() > children[m_childIndex].getVisits())
			{
				m_childIndex = i;
			}
			c.reset();
		}
	}
}

void GameTree::findBestChildSimulate(int num)
{
	for (int j = 0; j < num; j++)
	{
		for (int i = 0; i < children.size(); i++)
		{
			children[i].UCTSelectAction();
		}

		// test- choose random nodes to expand and select actions for num of simulations. Test until there are at least 10 visits?
		// if this works out well, could be a good place to force the opponent modeller data on
		/*srand (time(NULL));
		int randChild = rand() % children.size();
		children[randChild].UCTSelectAction();*/
		
	}
}

/*
void GameTree::findBestChildVisits()
{
	Util::Log("GameTree::findBestChildVisits()");
	
	// Find best node
	for (int i = 0; i < children.size(); i++)
	{
		children[i].UCTExpand();
		//children[i].UCTSelectAction();
	}

	// Do another simulation
	findBestChildSimulate(m_simCount);

	m_childIndex = 0;
	int childVisits = 0;
	if (children.size() > 0)
	{
		for (int i = 0; i < children.size(); i++)
		{
			childVisits = children[i].getBestChildVisits();
			if (childVisits > children[m_childIndex].getBestChildVisits())
			{
				m_childIndex = i;
			}
		}
	}
	childVisits = children[m_childIndex].getBestChildVisits();
	// test - if there are less than 10 visits to the best child, simulate the game further
	// Apparently childVCists is not storing things correctly as the log indicates there are some with 10 visits but this still wont exit
	if (childVisits < m_childMax)
	{
		Util::Log("GameTree::findBestChildVisits() - There were only " + ofToString(childVisits) + " and so we must continue the search");
		findBestChildVisits();
	}
	Util::Log("GameTree::findBestChildVisits() - ChildVisits: " + ofToString(childVisits) + " ChildIndex: " + ofToString(m_childIndex));
	
	int childVisits = 0;
	m_childIndex = 0;

	do
	{
		// Find best node
		for (int i = 0; i < children.size(); i++)
		{
			children[i].UCTExpand();
			try
			{
				children[i].UCTSelectAction();
			} catch (exception e) { Util::Log("GameTree::findBestChildVisits(): " + ofToString(e.what())); }
		}

		// Do another simulation
		//findBestChildSimulate(m_simCount);
		
		if (children.size() > 0)
		{
			Util::Log("GameTree::findBestChildVisits() - looping for the children. cV: " + ofToString(childVisits));
			for (int i = 0; i < children.size(); i++)
			{
				childVisits = children[i].getBestChildVisits();
				if (childVisits > children[m_childIndex].getBestChildVisits())
				{
					m_childIndex = i;
				}
			}
			childVisits = children[m_childIndex].getBestChildVisits();
		}

	} while (childVisits < m_childMax);
	Util::Log("GameTree::findBestChildVisits() - ChildVisits: " + ofToString(childVisits) + " ChildIndex: " + ofToString(m_childIndex));
	Util::ResultsLog("GameTree::findBestChildVisits() - ChildVisits: " + ofToString(childVisits) + " ChildIndex: " + ofToString(m_childIndex));
	for (int i = 0; i < children.size(); i++)
	{
		Util::ResultsLog("GameTree:findBestChildVisits() at " + ofToString(i) + ": " + ofToString(children[i].getBestChildVisits()));
	}
}*/
#pragma endregion