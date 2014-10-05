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
// Filename:	AIFactory.h
// Description: Factory classes for AI types to make the implementation abstract
// Author:		Danny Sok
// Date Created: 25/06/2013
// Date Last Modified: 25/06/2013

#pragma once

#ifndef AIFACTORY_H
#define AIFACTORY_H

#include "..\ai\AdaptiveAI.h"
#include "..\ai\EasyAI.h"
#include "..\ai\MediumAI.h"

class AdaptiveAIFactory : public PlayerFactory
{
public:
	IPlayer* getPlayer()
	{
		return new AdaptiveAI();
	}
};

class EasyAIFactory : public PlayerFactory
{
public:
	
	IPlayer* getPlayer()
	{
		//EasyAI * a = new EasyAI;
		return new EasyAI();
	}
};

class MediumAIFactory : public PlayerFactory
{
public:
	
	IPlayer* getPlayer()
	{
		return new MediumAI();
	}

};

#endif AIFACTORY_H