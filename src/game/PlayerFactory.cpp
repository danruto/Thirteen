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

#include "PlayerFactory.h"
#include "HumanFactory.h"
#include "AIFactory.h"

PlayerFactory* PlayerFactory::CreateFactory(PLAYER_TYPES factory)
{
    if(factory == HUMAN)
    {
        return new HumanFactory();
    }
    else if(factory == AI_EASY)
    {
        return new EasyAIFactory();
    }
	else if (factory == AI_MEDIUM)
	{
		return new MediumAIFactory();
	}
    else if(factory == AI_HARD)
    {
        return new AdaptiveAIFactory();
    }
}