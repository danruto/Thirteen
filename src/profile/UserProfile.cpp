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
// Filename: UserProfile.cpp
// Description: Contains the attributes and interfaces for a user profile
// Author: Danny Sok
// Date Created: 17/08/2013
// Date Last Modified: 17/08/2013

#include "UserProfile.h"
#include "../util/Util.h"

UserProfile::UserProfile()
{
	Init();
}

UserProfile::~UserProfile()
{
	close();
}

void UserProfile::Init()
{
	m_fileName = "Profiles.db";
	open();

	// DEBUG:
	// TODO:
	if (getProfileCount() == 0)
		insertProfile("Mate", AI_EASY, 3);
}

void UserProfile::save(int profileId, std::string name, int ai_choice, int nb_ai_players)
{
	// UPDATE Profile SET name = '', ai_choice = '', nb_ai_players = '' WHERE profile_id = '';
	stringstream ss;
	ss << "UPDATE Profile SET name = '" << name << "', ai_choice = '" << ai_choice << "', nb_ai_players = '" << nb_ai_players << "' WHERE profile_id = '" << profileId << "';";
	char *query = Util::sToC(ss.str());
	m_db->query(query);
	
	delete [] query;
}

void UserProfile::open()
{
	char* fn = Util::sToC(m_fileName);
	m_db = new Database(fn);
	
	// Check to see if DB Exists. If it does, then we are already connected. If not, create it.
	delete [] fn;

	m_db->query("CREATE TABLE IF NOT EXISTS Profile ( profile_id INTEGER PRIMARY KEY NOT NULL, name TEXT, ai_choice INTEGER, nb_ai_players INTEGER, player_wins INTEGER, player_losses INTEGER );");
}

void UserProfile::close()
{
	m_db->close();
}

int UserProfile::getProfileCount()
{
	vector<vector<string> > result = m_db->query("SELECT COUNT(profile_id) FROM Profile;");
	//ss
	
	return atoi(result[0].at(0).c_str());
}

std::string UserProfile::getName(int profileId)
{
	stringstream ss;
	ss << "SELECT name FROM Profile WHERE profile_id = '" << profileId << "';";
	char *query = Util::sToC(ss.str());
	vector< vector<string> > result = m_db->query(query);

	delete [] query;

	return result[0].at(0);
}

void UserProfile::setName(int profileId, std::string name)
{
	stringstream ss;
	ss << "UPDATE Profile SET name = '" << name << "' WHERE profile_id = '" << profileId << "';";
	char *query = Util::sToC(ss.str());
	m_db->query(query);
	
	delete [] query;
}

PLAYER_TYPES UserProfile::getAIChoice(int profileId)
{
	stringstream ss;
	ss << "SELECT ai_choice FROM Profile WHERE profile_id = '" << profileId << "';";
	char *query = Util::sToC(ss.str());

	PLAYER_TYPES p = PLAYER_TYPES::NONE;
	vector< vector<string> > result = m_db->query(query);
	if (result.size() != 0)
		p = (PLAYER_TYPES)atoi(result[0].at(0).c_str());

	result.clear();	result.shrink_to_fit();

	delete [] query;

	return p;
}

std::string UserProfile::getAIChoiceStr(int profileId)
{
	std::string AIChoice = "";
	switch(getAIChoice(profileId))
	{
		case AI_EASY:
			AIChoice = "Easy";
			break;
		case AI_MEDIUM:
			AIChoice = "Medium";
			break;
		case AI_HARD:
			AIChoice = "Hard";
			break;
		case NONE:
		default:
			break;
	}
	return AIChoice;
}

void UserProfile::setAIChoice(int profileId, int AIChoice)
{
	stringstream ss;
	ss << "UPDATE Profile SET ai_choice = '" << AIChoice << "' WHERE profile_id = '" << profileId << "';";
	char *query = Util::sToC(ss.str());
	m_db->query(query);
	
	delete [] query;
}

int UserProfile::getNbAIPlayers(int profileId)
{
	stringstream ss;
	ss << "SELECT nb_ai_players FROM Profile WHERE profile_id = '" << profileId << "';";
	char *query = Util::sToC(ss.str());

	int p = 0;
	
	vector< vector<string> > result = m_db->query(query);
	if (result.size() != 0)
		p = atoi(result[0].at(0).c_str());

	delete [] query;

	return p;
}

std::string UserProfile::getNbAIPlayersStr(int profileId)
{
	std::string nbPlayers = "";
	switch (getNbAIPlayers(profileId))
	{
	case 1:
		nbPlayers = "1";
		break;
	case 2:
		nbPlayers = "2";
		break;
	case 3:
		nbPlayers = "3";
		break;
	default:
		break;
	}
	return nbPlayers;
}

void UserProfile::setNbAIPlayers(int profileId, int NbAIPlayers)
{
	stringstream ss;
	ss << "UPDATE Profile SET nb_ai_players = '" << NbAIPlayers << "' WHERE profile_id = '" << profileId << "';";
	char *query = Util::sToC(ss.str());
	m_db->query(query);
	
	delete [] query;
}

int UserProfile::getWinCount(int profileId)
{
	stringstream ss;
	ss << "SELECT player_wins FROM Profile WHERE profile_id = '" << profileId << "';";
	char *query = Util::sToC(ss.str());

	int p = 0;
	
	vector< vector<string> > result = m_db->query(query);
	if (result.size() != 0)
		p = atoi(result[0].at(0).c_str());

	delete [] query;

	return p;
}

void UserProfile::setWinCount(int profileId)
{
	int winCount = getWinCount(profileId) + 1;
	stringstream ss;
	ss << "UPDATE Profile SET player_wins = '" << winCount << "' WHERE profile_id = '" << profileId << "';";
	char *query = Util::sToC(ss.str());
	m_db->query(query);
	
	delete [] query;
}

int UserProfile::getLossCount(int profileId)
{
	stringstream ss;
	ss << "SELECT player_losses FROM Profile WHERE profile_id = '" << profileId << "';";
	char *query = Util::sToC(ss.str());

	int p = 0;
	
	vector< vector<string> > result = m_db->query(query);
	if (result.size() != 0)
		p = atoi(result[0].at(0).c_str());

	delete [] query;

	return p;
}

void UserProfile::setLossCount(int profileId)
{
	int lossCount = getLossCount(profileId) + 1;
	stringstream ss;
	ss << "UPDATE Profile SET player_losses = '" << lossCount << "' WHERE profile_id = '" << profileId << "';";
	char *query = Util::sToC(ss.str());
	m_db->query(query);
	
	delete [] query;
}

void UserProfile::insertProfile(std::string name, int ai_choice, int nb_ai_players)
{
	// INSERT INTO Profile (name, ai_choice, nb_ai_players) VALUES ('', '', '');
	stringstream ss;
	ss << "INSERT INTO Profile (name, ai_choice, nb_ai_players, player_wins, player_losses) VALUES ('" << name << "', '" << ai_choice << "', '" << nb_ai_players << "', '0', '0" << "');";
	char *query = Util::sToC(ss.str());
	m_db->query(query);
	
	delete [] query;
}