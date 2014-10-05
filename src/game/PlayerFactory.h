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
// Filename:	PlayerFactory.h
// Description: A factory model of players
// Author:		Danny Sok
// Date Created: 25/06/2013
// Date Last Modified: 25/06/2013

#pragma once

#ifndef PLAYERFACTORY_H
#define PLAYERFACTORY_H

#include "IPlayer.h"

class PlayerFactory
{
public:
	virtual IPlayer* getPlayer() = 0;
	//IPlayer* getPlayer() { return NULL; }
	
	static PlayerFactory* CreateFactory(PLAYER_TYPES factory);
};

#endif PLAYERFACTORY_H