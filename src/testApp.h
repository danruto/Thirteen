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
// Filename: testApp.h
// Description: The main entry point of the game. Will deal with the GUI creation and corresponding GUI to game elements
// Author: Danny Sok
// Date Created: 10/04/2013
// Date Last Modified: 21/06/2013

#pragma once

#ifndef TESTAPP_H
#define TESTAPP_H

#include "ofMain.h"
#include "ofEvents.h"
#include "Game\Game.h"
#include "util\Util.h"
#include "ofxUI.h"
#include "ai\OpponentModeller.h"
#include "profile\UserProfile.h"

#define THIRTEEN_VIEW_START 0
#define THIRTEEN_VIEW_SETTINGS 1
#define THIRTEEN_VIEW_GAME 2
#define THIRTEEN_VIEW_SELECT_PROFILE 3
#define THIRTEEN_VIEW_GAME_LOADING 4
#define szIMAGEBUTTON_W 200
#define szIMAGEBUTTON_H 50
#define szFONT 14
#define DEBUG_MODE 1
#define BOTS_ONLY_MODE 0

class testApp : public ofBaseApp{
	public:
		~testApp();

		void setup();
		void setupCoordinates();
		void update();
		void draw();
		void clear();
		void start();
		void restart();
		
		void keyPressed(int key);
		void keyReleased(int key);
		void mouseMoved(int x, int y);
		void mouseDragged(int x, int y, int button);
		void mousePressed(int x, int y, int button);
		void mouseReleased(int x, int y, int button);
		void windowResized(int w, int h);
		void dragEvent(ofDragInfo dragInfo);
		void gotMessage(ofMessage msg);

	private:
		void drawGamesScreen();
		void drawSettingsScreen();
		void drawStartScreen();
		void drawCards(shared_ptr<IPlayer> p, int playerNum);
		void drawPlayerNames(int playerNum);
		void animateCard(int x, int y);
		int firstTurn();
		void findWinner();
		void whoseTurn();
		void hasPlayed(int AINum);
		void debug(std::string msg);
		void setupDB();
		void updateDB();
		void updateProfileWL(int pNum);
		std::string handToString(std::vector<Card> h, unsigned int p);
		void play(int pNum, std::vector<Card> played);
		void pass(int pNum, std::vector<Card> played);
		
		//Game game;
		vector<Card> m_cardsPlayed;
		bool m_bGameStart, m_bFullScreen;
		int m_displayView;
		ofImage m_BackgroundImage;
		

		int m_turnsPassed;
		bool m_bFoundWinner;

		// Profile Number
		int m_profileId; 
		UserProfile userProfile;
		void setupProfile();

		// TODO:
		// Move these to their local functions?
		// or set these up as defines in enums or something
		int p1x, p1y, p2x, p2y, p3x, p3y, p4x, p4y;
		int userButtonsX, userButtonsY, gameStartX, gameStartY;
		int playAreaX, playAreaY, playAreaWidth, playAreaHeight;

		int Player1_X, Player1_Y, Player2_X, Player2_Y, Player3_X, Player3_Y, Player4_X, Player4_Y;
		int Player1_W, Player1_H, Player2_W, Player2_H, Player3_W, Player3_H, Player4_W, Player4_H;
		int PlayerButtons_X, PlayerButtons_Y, PlayerButtons_W, PlayerButtons_H;
		int PlayArea_X, PlayArea_Y, PlayArea_W, PlayArea_H;
		int Card_W, Card_H, Button_W, Button_H;
		
		std::string m_name, m_turnString, m_eventString, m_eventString2;
		ofTrueTypeFont m_vagRounded;

		// NEW FUNCTIONS FOR ofxUI TESTING
		ofxUICanvas *UI_InGame, *UI_StartScreen, *UI_Settings;
		void UI_InGameSetup();
		void UI_StartScreenSetup();
		void UI_SettingsSetup();
		void UI_InGameEvent(ofxUIEventArgs &e);
		void UI_StartScreenEvent(ofxUIEventArgs &e);
		void UI_SettingsEvent(ofxUIEventArgs &e);
		void exit();

		int CARD_SPACE_V;
		int CARD_SPACE_H;
		
		bool m_isAnimating;
};


#endif