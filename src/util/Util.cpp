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
// Filename: Util.cpp
// Description: Contains utility functions
// Author: Danny Sok
// Date Created: 10/04/2013
// Date Last Modified: 27/09/2013

#include "Util.h"
#include "..\game\Globals.h"

namespace Util
{
	ofBuffer dataBuf;
	std::string logName = ofGetTimestampString() + ".txt";
	std::string resultsLogName =  ofGetTimestampString() + "Results.txt";
	void Log(string msg)
	{
		stringstream ss;
		if (DEBUG_MODE)
		{
			ss << msg << ": " << ofGetElapsedTimef() << endl;
		
			//cout << msg << ": " << ofGetElapsedTimef() << endl;
			
			ofLog(OF_LOG_VERBOSE, ss.str());
			cout << ss.str();
		}
		try
		{
			//dataBuf.set(dataBuf.getText() + ss.str());
			//ofBufferToFile(logName, dataBuf);
			std::ofstream output;
			output.open(logName, std::ios_base::app);
			output << ss.str();
		} catch (exception e) {}
	}

	void ResultsLog(string msg)
	{
		stringstream ss;
		ss << msg << endl;
		try
		{
			//dataBuf.set(dataBuf.getText() + ss.str());
			//ofBufferToFile(logName, dataBuf);
			std::ofstream output;
			output.open(resultsLogName, std::ios_base::app);
			output << ss.str();
		} catch (exception e) {}
	}

	void WriteLog()
	{
		//dataBuf.set(test);
		//ofBufferToFile(logName, dataBuf);
	}

	void NewLog()
	{
		logName = ofGetTimestampString() + ".txt";
		resultsLogName =  ofGetTimestampString() + "Results.txt";
	}

	char* sToC(string s)
	{
		char *query = new char[s.length() + 1];
		strcpy(query, s.c_str());
		return query;
	}

	string intToStr(int number)
	{
		stringstream ss;//create a stringstream
		ss << number;//add number to the stream
		return ss.str();//return a string with the contents of the stream
	}

	std::vector<std::string> &split(const std::string &s, char delim, std::vector<std::string> &elems) 
	{
		std::stringstream ss(s);
		std::string item;
		while (std::getline(ss, item, delim)) {
			elems.push_back(item);
		}
		return elems;
	}


	std::vector<std::string> split(const std::string &s, char delim) 
	{
		std::vector<std::string> elems;
		split(s, delim, elems);
		return elems;
	}

	double FindTenPercent(double n)
	{
		return (n * 0.1);
	}

	int charToCardValue(char c)
	{
		int i = -1;
		switch (c)
		{
			case 'A':
				i = 1;
				break;
			case 'K':
				i = 13;
				break;
			case 'Q':
				i = 12;
				break;
			case 'J':
				i = 11;
				break;
			case '2':
				i = 2;
				break;
			case '3':
				i = 3;
				break;
			case '4':
				i = 4;
				break;
			case '5':
				i = 5;
				break;
			case '6':
				i = 6;
				break;
			case '7':
				i = 7;
				break;
			case '8':
				i = 8;
				break;
			case '9':
				i = 9;
				break;
			default:
				break;
		}
		return i;
	}

	int charToCardSuite(char c)
	{
		int i = -1;
		switch (c)
		{
			case 'S':
				i = 0;
				break;
			case 'C':
				i = 1;
				break;
			case 'D':
				i = 2;
				break;
			case 'H':
				i = 3;
				break;
			default:
				break;
		}
		return i;
	}
}