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
// Filename: Card.cpp
// Description: This will be the representation of a Card. But it will also contain
//				methods on how to draw the card on screen
// Author:		Danny Sok
// Date Created: 10/04/2013
// Date Last Modified: 11/04/2013

#include <string>
#include "Card.h"
#include "..\util\Util.h"


using namespace std;

//ofEvent<ofVec2f> Card::clickedInsideGlobal = ofEvent<ofVec2f>();

// ----------------------------------------
// Card (constructor)
//  
//  Will setup the class
// Input:
//  number is the card number e.g. Three
//	suite is the card suite e.g. Heart
// Output:
//  
// Conditions:
//  
Card::Card(CardValue number, CardSuite suite)
{
	Number = number;
	Suite = suite;
	setup();	
}

// ----------------------------------------
// Card (constructor)
//  
//  Will setup the class
// Input:
//  number is the card number e.g. Three
//	suite is the card suite e.g. Heart
// Output:
//  
// Conditions:
//  
Card::Card(int number, int suite, int w, int h)
{
	Number = (CardValue) (number + 1);
	Suite = (CardSuite) suite;
	setup();	
	width = w;
	height = h;
}

// ----------------------------------------
// Card (constructor)
//  
//  Will setup the class
// Input:
//  number is the card number e.g. Three
//	suite is the card suite e.g. Heart
// Output:
//  
// Conditions:
//  
Card::Card(int number, int suite)
{
	Number = (CardValue) (number + 1);
	Suite = (CardSuite) suite;
	setup();
}

Card::Card(std::string nameString)
{
	if (nameString.size() == 2)
	{
		char n = nameString[0];
		char s = nameString[1];
		Number = (CardValue)Util::charToCardValue(n);
		Suite = (CardSuite)Util::charToCardSuite(s);
	}
	else if (nameString.size() == 3)
	{
		string n = nameString.substr(0, 2);
		char s = nameString[2];
		Number = (CardValue) ofToInt(n.c_str());
		Suite = (CardSuite)Util::charToCardSuite(s);
	}

	setup();
}

// ----------------------------------------
// setup
// 
// Will load the appropriate image for the card and setup where the card will be drawn as well as orientation
// Input:
//  
// Output:
//  
// Conditions:
//  
void Card::setup()
{
	width = CARD_WIDTH;
	height = CARD_HEIGHT;
	CARD_SPACE_H = 40;
	CARD_SPACE_V = 20;
	x = y = rotation = pos = 0;
	selected = false;
	rotated = false;
	isImageAllocated = false;
}

// ----------------------------------------
// setupDraw
// 
// 
// Input:
//  x is an int represeting the x value on the x/y/z plane
//	y is an int represeting the y value on the x/y/z plane
//	pos is an int representing the position of the card in the hand
//	rotation is an int representing the rotation amount in 90 degree rotations
//	type is an enum representing if the card is Vertical or Horizontal
// Output:
//  
// Conditions:
//  
void Card::setupDraw(int x, int y, int pos, int rotation, Orientation type, int cspace_x, int cspace_y)
{
	
	setupImg();
	this->x = x;
	this->pos = pos;
	this->rotation = rotation;
	this->type = type;
	CARD_SPACE_H = cspace_x;
	CARD_SPACE_V = cspace_y;

	if (!selected)
	{
		this->y = y;
	}

	//cout << "X: " << this->x << " _X: " << x << " Y: " << this->y << " _Y: " << y << endl;
}

void Card::setupImg()
{
	//if (!isImageAllocated)
	//{
		if (rotation > 0)
		{
			if (!backImg.isAllocated() || !backImg.isUsingTexture())
			{
				backImg.loadImage("images/cover.png");
				backImg.rotate90(rotation);
				//rotateImage(rotation);
			}
		}
		if (rotation == 0)
		{
			if (!img.isAllocated())
				img.loadImage(getFilePath());
		}

		//isImageAllocated = true;
	//}
}

// ----------------------------------------
// draw
// 
//  The cards will be drawn on the screen depending on the type
//	and parameters that were previously setup
// Input:
//  
// Output:
//  
// Conditions:
//  
void Card::draw(){
	if (type == Horizontal)
	{
		// Setup the card to be evenly spaced
		x += pos * CARD_SPACE_H;
		//cout << "-----------------------------" << endl;
		//cout << "Name: " << toString() << " X: " << x << endl;

		// Rotation here actually means playerNum. If it is 0 then it is the player and 
		// hence the cards are visible. If it is 1-3 then it is an AI agent and so do not 
		// show the card to the user
		if (rotation == 0)
			img.draw(x, y , width, height);
		else
			backImg.draw(x, y , width, height);

	}
	else // type == Vertical
	{
		// Setup the card to be evenly spaced
		y += pos * CARD_SPACE_V;

		// Rotate the card according to the player num
		rotateImage(1);		
		backImg.draw(x, y, height, width); // height and width are reversed due to the rotation
	}
}


// ----------------------------------------
// getNumber
// 
// Returns the CardValue of the card
// Input:
//  
// Output:
//  Number which is the enum CardValue saved
// Conditions:
//  
CardValue Card::getNumber()
{
	return Number;
}

int Card::getNumberInt()
{
	int i = Number;
	return i;
}

// ----------------------------------------
// setNumber
// 
// sets the CardValue of the card
// Input:
//  number
// Output:
//  
// Conditions:
//  
void Card::setNumber(CardValue number)
{
	Number = number;
}

// ----------------------------------------
// getSuite
// 
// Returns the CardSuite of the card
// Input:
//  
// Output:
//  
// Conditions:
//  
CardSuite Card::getSuite()
{
	return Suite;
}

int Card::getSuiteInt()
{
	int i = Suite;
	return i;
}

// ----------------------------------------
// setSuite
// 
// Sets the CardSuite of the card
// Input:
//  
// Output:
//  
// Conditions:
//  
void Card::setSuite(CardSuite suite)
{
	Suite = suite;
}

string Card::getXS()
{
	stringstream ss;
	ss << x;
	return ss.str();
}

// ----------------------------------------
// getX
// 
// Returns the x value of the card
// Input:
//  
// Output:
//  y
// Conditions:
//  
int Card::getX()
{
	return x;
}

// ----------------------------------------
// setX
// 
//  Sets the x value of the card
// Input:
//  x which is an int representing x in the x/y/z plane
// Output:
//  
// Conditions:
//  
void Card::setX(int x)
{
	this->x = x;
}

string Card:: getYS()
{
	stringstream ss;
	ss << y;
	return ss.str();
}

// ----------------------------------------
// getY
// 
// Returns the y value
// Input:
//  
// Output:
//  y
// Conditions:
//  
int Card::getY()
{
	return y;
}

// ----------------------------------------
// setY
// 
// Sets the y value
// Input:
//  y which is an int representing y in the x/y/z plane
// Output:
//  
// Conditions:
//  
void Card::setY(int y)
{
	this->y = y;
}

// ----------------------------------------
// getHeight
// 
// Returns the height of the card
// Input:
//  
// Output:
//  height
// Conditions:
//  
int Card::getHeight()
{
	return height;
}

int Card::getWidth()
{
	return width;
}

void Card::setHeight(int h)
{
	height = h;
}

void Card::setWidth(int w)
{
	width = w;
}

int Card::getSpacingH()
{
	return CARD_SPACE_H;
}

int Card::getSpacingV()
{
	return CARD_SPACE_V;
}

// ----------------------------------------
// getImage
// 
// Returns the ofImage of the card
// Input:
//  
// Output:
//  img which is an ofImage type
// Conditions:
//  
ofImage Card::getImage()
{
	return img;
}

// ----------------------------------------
// getBackImage
// 
// Returns the back side image of the card
// Input:
//  
// Output:
//  backImg which is an ofImage type
// Conditions:
//  
ofImage Card::getBackImage()
{
	return backImg;
}

// ----------------------------------------
// rotateImage
// 
// Rotates the image in 90 degree clickwise turns based on the input
// Input:
//  which - an int representing whether it is a computer player (1) or user (0)
// Output:
//  
// Conditions:
//  
void Card::rotateImage(int which)
{
	if (!rotated)
	{
		rotated = true;
		if (which == 0)
			img.rotate90(rotation);
		else
			backImg.rotate90(rotation);
	}
}

// ----------------------------------------
// getFilePath
// 
// Determines the filepath of the image that is correllated to the current card
// Input:
//  
// Output:
//  ss.str which is a string containing the file path to the card
// Conditions:
//  
string Card::getFilePath()
{
	std::stringstream ss;
	
	switch (getSuite()) //issue
	{
		case Spade:
			ss << "images/spade/" << getNumber() << ".png";
			break;
		case Club:
			ss << "images/club/" << getNumber() << ".png";
			break;
		case Diamond:
			ss << "images/diamond/" << getNumber() << ".png";
			break;
		case Heart:
			ss << "images/heart/" << getNumber() << ".png";
			break;
		default:
			ss << "";
			break;
	}

	return ss.str();
}

// ----------------------------------------
// toString
// 
// Returns the card value and suite
// Input:
//  
// Output:
//  card value and suite
// Conditions:
//  
string Card::toString()
{
	return "" + getNumberString() + getSuiteString();	
}

// ----------------------------------------
// setSelected
// 
// Sets the selected boolean based on the input
// Input:
//  selected is a boolean
// Output:
//  
// Conditions:
//  
void Card::setSelected(bool selected)
{
	this->selected = selected;
}

/*bool& Card::setSelect()
{
	return selected;
}*/

// ----------------------------------------
// isSelected
// 
// Returns whether or not the card is selected
// Input:
//  
// Output:
//  selected
// Conditions:
//  
bool Card::isSelected()
{
	return selected;
}

void Card::xName()
{
	if (x > 0)
	{
		cout << "X: " << x << endl;
	}
	cout << "Name: " << toString() << endl;
}


// ----------------------------------------
// getSuiteString
// 
// 
// Input:
//  
// Output:
//  s which is the string of the Suite
// Conditions:
//  
string Card::getSuiteString()
{
	string s = "";

	switch (Suite)
	{
		case Spade:
			s = "S";
			break;
		case Club:
			s = "C";
			break;
		case Diamond:
			s = "D";
			break;
		case Heart:
			s = "H";
			break;
		default:
			s = "";
			break;
	}

	return s;
}

// ----------------------------------------
// getNumberString
// 
// 
// Input:
//  
// Output:
//  s which is the string representation of the CardValue enum
// Conditions:
//  
string Card::getNumberString()
{
	string s = "";
	
	switch (Number)
	{
		case Ace:
			return "A";
			break;
		case Two:
			return "2";
			break;
		case Three:
			return "3";
			break;
		case Four:
			return "4";
			break;
		case Five:
			return "5";
			break;
		case Six:
			return "6";
			break;
		case Seven:
			return "7";
			break;
		case Eight:
			return "8";
			break;
		case Nine:
			return "9";
			break;
		case Ten:
			return "10";
			break;
		case Jack:
			s = "J";
			break;
		case Queen:
			s = "Q";
			break;
		case King:
			s = "K";
			break;
		default:
			return "";
			break;
	}

	return s;
}

void Card::setNeededByAI(bool b)
{
	neededByAI = b;
}

void Card::clearImgs()
{
	img.clear();
	backImg.clear();
}