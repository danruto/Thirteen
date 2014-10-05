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
// Filename: Card.h
// Description: This will be the representation of a Card. But it will also contain
//				methods on how to draw the card on screen
// Author:		Danny Sok
// Date Created: 10/04/2013
// Date Last Modified: 11/04/2013

#pragma once

#ifndef CARD_H
#define CARD_H

//#define CARD_SPACE_H  40
//#define CARD_SPACE_V  20

//#define CARD_HEIGHT 244
//#define CARD_WIDTH 168

#define CARD_HEIGHT 122
#define CARD_WIDTH 84



#include "ofMain.h"

enum CardValue {
	Ace = 1,	/* 1 */
	Two,		/* 2 */
	Three,		/* 3 */
	Four,		/* 4 */
	Five,		/* 5 */
	Six,		/* 6 */
	Seven,		/* 7 */
	Eight,		/* 8 */
	Nine,		/* 9 */
	Ten,		/* 10 */
	Jack,		/* 11 */
	Queen,		/* 12 */
	King		/* 13 */
};

enum CardSuite { 
	Spade,		/* 0 */
	Club,		/* 1 */
	Diamond,	/* 2 */
	Heart		/* 3 */
};

enum Orientation {
	Horizontal,
	Vertical
};

/*
struct cmp
{
	bool operator()(Card const & left, Card const & right) const 
	{
		if (left.Number > right.Number)
		{
			if (right.Number == Ace || right.Number == Two)
				return false;
			else
				return true;
		}
		else if (left.Number < right.Number)
		{
			if (left.Number == Ace || left.Number == Two)
				return true;
			else 
				return false;
		}
		else //if left.Number == right.Number
		{
			if (left.Suite > right.Suite)
				return true;
			else
				return false;
		}
};*/

class Card {
public:
	Card(CardValue number, CardSuite suite);
	Card(int number, int suite);
	Card(int number, int suite, int w, int h);
	Card(string nameString);
	Card(){};
	~Card(){};

	void setup();
	void setupDraw(int x, int y, int pos, int rotation, Orientation type, int cspace_x, int cspace_y);
	void setupImg();
	void draw();

	CardValue getNumber();
	void setNumber(CardValue number);
	CardSuite getSuite();
	void setSuite(CardSuite suite);

	int getNumberInt();
	int getSuiteInt();

	ofImage getImage();
	ofImage getBackImage();
	void rotateImage(int which);

	void setX(int x);
	int getX();
	void setY(int y);
	int getY();
	int getWidth();
	int getHeight();
	void setWidth(int w);
	void setHeight(int h);
	int getSpacingH();
	int getSpacingV();
	std::string getFilePath();

	bool isSelected();
	void setSelected(bool selected);
	void setNeededByAI(bool b);
	
	std::string toString();

	/*
	*	Test Variables and Functions 
	*/
	//bool& setSelect();
			
	void xName();
	std::string getXS();
	std::string getYS();

	CardValue Number;
	CardSuite Suite;

	void clearImgs();

	/*bool cmp(Card const & left, Card const & right) {
	if (left.Number > right.Number)
		{
			if (right.Number == Ace || right.Number == Two)
				return false;
			else
				return true;
		}
		else if (left.Number < right.Number)
		{
			if (left.Number == Ace || left.Number == Two)
				return true;
			else 
				return false;
		}
		else //if left.Number == right.Number
		{
			if (left.Suite > right.Suite)
				return true;
			else
				return false;
		}
	}*/

private:
	std::string getSuiteString();
	std::string getNumberString();
	

	//CardValue Number;
	//CardSuite Suite;
	ofImage img;
	ofImage backImg;

	int x, y, pos, rotation, width, height;
	int CARD_SPACE_H;
	int CARD_SPACE_V;
	
	Orientation type;

	// Used to determine if the card is selected, so we can remove it from the hand when it is played
	bool selected, rotated;
	bool neededByAI;
	bool isImageAllocated;
};






#endif /* CARD_H */

