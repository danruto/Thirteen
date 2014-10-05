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
// Filename: GameTree.h
// Description: Represents a the game tree. Houses many nodes and potential possibilities for said nodes
// Author:		Danny Sok
// Date Created: 9/08/2013
// Date Last Modified: 27/09/2013
#ifndef GAME_TREE_H
#define GAME_TREE_H

#include "GameTreeNode.h"
#include "..\game\Game.h"
#include "HandEvaluator.h"
#include "AIGame.h"


class GameTree
{
public: 
	~GameTree();
	void destroy();

	void init(int playerNum, bool uct, bool godMode);
	std::vector<Card> getUCTBestMove3();

#pragma region Unused/Old functions
	void start();
	void UCTPlaySimulation();
	void findBestChildSimulate(int num);
	PLAYER_ACTIONS findBestMove();
	// UCT Functions
	PLAYER_ACTIONS getUCTBestMove();
	PLAYER_ACTIONS getUCTBestMove2();
	GameTreeNode getChildrenAt(int index);
	std::vector<GameTreeNode> getChildren();
#pragma endregion

private:
	void saveGameState();
	void generateChildren(bool uct);
	void findBestChildVisits();
	AIGame generateGameBasedOnCurrentState();

	bool m_uct;
	int m_childIndex;
	int m_playerNum;
	int m_numOpponentCardGuesses;
	int m_childMax;

	std::vector<GameTreeNode> children;	
	std::vector< std::shared_ptr<AIGame> > m_AIGameList;

	// Testing functions for smarter playing. It'll try to play all possible cards instead of extremes
	void generateChildrenSmart(std::vector< std::vector<Card> > moves, bool uct);
	void generatePossibleMoves(bool uct);
	void saveRootNode();
	unique_ptr<GameTreeNode> parent;
	shared_ptr<GameTreeNode> bestChild;
	std::vector< unique_ptr<GameTreeNode> > parents;
	int m_bestParent;

#pragma region Unused/Old functions
	int m_simCount;
	std::shared_ptr<AIGame> m_AIGame;
	HandEvaluator m_handEvaluator;
	void findBestChild();
	void findBestChildSmartPtr();
#pragma endregion
};

#endif