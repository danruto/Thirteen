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
// Filename: GameTreeNode.cpp
// Description: Represents a node within the game tree and the generation of this node
// Author:		Danny Sok
// Date Created: 9/08/2013
// Date Last Modified: 27/09/2013
#include "GameTreeNode.h"
#include "..\util\Util.h"


// http://www.wisamyacteen.com/2012/11/an-artificial-intelligence-example-tic-tac-toe-using-c/

GameTreeNode::~GameTreeNode()
{
	//destroy();
}

void GameTreeNode::destroy()
{
	
	//delete m_AIGame;
	//delete m_ChildAIGame;

	children.clear();
	//children.shrink_to_fit();
	m_move.clear();
	m_prevMove.clear();
}

// Expand the node for the next round
void GameTreeNode::UCTExpand()
{
	if (DEBUG_MODE)
		Util::Log("GameTreeNode::UCTExpand()");
	if (children.size() == 0)
	{
		try
		{
			//generateChildren()
			//TODO: testing out generating all moves
			generateChildren(m_moves);
		} catch (exception e) 
		{ 
			Util::Log("GameTreeNode::UCTExpand()" + ofToString(e.what())); // Bad allocation error usually. So something wrong this generateChildren()
		}
	}
}

void GameTreeNode::update()
{
	unique_ptr<HandEvaluator> eval;
	m_visits++;
	
	if (isWinningNode())
		m_wins += 5;
	else if (isFreeReignNode())
		m_wins += 2;
	//else if(eval->getSelectedCards(m_move).size() == 0) // pass
		//m_wins; // do nothing. The bot likes passing too much so we need to reduce the chances of it doing that.
	//else
		//m_wins++;

	//if (DEBUG_MODE)
		//Util::Log("GameTreeNode::update() - Node Updated! - count for this node is - " + ofToString(m_visits) + " wins - " + ofToString(m_wins));
}

bool GameTreeNode::isLeaf()
{
	return children.size() == 0;
}

int GameTreeNode::getVisits()
{
	return m_visits;
}

// ------------------------------------------------------
// http://mcts.ai/code/java.html style implementation
void GameTreeNode::UCTSelectAction()
{
	if (DEBUG_MODE)
		Util::Log("GameTreeNode::UCTSelectAction()");

	std::vector<GameTreeNode*> visited;
	GameTreeNode * cur = this;
	visited.push_back(cur);

	while (!cur->isLeaf())
	{
		cur = cur->UCTSelectSL();
		visited.push_back(cur);
		if (cur == nullptr)
			break;
	}
	if (cur != nullptr)
	{
		cur->UCTExpand();
		GameTreeNode * newNode = cur->UCTSelectSL();
		visited.push_back(newNode);
	}

	for (int i = 0; i < visited.size(); i++)
	{
		if (visited[i] != nullptr)
			visited[i]->update();
	}
}


const double m_epsilon = 1e-6;

GameTreeNode * GameTreeNode::UCTSelectSL()
{
	if (DEBUG_MODE)
		Util::Log("GameTreeNode::UCTSelectSL()");

	GameTreeNode * result = nullptr;
	double bestUCT = 0;

	if (children.size() == 0)
		return result;

	for (int i = 0; i < children.size(); i++)
	{
		double a = children[i].getWins();
		double b = children[i].getVisits();
		double random = (rand() % 2) * DBL_EPSILON; // random int 0 or 1 times by epsilon. We want to explore something new
		/*double uctValue = children[i].getWins() / (children[i].getVisits() + m_epsilon) + 
			sqrt(log(children[i].getVisits()+1) / (children[i].getVisits() + m_epsilon)) +
			rand() * DBL_EPSILON;*/
		double uctValue = (a / (b + DBL_EPSILON)) +
			sqrt( (log (m_visits+1)) / (b + DBL_EPSILON)) +
			random;

		if (uctValue > bestUCT)
		{
			result = &children[i];
			bestUCT = uctValue;
		}
	}

	return result;
}

int GameTreeNode::getBestChildVisits()
{
	//if (DEBUG_MODE)
		//Util::Log("GameTreeNode::getBestChildVisits()");

	if (children.size() == 0)
	{
		return m_visits;
	}

	int j = 0;
	for (int i = 1; i < children.size(); i++)
	{
		if (children[i].getBestChildVisits() > children[j].getBestChildVisits())
		{
			j = i;
		}
	}
	if (!m_isParent)
		return children[j].getBestChildVisits() > m_visits ? children[j].getBestChildVisits() : m_visits;
	else
		return children[j].getBestChildVisits();
	
}

int GameTreeNode::getBestChildIndex()
{
	//if (DEBUG_MODE)
		//Util::Log("GameTreeNode::getBestChildVisits()");

	if (children.size() == 0)
	{
		return -1;
	}

	int j = 0;
	for (int i = 1; i < children.size(); i++)
	{
		if (children[i].getBestChildVisits() > children[j].getBestChildVisits())
		{
			j = i;
		}
	}

	for (int i = 0; i < children.size(); i++)
	{
		Util::ResultsLog("GameTreeNode::getBestChildIndex() at " + ofToString(i) + ": " + ofToString(children[i].getBestChildVisits()));
	}

	return j;
}

bool GameTreeNode::isWinningNode()
{
	std::shared_ptr<AIGame> game = m_AIGame.lock();
	if (game)
		return game->isWinner();
	else
		return false;
}

bool GameTreeNode::isFreeReignNode()
{
	std::shared_ptr<AIGame> game = m_AIGame.lock();
	if (game)
		return game->isFreeReign();
	else
		return false;
}

int GameTreeNode::getWins()
{
	return m_wins;
}

std::vector<Card> GameTreeNode::getMove()
{
	if (DEBUG_MODE)
		Util::Log("GameTreeNode::getMove()");
	return m_move;
}

#pragma region SmarterImplementation
GameTreeNode::GameTreeNode(std::vector<GameTreeNode> c, std::vector< std::shared_ptr<AIGame> > AIGameList, int playerNum, bool useUct, bool isParent)
{
	children = c;
	//m_AIGameList = AIGameList;
	m_AIGame = AIGameList.back();
	m_playerNum = playerNum;
	m_useUct = useUct;
	m_isParent = isParent;

	// Parent Node so null everything else;
	m_childIndex = 0;
	m_nextChildIndex = -1;
	m_winRate = 0;
	m_wins = 0;
	m_visits = 0;
}

GameTreeNode::GameTreeNode(std::vector<Card> prevMove, std::shared_ptr<AIGame> aiGame, int playerNum, bool useUct)
{
	//HandEvaluator eval;
	unique_ptr<HandEvaluator> eval;
	//if (DEBUG_MODE)
		//Util::Log("GameTreeNode::GameTreeNode() - Creating new node");
	//m_parent = parent;
	//m_handType = handType;
	m_move = prevMove;

	// Prev Move is OUR move
	m_prevMove = eval->getSelectedCards(prevMove);
	m_AIGame = aiGame;
	m_playerNum = playerNum;
	m_childIndex = 0;
	m_nextChildIndex = -1;
	m_winRate = 0;
	m_wins = 0;
	m_visits = 0;
	m_strat = m_prevMove.size() > 0 ? PLAYER_ACTIONS::ACTION_B : PLAYER_ACTIONS::ACTION_E;
	m_useUct = useUct;
	m_isParent = false;

	UCTK = 0.44;

	//generateMove();
	//generateMoveLowest();
	//generateMoveHighest();
	generatePossibleMoves();
}

void GameTreeNode::generatePossibleMoves()
{
	unique_ptr<HandEvaluator> eval;
	GameMove gm;
	
	auto game = m_AIGame.lock();
	std::vector< std::vector<Card> > moves;
	// Find all possible moves
	if (game)
	{
		int nextPlayerNum = m_playerNum+1;
		if (nextPlayerNum >= game->getPlayerCount())
			nextPlayerNum = 0;

		auto hand = game->getHand(nextPlayerNum);
		auto possibleHands = eval->findAllPossibleHands(hand);
  		gm.init(hand);

		if (m_prevMove.size() > 0)
			moves.push_back(hand); // We want a chance to pass as well

		// Generate the moves
		for (multimap<HAND_TYPES, int>::iterator it = possibleHands.begin(); it != possibleHands.end(); ++it)
		{
			if ( (*it).first == HAND_TYPES::Single )
			{
				auto m = gm.playSingleAt((*it).second);
				auto selectedCards = eval->getSelectedCards(m);
				if (eval->isValidPlay(selectedCards, m_prevMove))
					//moves.push_back(selectedCards);
					moves.push_back(m);
				gm.deselectAllCards();
			}
			else if ( (*it).first == HAND_TYPES::Pair )
			{
				auto m = gm.playPairAt((*it).second);
				auto selectedCards = eval->getSelectedCards(m);
				if (eval->isValidPlay(selectedCards, m_prevMove))
					//moves.push_back(selectedCards);
					moves.push_back(m);
				gm.deselectAllCards();
			}
			else if ( (*it).first == HAND_TYPES::Triple )
			{
				auto m = gm.playTripleAt((*it).second);
				auto selectedCards = eval->getSelectedCards(m);
				if (eval->isValidPlay(selectedCards, m_prevMove))
					//moves.push_back(selectedCards);
					moves.push_back(m);
				gm.deselectAllCards();
			}
			else if ( (*it).first == HAND_TYPES::FourKind )
			{
				auto m = gm.playFourKindAt((*it).second);
				auto selectedCards = eval->getSelectedCards(m);
				if (eval->isValidPlay(selectedCards, m_prevMove))
					//moves.push_back(selectedCards);
					moves.push_back(m);
				gm.deselectAllCards();
			}
			else if ( (*it).first == HAND_TYPES::Straight )
			{
				auto m = gm.playStraightAt((*it).second, eval->findSizeOfStraightAt((*it).second, hand));
				auto selectedCards = eval->getSelectedCards(m);
				if (eval->isValidPlay(selectedCards, m_prevMove))
					//moves.push_back( selectedCards );
					moves.push_back(m);
				gm.deselectAllCards();
			}
			else if ( (*it).first == HAND_TYPES::PairStraight )
			{
				auto m = gm.playPairStraightAt( (*it).second, eval->findSizeOfPairStraightAt((*it).second, hand));
				auto selectedCards = eval->getSelectedCards(m);
				if (eval->isValidPlay(selectedCards, m_prevMove))
					//moves.push_back( selectedCards );
					moves.push_back(m);
				gm.deselectAllCards();
			}
		}
	}

	m_moves = moves;
	//generateMoves(moves);
	//generateChildren(moves);
}

void GameTreeNode::generateMoves(std::vector< std::vector<Card> > moves)
{
	std::vector< std::vector<Card> > selectedMoves;
	HandEvaluator eval;
	
	for (int i = 0; i < moves.size(); i++)
	{
		selectedMoves.push_back(eval.getSelectedCards(moves[i]));
	}

	generateChildren(selectedMoves);
}

void GameTreeNode::generateChildren(std::vector< std::vector<Card> > moves)
{
	if (DEBUG_MODE)
		Util::Log("GameTreeNode::generateChildrenSpecial(), moves.size(): " + ofToString(moves.size()));
	int nextPlayerNum = m_playerNum + 1;
	std::shared_ptr<AIGame> game = m_AIGame.lock();
	if (game)
	{
		if (nextPlayerNum >= game->getPlayerCount())
			nextPlayerNum = 0;
	}

	for (int m = 0; m < moves.size(); m++)
	{
		m_AIGameList.push_back( make_shared<AIGame>( generateGame(moves[m])) );
		if (!m_AIGameList.back()->isOver())
		{
			// moves is next player moves? m_prevMove is my move i.e. the last played
			children.push_back( GameTreeNode(moves[m], m_AIGameList.back(), nextPlayerNum, m_useUct) );
		}
		else
			m_AIGameList.pop_back();
	}
}

AIGame GameTreeNode::generateGame(std::vector<Card> move)
{
	AIGame g;
	HandEvaluator m_handEvaluator;
	auto game = m_AIGame.lock();
	if (game)
	{
		auto selectedCards =  m_handEvaluator.getSelectedCards(move);
		for (int i = 0; i < game->getPlayerCount(); i++)
		{
			std::vector<Card> hand;

			// We need to remove the cards played from our hand before saving the game state
			if (i == m_playerNum)
			{
				auto remove = removeMoveFromHand(move);
				hand = remove.size() > 0 ? remove : game->getHand(i);
			}
			else
			{
				hand = game->getHand(i);
			}
			g.addHand(hand);
		}
		g.initAddedHands(m_playerNum,																		// Player Position / Number
						selectedCards.size() == 0 ? game->getPassCount() + 1 : game->getPassCount(),		// Pass or Play
						game->getOriginalPlayerPos(),														// Original Player Position (to determine winner)
						true,																				// Guess Mode = false, God Mode = true
						selectedCards.size() == 0,															// Free Reign Turn = size = 0, Normal Turn = size > 0
						game->getDepth());																	// Current node depth
	}
	return g;
}

std::vector<Card> GameTreeNode::removeMoveFromHand(std::vector<Card> move)
{
	//if (DEBUG_MODE)
		//Util::Log("GameTreeNode::removeMoveFromHand()");
	HandEvaluator m_handEvaluator;

	auto game = m_AIGame.lock();
	std::vector<Card> hand;
	if (game)
	{
		hand = game->getHand(m_playerNum);
		HandEvaluator handEval;
		auto selectedCards = handEval.getSelectedCards(move);
		bool erase = handEval.isValidPlay(selectedCards, m_prevMove);

		for (int i = 0; i < hand.size(); i++)
		{
			for (int j = 0; j < selectedCards.size(); j++)
			{
				if ( m_handEvaluator.compareCards(hand[i], selectedCards[j]) )
				{
					if (erase)
					{
						hand.erase(hand.begin() + i);
						if (i > 0) 
							i--;
					}
					else
						hand[i].setSelected(false); // deselect card if it is not a valid move and just pass the turn instead
				}
			}
		}
	}
	return hand;
}

std::vector<Card> GameTreeNode::getMoveAt(int index)
{
	HandEvaluator eval;
		for (int i = 0; i < children.size(); i++)
		{
			auto selCard = eval.getSelectedCards(children[i].getMove());
			Util::Log("Child " + ofToString(i) + ": " + eval.handToString(selCard));
			Util::ResultsLog("Child " + ofToString(i) + ": " + eval.handToString(selCard));
		}

	if (index >= children.size() || index < 0) 
	{
		//return m_AIGameList.back()->getHand(m_playerNum);
		auto game = m_AIGame.lock();
		if (game)
			return game->getHand(m_playerNum);

	}
	else
		return children[index].getMove();
}

// K: Index, E: Visits
std::vector<int> GameTreeNode::getBestChildInfo()
{
	std::vector<int> info;
	if (children.size() == 0)
	{
		info.push_back(-1);
		info.push_back(m_visits);
		return info;
	}

	

	int j = 0;
	for (int i = 1; i < children.size(); i++)
	{
		if (children[i].getBestChildVisits() > children[j].getBestChildVisits())
		{
			j = i;
		}
	}
	if (!m_isParent)
	{
		//return children[j].getBestChildVisits() > m_visits ? children[j].getBestChildVisits() : m_visits;
		//int index = children[j].getBestChildVisits() > m_visits ? j : -1;
		//int visits = children[j].getBestChildVisits() > m_visits ? children[j].getBestChildVisits() : m_visits;
		int index = j;
		int visits = children[j].getBestChildVisits();
		info.push_back(index);
		info.push_back(visits);
		return info;
	}
	else
	{
		//return children[j].getBestChildVisits();
		info.push_back(j);
		info.push_back(children[j].getBestChildVisits());
		return info;
	}
}

void GameTreeNode::printChildVisits()
{
	for (int i = 0; i < children.size(); i++)
	{
		Util::ResultsLog("GameTreeNode::getBestChildIndex() at " + ofToString(i) + ": " + ofToString(children[i].getBestChildVisits()));
	}
}
#pragma endregion

#pragma region OldFunctions
//-------------------------------------------------
// OLDER FUNCTIONS - PROBABLY NOT USING ANYMORE
//-------------------------------------------------
// ----------------------------------------
// GameTreeNode (constructor)
//  
//  Will setup the class
// Input:
//  gameState is a pointer to the Game
//	handType is the handtype that this node will play
//	move is the last played move (either Parent move from GameTree or Enemy move from GameTreeNode)
//	hand is the current hand of the player
//	parent is a pointer to the parent node
// Output:
//  
// Conditions:
//  
/*GameTreeNode::GameTreeNode(HAND_TYPES handType, std::vector<Card> move, GameTreeNode  * parent, AIGame & aiGame, int playerNum)
{
	//m_parent = parent;
	m_handType = handType;
	m_prevMove = move;
	m_AIGame = aiGame;
	m_playerNum = playerNum;
	m_strat = m_prevMove.size() > 0 ? PLAYER_ACTIONS::ACTION_B : PLAYER_ACTIONS::ACTION_E;

	// Generate the move to be taken for this node
	generateMove();

	// Generate the payoff for the move taken
	generatePayoff();

	// Generate children based on the move made
	generateChildren();
}*/

void GameTreeNode::UCTPlaySimulation()
{
	if (children.size() == 0)
		UCTExpand();

	int index = 0;
	for (int i = 0; i < children.size(); i++)
	{
		if (index >= 0)
			index = UCTSelect(i);

		children[i].update();
	}
}

// Select the best node in the child set
int GameTreeNode::UCTSelect(int nodeIndex)
{
	int result = -1;
	double bestUCT = 0;

	if (nodeIndex < children.size())
	{
		GameTreeNode * node = &children[nodeIndex];
		GameTreeNode * nextNode = children[nodeIndex].getNextChild();

		while (nextNode != nullptr)
		{
			double uctValue;
			if (nextNode->getVisits() > 0)
			{
				double winRate = nextNode->getWinRate();
				double uct = UCTK * sqrt( log(node->getVisits() / nextNode->getVisits()) );
				uctValue = winRate + uct;
			}
			else
			{
				// play random unexplored move
				uctValue = 10000 + 1000 * rand();
			}

			if (uctValue > bestUCT)
			{
				bestUCT = uctValue;
				result = nodeIndex; // next;
			}

			nextNode = children[nodeIndex].getNextChild();
		}
	}

	return result;
}

// Select best node from all children?
// For a selected depth search/simluation count
// Expand the branch IF it doesn't have any children
// then try to simulate the children just created
// ELSE just simulate the children already created.
// Increase the visits, update win status, and save the best child
// void UCTSearch()

GameTreeNode * GameTreeNode::getNextChild()
{
	if (m_nextChildIndex >= children.size())
		return nullptr;
	else
		return &children[m_nextChildIndex++];
}

//TODO
void GameTreeNode::generatePayoff()
{
	m_payoffValue = 1;

	if (m_move.size() > 0)
		m_payoffValue = 2;
}


int GameTreeNode::findBestPayoff()
{
	if (DEBUG_MODE)
		Util::Log("GameTreeNode::findBestPayoff()");
	int payoff = m_payoffValue;
	for (int i = 0; i < children.size(); i++)
	{
		int cPayoff = children[i].getPayoffValue();
		if (cPayoff > m_payoffValue)
		{
			payoff = cPayoff;
		}
	}

	// Update all payoffs for this branch
	updatePayoffs(payoff);
	return payoff;
}

HAND_TYPES GameTreeNode::getHandType()
{
	return m_handType;
}

PLAYER_ACTIONS GameTreeNode::getStrategy()
{
	return m_strat;
}

int GameTreeNode::getPayoffValue()
{
	return m_payoffValue;
}

void GameTreeNode::updatePayoffs(int payoff)
{
	m_payoffValue = payoff;

	for (int i = 0; i < children.size(); i++)
	{
		children[i].updatePayoffs(payoff);
	}
}

int GameTreeNode::getWinRate()
{
	if (m_visits > 0) 
		m_winRate = (double)m_wins / m_visits;

	return m_winRate;
}

//----------------------------------------
// UCT Implementation based on http://senseis.xmp.net/?UCT // http://mcts.ai/code/java.html
//----------------------------------------
GameTreeNode::GameTreeNode(HAND_TYPES handType, std::vector<Card> move, std::shared_ptr<AIGame> aiGame, int playerNum, bool useUct)
{
	//if (DEBUG_MODE)
		//Util::Log("GameTreeNode::GameTreeNode() - Creating new node");
	//m_parent = parent;
	m_handType = handType;
	m_prevMove = move;
	m_AIGame = aiGame;
	m_playerNum = playerNum;
	m_childIndex = 0;
	m_nextChildIndex = -1;
	m_winRate = 0;
	m_wins = 0;
	m_visits = 0;
	m_strat = m_prevMove.size() > 0 ? PLAYER_ACTIONS::ACTION_B : PLAYER_ACTIONS::ACTION_E;
	m_useUct = useUct;

	UCTK = 0.44;

	generateMove();
	//generateMoveLowest();
	//generateMoveHighest();
}

// Generate move based on what we are given
// 1. Check what HandType we will use
// 2. Find the cards that can be played based on the hand type
// 3. Remove them from the hand
// 4. Generate the 1st moves of the other players (to complete the one turn, so 3 moves MAX)
// 5. Generate a payoff now knowing the values
void GameTreeNode::generateMove()
{
	//if (DEBUG_MODE)
		//Util::Log("GameTreeNode::generateMove()");
	m_payoffValue = 0;
	// Remove cards from 'move' in 'hand'
	std::shared_ptr<AIGame> game = m_AIGame.lock();
	if (game)
	{
		gameMove.init(game->getHand(m_playerNum));
		srand (time(NULL));
		switch (m_handType)
		{
			case HAND_TYPES::Single:
				if ( (rand() % 2) == 0 )
					m_move = gameMove.playSingle(m_prevMove);
				else
					m_move = gameMove.playHighSingle(m_prevMove);
				
				break;
			case HAND_TYPES::Pair:
				if ( (rand() % 2) == 0 )
					m_move = gameMove.playPair(m_prevMove);
				else
					m_move = gameMove.playHighPair(m_prevMove);
				break;
			case HAND_TYPES::Triple:
				if ( (rand() % 2) == 0 )
					m_move = gameMove.playTriple(m_prevMove);
				else
					m_move = gameMove.playHighTriple(m_prevMove);
				break;
			case HAND_TYPES::Straight:
				if ( (rand() % 2) == 0 )
					m_move = gameMove.playStraight(m_prevMove);
				else
					m_move = gameMove.playHighStraight(m_prevMove);
				break;
			case HAND_TYPES::FourKind:
				if ( (rand() % 2) == 0 )
					m_move = gameMove.playFourKind(m_prevMove);
				else
					m_move = gameMove.playHighFourKind(m_prevMove);
				break;
			case HAND_TYPES::PairStraight:
				if ( (rand() % 2) == 0 )
					m_move = gameMove.playPairStraight(m_prevMove);
				else
					m_move = gameMove.playHighPairStraight(m_prevMove);
				break;
			default:
				m_move = game->getHand(m_playerNum);
				// pass. So we don't need to give move any cards
				break;
		}
	}
	
}

void GameTreeNode::generateMoveLowest()
{
	//if (DEBUG_MODE)
		//Util::Log("GameTreeNode::generateMoveLowest()");
	m_payoffValue = 0;
	// Remove cards from 'move' in 'hand'
	std::shared_ptr<AIGame> game = m_AIGame.lock();
	if (game)
	{
		gameMove.init(game->getHand(m_playerNum));
		//srand (time(NULL));
		switch (m_handType)
		{
			case HAND_TYPES::Single:
					m_move = gameMove.playSingle(m_prevMove);
				
				break;
			case HAND_TYPES::Pair:
					m_move = gameMove.playPair(m_prevMove);
				break;
			case HAND_TYPES::Triple:
					m_move = gameMove.playTriple(m_prevMove);
				break;
			case HAND_TYPES::Straight:
					m_move = gameMove.playStraight(m_prevMove);
				break;
			case HAND_TYPES::FourKind:
					m_move = gameMove.playFourKind(m_prevMove);
				break;
			case HAND_TYPES::PairStraight:
					m_move = gameMove.playPairStraight(m_prevMove);
				break;
			default:
				m_move = game->getHand(m_playerNum);
				// pass. So we don't need to give move any cards
				break;
		}
	}
}

void GameTreeNode::generateMoveHighest()
{
	//if (DEBUG_MODE)
		//Util::Log("GameTreeNode::generateMoveHighest()");
	m_payoffValue = 0;
	// Remove cards from 'move' in 'hand'
	std::shared_ptr<AIGame> game = m_AIGame.lock();
	if (game)
	{
		gameMove.init(game->getHand(m_playerNum));
		//srand (time(NULL));
		switch (m_handType)
		{
			case HAND_TYPES::Single:
					m_move = gameMove.playHighSingle(m_prevMove);
				
				break;
			case HAND_TYPES::Pair:
					m_move = gameMove.playHighPair(m_prevMove);
				break;
			case HAND_TYPES::Triple:
					m_move = gameMove.playHighTriple(m_prevMove);
				break;
			case HAND_TYPES::Straight:
					m_move = gameMove.playHighStraight(m_prevMove);
				break;
			case HAND_TYPES::FourKind:
					m_move = gameMove.playHighFourKind(m_prevMove);
				break;
			case HAND_TYPES::PairStraight:
					m_move = gameMove.playHighPairStraight(m_prevMove);
				break;
			default:
				m_move = game->getHand(m_playerNum);
				// pass. So we don't need to give move any cards
				break;
		}
	}
	
}

std::vector<Card> GameTreeNode::removeMoveFromHand()
{
	//if (DEBUG_MODE)
		//Util::Log("GameTreeNode::removeMoveFromHand()");
	HandEvaluator m_handEvaluator;

	auto game = m_AIGame.lock();
	std::vector<Card> hand;
	if (game)
	{
		hand = game->getHand(m_playerNum);
		HandEvaluator handEval;
		auto selectedCards = handEval.getSelectedCards(m_move);
		bool erase = handEval.isValidPlay(selectedCards, m_prevMove);

		for (int i = 0; i < hand.size(); i++)
		{
			for (int j = 0; j < selectedCards.size(); j++)
			{
				if ( m_handEvaluator.compareCards(hand[i], selectedCards[j]) )
				{
					if (erase)
					{
						hand.erase(hand.begin() + i);
						if (i > 0) 
							i--;
					}
					else
						hand[i].setSelected(false); // deselect card if it is not a valid move and just pass the turn instead
				}
			}
		}
	}
	return hand;
}



// GenerateChildren(): Are for OUR moves based on all the possible moves of the enemy that preceeds our turn
// 1. Find possible hands to play from the last generated move (by the enemy player)
// 2. Create nodes to play a move based on the last generated move (by the enemy player)
// 3. etc
void GameTreeNode::generateChildren()
{
	//if (DEBUG_MODE)
		//Util::Log("GameTreeNode::generateChildren()");

	HandEvaluator m_handEvaluator;

	//AIGame * Game;
	//m_ChildAIGame = new AIGame;

	// I assume these two here make the bad allocation error
	m_ChildAIGame = make_shared<AIGame>();
	std::shared_ptr<AIGame> game = m_AIGame.lock();
	if (game)
	{
		if (!game->isOver())
		{

			auto selectedCards =  m_handEvaluator.getSelectedCards(m_move);
			for (int i = 0; i < game->getPlayerCount(); i++)
			{
				std::vector<Card> hand;

				// We need to remove the cards played from our hand before saving the game state
				if (i == m_playerNum)
				{
					auto remove = removeMoveFromHand();
					hand = remove.size() > 0 ? remove : game->getHand(i);
				}
				else
				{
					hand = game->getHand(i);
				}
				m_ChildAIGame->addHand(hand);
			}
			m_ChildAIGame->initAddedHands(m_playerNum, selectedCards.size() == 0 ? game->getPassCount() + 1 : game->getPassCount(), game->getOriginalPlayerPos());

			if (!m_ChildAIGame->isOver())
			{
				//for (int i = 0; i < m_ChildAIGame->getPlayerCount(); i++)
				//{
					int nextPlayerNum = m_playerNum+1;
					if (nextPlayerNum >= m_ChildAIGame->getPlayerCount())
						nextPlayerNum = 0;

					// Should be getting the hand of the next player to check what they can play!
					auto hand = m_ChildAIGame->getHand(nextPlayerNum);
					std::vector<HAND_TYPES> possibleHands;

					// If we are still playing/passing
					if (m_ChildAIGame->getPassCount() < m_ChildAIGame->getPlayerCount())
					{
						possibleHands = m_handEvaluator.findPossibleHands(hand, selectedCards.size() == 0 ? m_prevMove : selectedCards);
						possibleHands.push_back(NoHand);
					}
					// If it is our free reign turn
					else
						possibleHands = m_handEvaluator.findPossibleHands(hand);
					

					for (int j = 0; j < possibleHands.size(); j++)
					{
						//auto move = m_handEvaluator.findMoveFrom(possibleHands[j], m_hand, m_move);
						if (selectedCards.size() == 0)
							children.push_back(GameTreeNode(possibleHands[j], m_prevMove, m_ChildAIGame, nextPlayerNum, m_useUct));
						else
							children.push_back(GameTreeNode(possibleHands[j], selectedCards, m_ChildAIGame, nextPlayerNum, m_useUct));
					}
				//}
			} // mChildGame isOver
		} // mGame isOver
	} // game lock
}
void GameTreeNode::UCTSelectActionInt()
{
	if (DEBUG_MODE)
		Util::Log("GameTreeNode::UCTSelectActionInt()");

	std::vector< std::unique_ptr<GameTreeNode> > visited;
	unique_ptr<GameTreeNode> cur (this);
	visited.push_back(std::move(cur));
	int child = visited.back()->UCTSelectSLInt();
	
	while (!visited.back()->isLeaf())
	{
		int i = visited.back()->UCTSelectSLInt();
		if (i >= 0)
		{
			unique_ptr<GameTreeNode> temp (&children[child].getChildren()[i]);
			visited.push_back(std::move(temp));
		}
	}

	

	//visited.back()->UCTExpand();
	

	
	if (children.size() > 0)
	{
		int i = visited.back()->UCTSelectSLInt();
		if (i >= 0)
		{
			unique_ptr<GameTreeNode> newNode (&children[i]);
			visited.push_back(std::move(newNode));
		}
	}

	for (int i = 0; i < visited.size(); i++)
	{
		if (visited[i] != nullptr)
			visited[i]->update();
	}

	/*
	update();

	int i = 0;
	bool leaf = false;
	do
	{
		int j = children[i].UCTSelectSLInt();
		leaf = children[i].isLeaf();
		if (j >= 0)
		{
			i = j;
			children[i].update();
		}
	} while (!leaf);

	children[i].UCTExpand();

	if (children.size() > 0)
	{
		int k = children[i].UCTSelectSLInt();
		if (k >= 0)
		{
			children[k].update();
		}
	}*/
}

std::vector<GameTreeNode>& GameTreeNode::getChildren()
{
	return children;
}


// Returns the best child OF this node
int GameTreeNode::UCTSelectSLInt()
{
	if (DEBUG_MODE)
		Util::Log("GameTreeNode::UCTSelectSLInt()");

	int result = -1;
	double bestUCT = 0;

	if (children.size() == 0)
		return result;

	for (int i = 0; i < children.size()-1; i++)
	{
		double a = children[i].getWins();
		double b = children[i].getVisits();
		double random = rand();
		/*double uctValue = children[i].getWins() / (children[i].getVisits() + m_epsilon) + 
			sqrt(log(children[i].getVisits()+1) / (children[i].getVisits() + m_epsilon)) +
			rand() * DBL_EPSILON;*/
		double uctValue = a / (b + DBL_EPSILON) +
			sqrt(log (b+1) / (b + DBL_EPSILON)) +
			random * DBL_EPSILON;

		if (uctValue > bestUCT)
		{
			result = i;
			bestUCT = uctValue;
		}
	}

	return result;
}

GameTreeNode * GameTreeNode::getBestChild()
{
	if (DEBUG_MODE)
		Util::Log("GameTreeNode::getBestChild()");

	if (children.size() == 0)
		return this;

	GameTreeNode * child = &children[0];
	for (int i = 0; i < children.size(); i++)
	{
		if (children[i].getVisits() > child->getVisits())
		{
			child = &children[i];
		}
	}

	return child;
}

shared_ptr<GameTreeNode> GameTreeNode::getBestChildSmartPtr()
{
	if (DEBUG_MODE)
		Util::Log("GameTreeNode::getBestChildSmartPtr()");

	if (children.size() == 0)
	{
		return shared_ptr<GameTreeNode> (this);
	}

	int j = 0;
	for (int i = 0; i < children.size(); i++)
	{
		if (children[i].getVisits() > children[j].getVisits())
		{
			j = i;
		}
	}

	return shared_ptr<GameTreeNode> (&children[j]);
}
#pragma endregion OldFunctions