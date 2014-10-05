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
// Filename: UserProfile.h
// Description: Contains the attributes and interfaces for a user profile
// Author: Danny Sok
// Date Created: 17/08/2013
// Date Last Modified: 17/08/2013

#pragma once

#ifndef USER_PROFILE_H
#define USER_PROFILE_H

#include "../game/Globals.h"
#include "../lib/Database.h"

class UserProfile
{
public:
	UserProfile();
	~UserProfile();

	void Init();
	void save(int profileId, std::string name, int ai_choice, int nb_ai_players);
	int getProfileCount();

	std::string getName(int profileId);
	void setName(int profileId, std::string name);
	PLAYER_TYPES getAIChoice(int profileId);
	std::string getAIChoiceStr(int profileId);
	void setAIChoice(int profileId, int AIChoice);
	int getNbAIPlayers(int profileId);
	std::string getNbAIPlayersStr(int profileId);
	void setNbAIPlayers(int profileId, int nbAIPlayers);
	int getWinCount(int profileId);
	void setWinCount(int profileId);
	int getLossCount(int profileId);
	void setLossCount(int profileId);

	void insertProfile(std::string name, int ai_choice, int nb_ai_players);

private:
	void open();
	void close();

	Database *m_db;
	std::string m_fileName;
};

#endif