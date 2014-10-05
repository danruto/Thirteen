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
// Filename: GameTreeNode.h
// Description: Represents a node within the game tree and the generation of this node
// Author:		Danny Sok
// Date Created: 9/08/2013
// Date Last Modified: 27/09/2013
#ifndef GAME_TREE_NODE_H
#define GAME_TREE_NODE_H

#include "..\game\Globals.h"
#include "..\game\Card.h"
#include "GameMove.h"
#include "HandEvaluator.h"
#include "AIGame.h"

class GameTreeNode
{
public:
	~GameTreeNode();

	// --------- Testing ---------- //
	// Todo Functions/New not yet implemented
	GameTreeNode(std::vector<GameTreeNode> children, std::vector< std::shared_ptr<AIGame> > AIGameList, int playerNum, bool useUct, bool isParent = true);
	GameTreeNode(std::vector<Card> prevMove, std::shared_ptr<AIGame> aiGame, int playerNum, bool useUct);
	void generatePossibleMoves();
	void generateChildren(std::vector< std::vector<Card> > moves);
	void generateMoves(std::vector< std::vector<Card> > moves);
	std::vector< std::shared_ptr<AIGame> > m_AIGameList;
	AIGame generateGame(std::vector<Card> move);
	std::vector<Card> removeMoveFromHand(std::vector<Card> move);
	std::vector< std::vector<Card> > m_moves;
	int getBestChildIndex();
	std::vector<Card> getMoveAt(int index);
	bool m_isParent;
	std::vector<int> getBestChildInfo();
	void printChildVisits();
	// --------- Testing ---------- //

	// UCT Functions
	GameTreeNode(HAND_TYPES handType, std::vector<Card> move, std::shared_ptr<AIGame> aiGame, int playerNum, bool useUct);
	
	void UCTExpand();
	int getVisits();
	int getWinRate();
	void update();
	bool isLeaf();
	void UCTSelectAction();
	void UCTSelectActionInt();
	GameTreeNode * UCTSelectSL();
	int UCTSelectSLInt();
	GameTreeNode * getBestChild();
	shared_ptr<GameTreeNode> getBestChildSmartPtr();
	int getBestChildVisits();
	bool isWinningNode();
	bool isFreeReignNode();
	int getWins();
	std::vector<Card> getMove();

	void destroy();

	std::vector<GameTreeNode>& getChildren();

	//--------------
	// Old Functions
	//--------------
	//GameTreeNode(Game * gameState, HAND_TYPES handType, std::vector<Card> move, std::vector<Card> hand);
	//GameTreeNode(Game * gameState, HAND_TYPES handType, std::vector<Card> move, std::vector<Card> hand, GameTreeNode * parent);
	//GameTreeNode(HAND_TYPES handType, std::vector<Card> move, GameTreeNode * parent, AIGame & aiGame, int playerNum);

	int findBestPayoff();
	int getPayoffValue();
	
	void insert();
	HAND_TYPES getHandType();
	PLAYER_ACTIONS getStrategy();
	int UCTSelect(int nodeIndex);
	GameMove UCTSearch(int numSim);
	int UCTPlaySimulation(GameTreeNode n);
	GameTreeNode * getNextChild();
	void UCTPlaySimulation();
private:
	//HandEvaluator m_handEvaluator;
	std::vector<Card> m_move, m_prevMove;
	weak_ptr<AIGame> m_AIGame;
	shared_ptr<AIGame> m_ChildAIGame;
	int m_playerNum;
	bool m_useUct;

	std::vector<GameTreeNode> children;
	GameMove gameMove;

	void generateMove();
	void generateMoveLowest();
	void generateMoveHighest();
	void generateChildren();
	
	std::vector<Card> removeMoveFromHand();

	double UCTK;
	int m_childIndex; // Child Index From Parent
	int m_nextChildIndex; // Element keeper for returning next child
	int m_visits;
	
	int m_wins;

	//-----------
	// Old Functions
	//------------
	int m_winRate;
	void generatePayoff();
	void updatePayoffs(int payoff);
	HAND_TYPES m_handType;
	PLAYER_ACTIONS m_strat;
	int m_payoffValue;
	//GameTreeNode * m_parent;
};

#endif