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
// Filename: Util.h
// Description: Contains utility functions
// Author: Danny Sok
// Date Created: 10/04/2013
// Date Last Modified: 10/04/2013

#pragma once

#ifndef UTIL_H
#define UTIL_H

#include "ofMain.h"
#define DEBUG_MODE 1

namespace Util
{
//class Util {
//public:
	void Log(std::string msg);
	void ResultsLog(std::string msg);
	void WriteLog();
	void NewLog();
	char* sToC(string s);
	std::string intToStr(int number);
	std::vector<std::string> &split(const std::string &s, char delim, std::vector<std::string> &elems);
	std::vector<std::string> split(const std::string &s, char delim);
	double FindTenPercent(double n);
	int charToCardValue(char c);
	int charToCardSuite(char c);

//};
}
#endif /* UTIL_H */