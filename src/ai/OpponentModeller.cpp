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
// Filename:	OpponentModeller.cpp
// Description: A database interface to model the opponent
// Author:		Danny Sok
// Date Created: 26/06/2013
// Date Last Modified: 27/09/2013

#include "OpponentModeller.h"
#include "..\util\Util.h"
#include <iostream>
#include <string>
#include <sstream>
#include <algorithm>
#include <iterator>

void OpponentModeller::create(string filename)
{
	char* fn = Util::sToC(filename);
	_db = new Database(fn);
	
	// Check to see if DB Exists. If it does, then we are already connected. If not, create it.
	//if (!_db->open(fn))
		//_db->query("CREATE TABLE PlayerModel ( id INTEGER PRIMARY KEY NOT NULL, hand text, player_type INTEGER );");
	delete [] fn;

	// Session 1 example:
	// Create a session; Reference that session in creating the players; 
	// Insert into Session the session number
	// Insert into Player the players in the session
	// Insert Values into Hand with player number, hand
	// At the end of the game, alter the players to add in turnCount(turns_participated), play_percentage(playCount), passPercentage(turnCount-playCount), win_percentage(winCount / turnCount * 100), loss_percentage (100-win_percentage);
	// CHANGE: Move Win/Loss count to Session

	// CREATE TABLE Player ( player_id INTEGER PRIMARY KEY NOT NULL, session_id INTEGER, player_type INTEGER, turns_participated INTEGER, play_percentage DOUBLE, pass_percentage DOUBLE, FOREIGN KEY(session_id) REFERENCES Session );
	_db->query("CREATE TABLE IF NOT EXISTS Player ( player_id INTEGER PRIMARY KEY NOT NULL, session_id INTEGER, player_type INTEGER, turns_participated INTEGER, play_percentage DOUBLE, pass_percentage DOUBLE, FOREIGN KEY(session_id) REFERENCES Session );");
	// CREATE TABLE Session ( session_id INTEGER PRIMARY KEY NOT NULL, num_players INTEGER, session_winner INTEGER );
	_db->query("CREATE TABLE IF NOT EXISTS Session ( session_id INTEGER PRIMARY KEY NOT NULL, num_players INTEGER, session_winner INTEGER );");
	// CREATE TABLE Hand ( hand_id INTEGER PRIMARY KEY NOT NULL, player_id INTEGER FOREIGN KEY REFERENCES Player (player_id), hand TEXT, hand_type INTEGER );
	_db->query("CREATE TABLE IF NOT EXISTS Hand ( hand_id INTEGER PRIMARY KEY NOT NULL, player_id INTEGER, hand TEXT, hand_type INTEGER, is_winning_hand INTEGER, FOREIGN KEY(player_id) REFERENCES Player);");
	// CREATE TABLE ActionUtilities ( action_id INTEGER PRIMARY KEY NOT NULL, action_a_util DOUBLE, action_b_util DOUBLE, action_c_util DOUBLE, action_d_util DOUBLE, action_e_util DOUBLE, action_f_util DOUBLE, action_g_util DOUBLE, action_h_util DOUBLE, action_i_util DOUBLE );
	_db->query("CREATE TABLE IF NOT EXISTS ActionUtilities ( action_id INTEGER PRIMARY KEY NOT NULL, action_a_util DOUBLE, action_b_util DOUBLE, action_c_util DOUBLE, action_d_util DOUBLE, action_e_util DOUBLE, action_f_util DOUBLE, action_g_util DOUBLE, action_h_util DOUBLE, action_i_util DOUBLE );");
	createUtilities();
}

void OpponentModeller::connect(string filename)
{
	char* fn = Util::sToC(filename);
	_db = new Database(fn);
	
	// Check to see if DB Exists. If it does, then we are already connected. If not, create it.
	//if (!_db->open(fn))
		//_db->query("CREATE TABLE PlayerModel ( id INTEGER PRIMARY KEY NOT NULL, hand text, player_type INTEGER );");
	delete [] fn;
}

bool OpponentModeller::open(string filename)
{
	char* fn = Util::sToC(filename);
	bool b = (_db->open(fn));
	delete [] fn;
	return b;
}

void OpponentModeller::insertSession(int playerCount)
{
	string i = Util::intToStr(playerCount);
	string s = "INSERT INTO Session (num_players) VALUES ('" + i + "');";
	char *query = Util::sToC(s);
	_db->query(query);

	delete [] query;
}

void OpponentModeller::updateSession(int winner)
{
	string i = Util::intToStr(winner);

	string s = "UPDATE Session SET session_winner = '" + i + "' WHERE session_id = '" + getCurrentSession() + "';";
	char *query = Util::sToC(s);
	_db->query(query);

	delete [] query;

	updateHand();
}

void OpponentModeller::insertPlayer(int playerId, int playerType, int turns, int playCount)
{
	string i = Util::intToStr(playerType);
	string s;
	if (turns == 0 || playCount == 0)
		s = "INSERT INTO Player (player_type, turns_participated, play_percentage, session_id) VALUES ('" + i + "', '0', '0', " + getCurrentSession() + " );";
	else
		s = "INSERT INTO Player (player_type, turns_participated, play_percentage, session_id) VALUES ('" + i + "', '" + Util::intToStr(turns) + "', '" + Util::intToStr(playCount/turns) + "', " + getCurrentSession() + " );";
	char *query = Util::sToC(s);
	_db->query(query);

	delete [] query;
}

void OpponentModeller::updatePlayer(int playerId, int turns, int playCount)
{
	int p = turns > 0 ? (playCount*100) / turns : 0;
	int p2 = 100 - p;
	string s = "UPDATE Player SET turns_participated = '" + Util::intToStr(turns) + "', play_percentage = '" + ofToString(p) + "', pass_percentage = '" + ofToString(p2) + "' WHERE player_id = '" + getPlayerId(playerId) + "';";
	char *query = Util::sToC(s);
	_db->query(query);

	delete [] query;
}

void OpponentModeller::insertHand(vector<Card> played, HAND_TYPES ht, int playerId, bool isWinningHand)
{
	string tCard, sType, sWinningHand;
	int iType = ht;
	sType = Util::intToStr(iType);
	sWinningHand = isWinningHand ? ofToString(isWinningHand) : ofToString(isWinningHand);

	for (int i = 0; i < played.size(); i++)
	{
		if (i == played.size()-1)
			tCard += played[i].toString();
		else
			tCard += played[i].toString() + " ";
	}

	string pId = getPlayerId(playerId);
	string s = "INSERT INTO Hand (player_id, hand, hand_type, is_winning_hand) VALUES ('" +  pId + "', '" + tCard + "', '" + sType + "', '" + sWinningHand + "');";
	char *query = Util::sToC(s);
	_db->query(query);
	
	delete [] query;

	//HACK:
	//debug, just output all the cards played for the session
	//cout << tCard << endl;
}

void OpponentModeller::updateHand()
{
	string sWinningHand = ofToString(true);
	string s = "UPDATE Hand SET is_winning_hand = '" + sWinningHand + "' WHERE hand_id = 'MAX(hand_id)';";
	char *query = Util::sToC(s);
	_db->query(query);

	delete [] query;
}

void OpponentModeller::createUtilities()
{
	if (!utilitiesExist())
	{
		string s = "INSERT INTO ActionUtilities (action_a_util, action_b_util, action_c_util, action_d_util, action_e_util, action_f_util, action_g_util, action_h_util, action_i_util) VALUES ('0', '0', '0', '0', '0', '0', '0', '0', '0');";
		char *query = Util::sToC(s);
		_db->query(query);

		delete [] query;
	}
}

bool OpponentModeller::utilitiesExist()
{
	vector<string> r;
	string s = ("SELECT COUNT(action_a_util) FROM ActionUtilities;");
	char *query = Util::sToC(s);
	vector< vector<string> > result = _db->query(query);
	for(vector< vector<string> >::iterator it = result.begin(); it < result.end(); ++it)
	{
		vector<string> row = *it;
		r.push_back( row.at(0) );
	}

	string count = r[0];
	r.clear(); r.shrink_to_fit();
	result.clear();	result.shrink_to_fit();

	return atoi(count.c_str()) > 0;
}

// Will grab all the hands played from the database
// then split the hand into cards per hand for analysis
// e.g. from hands[0] = { "3S 4D 5H" }, hands[1] = { "10S 10C 10H" }
// to cardsInHand[0] = { "3S", "4D", "5H" }
vector< vector<string> > OpponentModeller::getResult()
{
	vector<string> hands;

	vector< vector<string> > result = _db->query("SELECT hand, player_type FROM PlayerModel;");
	for(vector< vector<string> >::iterator it = result.begin(); it < result.end(); ++it)
	{
		vector<string> row = *it;
		hands.push_back( row.at(0) );
	}

	vector< vector<string> > cardsInHand;
	for (int i = 0; i < hands.size(); i++)
	{
		vector<string> tokens;
		istringstream iss(hands[i]);
		copy(istream_iterator<string>(iss),
			 istream_iterator<string>(),
			 back_inserter<vector<string>>(tokens));
		cardsInHand.push_back(tokens);
	}
	return cardsInHand;
}

string OpponentModeller::getCurrentSession()
{
	vector<string> r;
	vector< vector<string> > result = _db->query("SELECT MAX(session_id) FROM Session WHERE session_id IS NOT NULL;");
	for(vector<vector<string> >::iterator it = result.begin(); it < result.end(); ++it)
	{
		vector<string> row = *it;
		r.push_back( row.at(0) );
	}

	string id = r[0];
	r.clear(); r.shrink_to_fit();
	result.clear();	result.shrink_to_fit();

	return id;
}

string OpponentModeller::getPlayerId(int pId)
{
	vector<string> r;
	string s = ("SELECT player_id FROM Player WHERE session_id = '" + getCurrentSession() + "';");
	char *query = Util::sToC(s);
	vector< vector<string> > result = _db->query(query);
	for(vector< vector<string> >::iterator it = result.begin(); it < result.end(); ++it)
	{
		vector<string> row = *it;
		r.push_back( row.at(0) );
	}

	string id = r[pId];
	r.clear(); r.shrink_to_fit();
	result.clear();	result.shrink_to_fit();

	return id;
}

void OpponentModeller::setUtility(PLAYER_ACTIONS strat, double value)
{
	double start = ofGetElapsedTimef();
	stringstream ss;
	ss << "UPDATE ActionUtilities SET "; 
	
	
	switch (strat)
	{
		case ACTION_A:
			ss << "action_a_util = '" << value << "';";
			break;
		case ACTION_B:
			ss << "action_b_util = '" << value << "';";
			break;
		case ACTION_C:
			ss << "action_c_util = '" << value << "';";
			break;
		case ACTION_D:
			ss << "action_d_util = '" << value << "';";
			break;
		case ACTION_E:
			ss << "action_e_util = '" << value << "';";
			break;
		case ACTION_F:
			ss << "action_f_util = '" << value << "';";
			break;
		case ACTION_G:
			ss << "action_g_util = '" << value << "';";
			break;
		case ACTION_H:
			ss << "action_h_util = '" << value << "';";
			break;
		case ACTION_I:
			ss << "action_i_util = '" << value << "';";
			break;
		default:
			break;
	}
	char *query = Util::sToC(ss.str());
	_db->query(query);
	
	delete [] query;

	if (DEBUG_MODE)
		Util::Log("OpponentModeller::setUtility() took: " + ofToString(ofGetElapsedTimef() - start));
}

double OpponentModeller::getUtility(PLAYER_ACTIONS strat)
{
	stringstream ss;

	switch (strat)
	{
		case ACTION_A:
			ss << "SELECT action_a_util FROM ActionUtilities;";
			break;
		case ACTION_B:
			ss << "SELECT action_b_util FROM ActionUtilities;";
			break;
		case ACTION_C:
			ss << "SELECT action_c_util FROM ActionUtilities;";
			break;
		case ACTION_D:
			ss << "SELECT action_d_util FROM ActionUtilities;";
			break;
		case ACTION_E:
			ss << "SELECT action_e_util FROM ActionUtilities;";
			break;
		case ACTION_F:
			ss << "SELECT action_f_util FROM ActionUtilities;";
			break;
		case ACTION_G:
			ss << "SELECT action_g_util FROM ActionUtilities;";
			break;
		case ACTION_H:
			ss << "SELECT action_h_util FROM ActionUtilities;";
			break;
		case ACTION_I:
			ss << "SELECT action_i_util FROM ActionUtilities;";
			break;
		default:
			break;
	}

	char *query = Util::sToC(ss.str());
	vector< vector<string> > result = _db->query(query);
	
	delete [] query;

	vector<string> r;
	for(vector<vector<string> >::iterator it = result.begin(); it < result.end(); ++it)
	{
		vector<string> row = *it;
		r.push_back( row.at(0) );
	}

	//debug();

	if (r.size() > 0)
		return atof(r[0].c_str());
	else 
		return 0;
}

double OpponentModeller::getWinRateOf(PLAYER_ACTIONS strat)
{
	return getTotalWinsFor(strat) / getTotalHands();
}

int OpponentModeller::getTotalWinsFor(PLAYER_ACTIONS strat)
{
	int wins = 0;

	if (strat == PLAYER_ACTIONS::ACTION_A)
		return 0;

	HAND_TYPES ht = NoHand;

	vector<string> r;
	vector< vector<string> > result = _db->query("SELECT hand_type FROM Hand WHERE is_winning_hand = '1';");
	for(vector<vector<string> >::iterator it = result.begin(); it < result.end(); ++it)
	{
		vector<string> row = *it;
		r.push_back( row.at(0) );
	}
	r.clear(); r.shrink_to_fit();
	result.clear();	result.shrink_to_fit();

	for (int i = 0; i < r.size(); i++)
	{
		ht = (HAND_TYPES)ofToInt(r[i]);
		if (handTypeToAction(ht) == strat)
			wins++;
	}

	return wins;
}

int OpponentModeller::getTotalHands()
{
	string id = "0";
	vector<string> r;
	vector< vector<string> > result = _db->query("SELECT COUNT(hand_id) FROM Hand WHERE hand_id IS NOT NULL;");
	for(vector<vector<string> >::iterator it = result.begin(); it < result.end(); ++it)
	{
		vector<string> row = *it;
		r.push_back( row.at(0) );
	}

	id = r[0];
	r.clear(); r.shrink_to_fit();
	result.clear();	result.shrink_to_fit();

	return ofToInt(id);
}

bool OpponentModeller::isWinningHand(PLAYER_ACTIONS strat)
{
	bool b = false;
	HAND_TYPES ht = NoHand;

	string id = "-1";
	vector<string> r;
	vector< vector<string> > result = _db->query("SELECT hand_type FROM Hand WHERE is_winning_hand = '1';");
	for(vector<vector<string> >::iterator it = result.begin(); it < result.end(); ++it)
	{
		vector<string> row = *it;
		r.push_back( row.at(0) );
	}

	if (r.size() > 0)
		id = r[0];
	r.clear(); r.shrink_to_fit();
	result.clear();	result.shrink_to_fit();

	if (ofToInt(id) != -1)
	{
		ht = (HAND_TYPES)ofToInt(id);
		return (handTypeToAction(ht) == strat);
	}

	return ht;
}



PLAYER_ACTIONS OpponentModeller::handTypeToAction(HAND_TYPES ht)
{
	PLAYER_ACTIONS a = ACTION_A;

	switch (ht)
	{
		case Single:
			a = ACTION_C;
			break;
		case Pair:
			a = ACTION_C;
			break;
		case Triple:
			a = ACTION_C;
			break;
		case FourKind:
			a = ACTION_C;
			break;
		case Straight:
			a = ACTION_C;
			break;
		case PairStraight:
			a = ACTION_C;
			break;
		case NoHand:
		default:
			a = ACTION_A;
			break;
	}

	return a;
}

vector< vector<string> > OpponentModeller::getResult(string s)
{
	char *query = Util::sToC(s);
	vector< vector<string> > result = _db->query(query);
	delete [] query;

	return result;
}

void OpponentModeller::close()
{
	_db->close();
}

void OpponentModeller::debug()
{
	string id = "-1";
	vector<string> r;
	vector< vector<string> > result = _db->query("SELECT * FROM ActionUtilities;");
	for(vector<vector<string> >::iterator it = result.begin(); it < result.end(); ++it)
	{
		vector<string> row = *it;
		r.push_back( row.at(0) );
	}

	if (r.size() > 0)
		id = r[0];

	cout << id << endl;
	r.clear(); r.shrink_to_fit();
	result.clear();	result.shrink_to_fit();
}

std::vector<Card> OpponentModeller::getCardsPlayedInLatestSession()
{
	std::vector<Card> hand;

	string sessionId = getCurrentSession();
	stringstream ss;
	ss << "SELECT hand FROM Hand WHERE player_id in (SELECT player_id FROM Player WHERE session_id = '" << sessionId << "');" << endl;

	char *query = Util::sToC(ss.str());
	vector< vector<string> > result = _db->query(query);

	vector<string> r;
	delete query;

	// Get all the Hand strings
	for(vector<vector<string> >::iterator it = result.begin(); it < result.end(); ++it)
	{
		vector<string> row = *it;

		vector<string> v = Util::split(row.at(0), ' ');
		
		r.insert(r.end(), v.begin(), v.end());
	}

	// Create hand from the hand strings
	for (vector<string>::iterator it = r.begin(); it < r.end(); ++it)
	{
		Card c = Card(*it);
		c.setNeededByAI(true);
		c.setup();
		hand.push_back(c);
	}

	r.clear(); r.shrink_to_fit();
	result.clear();	result.shrink_to_fit();

	return hand;
}