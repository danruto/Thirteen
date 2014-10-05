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
// Filename: testApp.cpp
// Description: The main entry point of the Game::getInstance(). Will deal with the GUI creation and corresponding GUI to game elements
// Author: Danny Sok
// Date Created: 10/04/2013
// Date Last Modified: 21/06/2013

#include "testApp.h"
#include "game\Globals.h"
#include "ai\HandEvaluator.h"



testApp::~testApp()
{
	//OpponentModeller::getInstance().close();
	//delete UI_InGame; delete UI_StartScreen; delete UI_Settings;
	
}
// ----------------------------------------
// setup
// 
// Setup the GUI and the game instance
// Input:
//  
// Output:
//  
// Conditions:
//  
void testApp::setup(){
	Util::Log("Setup");

	
	ofSetWindowTitle("Thirteen - By Danny SOK");
	ofSetFrameRate(60);
	ofSetVerticalSync(true); 
	ofSetEscapeQuitsApp(true);

	m_name = "Mate";
	
	m_BackgroundImage.loadImage("images/bg2.png");

	m_displayView = THIRTEEN_VIEW_START;
	m_bGameStart = false;
	m_bFullScreen = false;
	m_bFoundWinner = false;

	setupCoordinates();
	setupProfile();
	UI_StartScreenSetup();
	UI_SettingsSetup();

	m_vagRounded.loadFont("vag.ttf", szFONT, true, false, true);

	m_turnsPassed = 0;
	OpponentModeller::getInstance().create("OpponentModel.db");
	
	CARD_SPACE_V = 20;
	CARD_SPACE_H = 40;

	Util::Log("Setup Finished");
}

// ----------------------------------------
// setupCoordinates
// 
// Setup the X and Y coordinates of all the drawables
// Input:
//  
// Output:
//  
// Conditions:
//  
void testApp::setupCoordinates()
{
	int screenW = ofGetWindowWidth();
	int screenH = ofGetWindowHeight();
	double Scale_Horizontal_H = 0.2;
	double Scale_Horizontal_W = 0.6;
	ofSetWindowPosition(ofGetWidth()/2, ofGetHeight()/2);

	// Width of 13 cards (horizontal) = 40*13 + (84 - 40) = 564
	// 564 / 2 = 282. Center of X is therefore (screenW / 2) - 324
	p1x = (screenW / 2) - 282;		 
	p1y = screenH - 220; 

	p3x = (screenW / 2) - 282;
	p3y = 50;

	// Width of 13 cards (vertical) = 20 * 13 + (168 - 40)) = 388
	// 388 / 2 = 194
	p2x = 50;		
	p2y = (screenH / 2) - 194;	

	p4x = screenW - 182;
	p4y = (screenH / 2) - 194;

	// 3 Buttons for this section (3*200 = 600. 600/2 = 300)
	userButtonsX = (screenW / 2) - 300;
	userButtonsY = screenH - 70;
	gameStartX = screenW/2 - 200;
	gameStartY = screenH - 150;

	playAreaWidth = 564;
	playAreaHeight = 244;
	playAreaX = (screenW / 2) - playAreaWidth/2;
	playAreaY = (screenH / 2) - playAreaHeight/2;

	//////////////////////////
	//			P3          //
	// P2		PA		P4  //
	//			P1			//
	//////////////////////////

	// Button Area Defines
	
	PlayerButtons_W = PlayerButtons_X * 2;
	PlayerButtons_X = ((screenW) / 3) - (screenW * 0.1); // 3 Buttons, with 10% leeway
	PlayerButtons_H = (screenH) * (0.1);
	PlayerButtons_Y = screenH - PlayerButtons_H; // 10% of the screen
	
	// Player 1 Area
	
	Player1_W = screenW * (Scale_Horizontal_W);
	Player1_H = screenH * (Scale_Horizontal_H);

	Player1_X = screenW * (Scale_Horizontal_H);
	Player1_Y = PlayerButtons_Y - Player1_H;
	

	// Player 2 Area
	
	Player2_W = screenW * (Scale_Horizontal_H);
	Player2_H = screenH * (0.5);

	Player2_X = 0;
	Player2_Y = screenH * (0.2);
	

	// Player 3 Area
	
	Player3_W = Player1_W;
	Player3_H = Player1_H;

	Player3_X = Player1_X;
	Player3_Y = 0;
	

	// Player 4 Area
	
	Player4_W = Player2_W;
	Player4_H = Player2_H;

	Player4_X = screenW - Player4_W;
	Player4_Y = Player2_Y;
	
	// Card Size Defines
	Card_W = Player1_W / 6;
	Card_H = Player1_H - (Player1_H * 0.1);

	// Played Hands Area
	
	PlayArea_W = screenW * (0.6);
	PlayArea_H = screenH * (0.5);

	//PlayArea_X = Player2_W;
	//PlayArea_Y = Player3_H;
	
	PlayArea_X = Player2_W;
	PlayArea_Y = Player3_H + (PlayArea_H/2) - Card_H;
	//setupCardSizes();
	Util::Log("Setup Coordinates Finished");
}

// ----------------------------------------
// update
// 
// Called periodically as part of the openFrameworks kit. Used to change variables before draw() ing them
// Input:
//  
// Output:
//  
// Conditions:
//  
void testApp::update(){
	if (m_displayView == THIRTEEN_VIEW_GAME
		&& m_bGameStart)
	{
		whoseTurn();
		findWinner();
	}
}

// ----------------------------------------
// draw
// 
// Draws the drawables to the GUI
// Input:
//  
// Output:
//  
// Conditions:
//  
void testApp::draw(){
	
	ofSetupScreen();
	m_BackgroundImage.draw(0, 0, 0, ofGetWidth(), ofGetHeight()); 
	//setupCoordinates();
	

	if (m_displayView == THIRTEEN_VIEW_GAME) 
	{
		if (BOTS_ONLY_MODE)
			m_isAnimating = false;
		else
			drawGamesScreen();
	}
	else if (m_displayView == THIRTEEN_VIEW_SETTINGS)
		drawSettingsScreen();
	else if (m_displayView == THIRTEEN_VIEW_START)
		drawStartScreen();
	//cout << "--draw--" << endl;
	//cout << ofGetElapsedTimef() << endl;
}

void testApp::drawSettingsScreen()
{
	//genSettingsGui();
	UI_Settings->setVisible(true);
}

void testApp::drawGamesScreen()
{
	//Util::Log("testApp::drawGamesScreen()");
	// Draw event strings e.g. It is your turn // Invalid Move
	// Draw the strings first so they don't get blocked while waiting on cards!
	m_vagRounded.drawString(m_eventString, PlayArea_X, PlayArea_Y  );
	ofSetColor(ofColor::orangeRed);
	m_vagRounded.drawString(m_eventString2, PlayArea_X, PlayArea_Y + Card_H + szFONT);
	ofSetColor(ofColor::white);
	//m_vagRounded.drawString(m_eventString, playAreaX, playAreaY + 225);
	//m_vagRounded.drawString(m_eventString2, playAreaX, playAreaY + 250);

	// Draw the cards played, maximum 13 cards at a time on screen
	for (int i = 0; i < m_cardsPlayed.size(); i++)
	{
		//Util::Log("drawCardsPlayed()");
		m_cardsPlayed[i].setupDraw(PlayArea_X, PlayArea_Y, i % 13, 0, Horizontal, CARD_SPACE_H, CARD_SPACE_V);
		//m_cardsPlayed[i].setupDraw(playAreaX, playAreaY, i % 13, 0, Horizontal);
		m_cardsPlayed[i].draw();
		
	}

	// Draw cards for the user
	// Draw cards for the AI Players
	// Draw player names
	for (int i = 0; i < Game::getInstance().getPlayerCount(); i++)
	{
		drawCards(Game::getInstance().getPlayer(i), i);
		drawPlayerNames(i);
	}

	m_isAnimating = false;
}

void testApp::drawStartScreen()
{
	UI_StartScreen->setVisible(true);
}
// ----------------------------------------
// drawCards
// 
// Draws the cards onto the screen
// Input:
//  *p is a pointer to a player
//	playNum is the player number e.g. 1, 2, 3 or 4
// Output:
//  
// Conditions:
//  
void testApp::drawCards(shared_ptr<IPlayer> p, int playerNum)
{
	int x, y;
	
	Orientation type;

	if (playerNum == 0)
	{
		x = Player1_X;
		y = Player1_Y;
		//x = p1x;
		//y = p1y;
		type = Horizontal;
	}
	else if (playerNum == 1)
	{
		// [(width of block) / 2] - [(width of cards)/2]
		//x = Player2_X;
		x = (Player2_W / 2) - (Card_H / 2);
		y = Player2_Y;
		//x = p2x;
		//y = p2y;
		type = Vertical;
	}
	else if (playerNum == 2)
	{
		x = Player3_X;
		y = Player3_Y;
		//x = p3x;
		//y = p3y;
		type = Horizontal;
	}
	else //if (playerNum == 3)
	{
		x = Player4_X;
		y = Player4_Y;
		//x = p4x;
		//y = p4y;
		type = Vertical;
	}

	
	std::vector<Card> cards;
	if (p->lock())
	{
		cards = p->getCards();
		p->unlock();
	}

	for (int i = 0; i < cards.size(); i++)
	{
		if (playerNum > 0 && cards[i].isSelected())
		{
		}
		else
		{
			cards[i].setupDraw(x, y, i, playerNum, type, CARD_SPACE_H, CARD_SPACE_V);
			cards[i].draw();
		}
	}
		// Make sure to update the old card
		//p->setCard(i, cards[i]);
	if (p->lock())
	{
		p->setCards(cards);
		p->unlock();
	}
	
	//cout << "--drawCards--" << endl;
	//cout << ofGetElapsedTimef() << endl;
}

// ----------------------------------------
// drawPlayerNames
// 
// Draws the player names onto the screen next to their cards
// Input:
//  playerNum is the player number
// Output:
//  
// Conditions:
//  
void testApp::drawPlayerNames(int playerNum)
{
	int x, y;
	x = y = 0;
	if (playerNum == 0)
	{
		x = Player1_X + Player1_W;
		y = Player1_Y + Player1_H - (Player1_H * 0.1);
		//x = p1x + 570;
		//y = p1y + 100;
	}
	else if (playerNum == 1)
	{
		x = Player2_X;
		y = Player2_Y + Player2_H;
		//x = p2x;
		//y = p2y + 350;
	}
	else if (playerNum == 2)
	{
		x = Player3_X + Player3_W;
		y = Player3_Y + Player3_H - (Player3_H * 0.1);
		//x = p3x + 570;
		//y = p3y + 100;
	}
	else //if (playerNum == 3)
	{
		x = Player4_X;
		y = Player4_Y + Player4_H;
		//x = p4x;
		//y = p4y + 350;
	}

	//if (ofGetFrameNum() % 240 == 0)
		Game::getInstance().getPlayer(playerNum)->drawName(x, y, &m_vagRounded);
		
}

// ----------------------------------------
// animateCard
// 
// Moves the selected cards up by a certain amount to visually show the card selection
// Input:
//  
// Output:
//  
// Conditions:
//  
void testApp::animateCard(int x, int y)
{
	auto cards = Game::getInstance().getPlayer(0)->getCards();
	//for (int i = 0; i < Game::getInstance().getPlayer(0).getCards().size(); i++)
	for (int i = 0; i < cards.size(); i++)
	{
		//Card c = this->Game::getInstance().getPlayer(0).getCards().at(i);
		Card c = cards[i];
		int cY = c.getY();
		int cX = c.getX();
		int newY = cY - (c.getHeight() / 2);

		// TODO: Fix this
		// ::Hack fix to make sure you can't select two cards on the same edge
		if ( (i == cards.size() - 1) && 
			x >= (cX + 1) && x <= ((cX - 1) + c.getSpacingH()) && 
			y >= (cY + 1) && y <= ((cY + 1) + c.getHeight()) )
		{
			if (!c.isSelected())
			{
				//Game::getInstance().getPlayer(0)->getCardAt(i)->setY(cY - CARD_SPACE_V);
				Game::getInstance().getPlayer(0)->getCardAt(i)->setY(newY);
				Game::getInstance().getPlayer(0)->getCardAt(i)->setSelected(true);
			}
			else
				Game::getInstance().getPlayer(0)->getCardAt(i)->setSelected(false);
		}
		//else if ( x >= (cX + 1) && x <= ((cX - 1) + CARD_SPACE_H) )
		else if ( x >= (cX + 1) && x <= ((cX - 1) + c.getSpacingH()) && 
					y >= (cY + 1) && y <= ((cY + 1) + c.getHeight()) )
		{
			if (!c.isSelected())
			{
				Game::getInstance().getPlayer(0)->getCardAt(i)->setY(newY);
				Game::getInstance().getPlayer(0)->getCardAt(i)->setSelected(true);
			}
			else
				Game::getInstance().getPlayer(0)->getCardAt(i)->setSelected(false);
		}
	}

}

// ----------------------------------------
// firstTurn
// 
// Returns the player number of the player who should have the first turn
// Input:
//  
// Output:
//  
// Conditions:
//  
int testApp::firstTurn()
{
	Card lowest = Game::getInstance().getPlayer(0)->getCards()[0]; // Lowest card of first player
	int p = 0;

	for (int i = 1; i < Game::getInstance().getPlayerCount(); i++)
	{
		Card c = Game::getInstance().getPlayer(i)->getCards()[0];
		//Card c = Game::getInstance().getAI(i-1).getCards()[0];
		if (c.getNumber() < lowest.getNumber())
		{
			if (!(c.getNumber() == Ace || c.getNumber() == Two))
			{
				lowest = c;
				p = i;
			}
		}
		else if (c.getNumber() == lowest.getNumber())
		{
			if (c.getSuite() < lowest.getSuite())
			{
				lowest = c;
				p = i;
			}
		}
	}

	return p;
}

// ----------------------------------------
// whoseTurn
// 
// Determines which player should have their turn and display this information onf the screen
// Input:
//  
// Output:
//  
// Conditions:
//  
// IDEA: to implement the free reign rule, count the amount of times a pass call has been made and reset the count if a turn is true. If count reaches 3 then the player who didn't pass can play whatever (can be done by clearing last played)
// Free reign is implemented [25/04/2013]
void testApp::whoseTurn()
{
	int playFrom = 0;

	if (BOTS_ONLY_MODE)
		playFrom = -1;

	for (int i = 0; i < Game::getInstance().getPlayerCount(); i++)
	{
		if (Game::getInstance().getPlayer(i)->isTurn())
		{
			stringstream ss;
			ss << "It is your turn " << Game::getInstance().getPlayer(i)->getName() << endl;
			
			m_eventString = ss.str();
			//if (i > 0) // CHANGE BACK TO THIS FOR HUMAN
			if (i > playFrom)
			{
				m_eventString2 = Game::getInstance().getPlayer(i)->getName() + " is thinking";
				m_eventString2 += ".";
				if (m_eventString2.size() >= 30)
					m_eventString2 = Game::getInstance().getPlayer(i)->getName() + " is thinking";
			}
			if (i > playFrom && !m_isAnimating) 
			//if (i > 0 && !m_isAnimating) 
			{
				hasPlayed(i);	
			}
		}
	}
}

// ----------------------------------------
// findWinner
// 
// Checks the card count for all players. If a player has 0 cards they are the winner
// Input:
//  
// Output:
//  
// Conditions:
//  
void testApp::findWinner()
{	
	bool bWinner = false;	
	int pNum = -1;

	for (int i = 0; i < Game::getInstance().getPlayerCount(); i++)
	{
		if (Game::getInstance().getPlayer(i)->isFinished()) 
		{ 
			bWinner = true; 
			pNum = i; 
			Util::Log("Player: " + ofToString(i) + " Card.size(): " + ofToString(Game::getInstance().getPlayer(i)->getCards().size()));
		}
	}

	//if ( Game::getInstance().getPlayer(0).isFinished() || Game::getInstance().getAI(0).isFinished() || Game::getInstance().getAI(1).isFinished() || Game::getInstance().getAI(2).isFinished() )
	if ( bWinner )
	{
		OpponentModeller::getInstance().updateSession(pNum);
		Game::getInstance().updateWinner(pNum);
		stringstream ss1;
		ss1 << "testApp::findWinner() is " << pNum << endl;
		Util::Log(ss1.str());
		Util::ResultsLog("Winner is: " + Game::getInstance().getPlayer(pNum)->toString());
		updateProfileWL(pNum);

		stringstream ss;

		ss << "You have won: " << userProfile.getWinCount(m_profileId) << " time(s)!" << endl;
		ss << "You have lost: " << userProfile.getLossCount(m_profileId) << " time(s)!" << endl;

		//ss << "Won: " << p1w << " / Lost: " << p1l << endl;
		//ss << "P1W: " << p1w << " P2W: " << p2w << " P3W: " << p3w << " P4W: " << p4w << endl;
		//ss << "It took you: " << Game::getInstance().getPlayer(0)->getTurn() << " turns!" << endl;

		if (!BOTS_ONLY_MODE)
			ofSystemAlertDialog(ss.str());
		Util::Log(ss.str());
		m_bGameStart = false;
		restart();
	}
}

void testApp::updateProfileWL(int pNum)
{
	if (pNum == 0)
		userProfile.setWinCount(m_profileId);
	else
		userProfile.setLossCount(m_profileId);
}

// ----------------------------------------
// hasPlayed
// 
// Checks to see if any of the AI have tried to play a card
// Input:
//  pNum is an int of the Player number (in the vector)
// Output:
//  
// Conditions:
//  
void testApp::hasPlayed(int pNum)
{
	//Util::Log("testApp::hasPlayed()");
	/*vector<Card> played = Game::getInstance().getPlayer(pNum)->play(m_cardsPlayed);
	// debug
	
	
	// If cards have been selected to be played then play them and set the next players turn to true
	play(pNum, played);*/

	// Thinkg of threading, lets get some ideas here

	// If it isn't thinking of making a play, then get the ball rolling
	if (Game::getInstance().getPlayer(pNum)->isTurn())
	{
		if (!Game::getInstance().getPlayer(pNum)->hasMadeMove())
		//if (!Game::getInstance().getPlayer(pNum)->isThreadRunning())
		{
			/*m_eventString2 = Game::getInstance().getPlayer(pNum)->getName() + " is thinking";
			m_eventString2 += ".";
			if (m_eventString2.size() >= 30)
				m_eventString2 = Game::getInstance().getPlayer(pNum)->getName() + " is thinking";*/

			Game::getInstance().getPlayer(pNum)->setLastPlayed(m_cardsPlayed);
			Game::getInstance().getPlayer(pNum)->start();
		}
		else
		{
			vector<Card> played = Game::getInstance().getPlayer(pNum)->play(m_cardsPlayed);
			play(pNum, played);
		}
	}
}

void testApp::play(int pNum, std::vector<Card> played)
{
	//Util::Log("testApp::play()");
	HandEvaluator eval;
	int nextPlayer = pNum + 1;
	if (nextPlayer >= Game::getInstance().getPlayerCount())
		nextPlayer = 0;

	if (played.size() > 0)
	{
		std::vector<int> pos;
		m_turnsPassed = 0;
		m_cardsPlayed = played;
		m_eventString2 = "";

		Game::getInstance().setPassCount(m_turnsPassed);

		HAND_TYPES ht = eval.findHandType(played);
		Game::getInstance().setLastPlayed(played);
		Game::getInstance().setFieldState(ht);

		OpponentModeller::getInstance().insertHand(played, ht, pNum, false);
		Util::Log(handToString(played, pNum));
		Util::Log("testApp::play() - Player: " + ofToString(pNum) + " Cards left: " + 
			ofToString(Game::getInstance().getPlayer(pNum)->getCards().size()) +
			" Just played size: " + ofToString(played.size()));
		Util::ResultsLog("testApp::play() - Player: " + ofToString(pNum) + " Cards left: " + 
			ofToString(Game::getInstance().getPlayer(pNum)->getCards().size()) +
			" Just played size: " + ofToString(played.size()) + " Played: " + handToString(played, pNum));

		if (!Game::getInstance().getPlayer(pNum)->isFinished())
			Game::getInstance().getPlayer(nextPlayer)->setTurn(true);

		m_isAnimating = true;
	}

	// If the cards are still invalid/not selected then notify the user.
	else
	{
		pass(pNum, played);			
	}
}

void testApp::pass(int pNum, std::vector<Card> played)
{
	//Util::Log("testApp::pass()");
	int nextPlayer = pNum + 1;
	if (nextPlayer >= Game::getInstance().getPlayerCount())
		nextPlayer = 0;

	// If the player didn't pass and have not selected any cards, it is an invalid move
	if (Game::getInstance().getPlayer(pNum)->isTurn())
		m_eventString2 = "Invalid Move";
	// AI Passes
	else //if (!Game::getInstance().getPlayer(pNum)->isTurn())
	{
		m_turnsPassed++;
		Util::Log("Player "+ ofToString(pNum) + " passes!");
		Util::ResultsLog("Player "+ ofToString(pNum) + " passes!");
		if (m_turnsPassed == Game::getInstance().getPlayerCount()-1)
		{
			m_turnsPassed = 0;
			m_cardsPlayed.clear();

			Game::getInstance().setLastPlayed(played);
			Util::Log("Free Reign Turn for " + ofToString(nextPlayer) + "!");
			Util::ResultsLog("Free Reign Turn for " + ofToString(nextPlayer) + "!");
		}
		m_eventString2 = "";
		Game::getInstance().setPassCount(m_turnsPassed);
		Game::getInstance().getPlayer(nextPlayer)->setTurn(true);
		m_isAnimating = true;
	}
}

// ----------------------------------------
// clear
// 
// Clears out the variables used for the class
// Input:
//  
// Output:
//  
// Conditions:
//  
void testApp::clear()
{
	m_eventString = "";
	m_eventString2 = "";
	m_cardsPlayed.clear();
	Game::getInstance().clear();
	UI_InGame->setVisible(false);

}

// ----------------------------------------
// start
// 
// Starts the game
// Input:
//  
// Output:
//  
// Conditions:
//  
void testApp::start()
{
	Util::Log("Start");
	m_isAnimating = false;
	std::vector<int> AIToMake;

	AIToMake.push_back(1); // Easy AI
	AIToMake.push_back(0); // Medium AI
	AIToMake.push_back(1); // Hard AI
	
	Game::getInstance().setup(Card_W, Card_H);
	if (BOTS_ONLY_MODE)
	{
		Game::getInstance().start(userProfile.getName(m_profileId), AIToMake);
	}
	else
	{
		Game::getInstance().start(userProfile.getName(m_profileId), 
			userProfile.getNbAIPlayers(m_profileId)+1, 
			userProfile.getAIChoice(m_profileId));
			//AI_HARD);
	}
	
	setupDB();

	m_displayView = THIRTEEN_VIEW_GAME;

	// TODO: Clean this up
	int pNum = firstTurn();

	if (pNum == 0)
	{
		Game::getInstance().getPlayer(0)->setTurn(true);
		if (BOTS_ONLY_MODE)
			Game::getInstance().getPlayer(pNum)->setFirst(true);
	}
	else
	{
		Game::getInstance().getPlayer(pNum)->setTurn(true);
		Game::getInstance().getPlayer(pNum)->setFirst(true);
	}
	//this->Game::getInstance().getPlayer(firstTurn()).setTurn(true);
	m_bGameStart = !m_bGameStart;
	Util::Log("Start Finished");
}

// ----------------------------------------
// restart
// 
// Restarts the game
// Input:
//  
// Output:
//  
// Conditions:
//  
void testApp::restart()
{
	Util::Log("Restart");
	Util::NewLog();

	m_bGameStart = false;
	updateDB();
	clear();

	start();
	UI_InGame->setVisible(true);
}

void testApp::UI_InGameSetup()
{
	UI_InGame = new ofxUICanvas(PlayerButtons_X, PlayerButtons_Y, szIMAGEBUTTON_W * 4, szIMAGEBUTTON_H);
	//UI_InGame = new ofxUICanvas(userButtonsX, userButtonsY, szIMAGEBUTTON_W * 4, szIMAGEBUTTON_H);

	UI_InGame->addImageButton("Pass Image", "images/pass.png", false, szIMAGEBUTTON_W, szIMAGEBUTTON_H);
	UI_InGame->setWidgetPosition(OFX_UI_WIDGET_POSITION_RIGHT);
	UI_InGame->addImageButton("Play Image", "images/play.png", false, szIMAGEBUTTON_W, szIMAGEBUTTON_H);
	UI_InGame->setWidgetPosition(OFX_UI_WIDGET_POSITION_RIGHT);
	UI_InGame->addImageButton("Quit Image", "images/quit.png", false, szIMAGEBUTTON_W, szIMAGEBUTTON_H);
	UI_InGame->setWidgetPosition(OFX_UI_WIDGET_POSITION_RIGHT);
	
	UI_InGame->autoSizeToFitWidgets();    

	ofAddListener(UI_InGame->newGUIEvent, this, &testApp::UI_InGameEvent);
	UI_InGame->setWidgetColor(OFX_UI_WIDGET_COLOR_BACK, ofColor(255,100));
}

void testApp::UI_InGameEvent(ofxUIEventArgs &e)
{
	string name = e.widget->getName(); 
	int kind = e.widget->getKind(); 
	ofxUIImageButton *button = (ofxUIImageButton *) e.widget; 

	if (name == "Play Image" && m_displayView == THIRTEEN_VIEW_GAME)
	{
		//cout << name << "\t value: " << button->getValue() << endl; 
		// add selected cards to table
		if (Game::getInstance().getPlayer(0)->isTurn() && button->getValue())
		{
			vector<Card> played = Game::getInstance().getPlayer(0)->play(m_cardsPlayed);
			//cardsPlayed.insert(cardsPlayed.end(), played.begin(), played.end());
			play(0, played);
		}
	}

	else if (name == "Pass Image" && m_displayView == THIRTEEN_VIEW_GAME)
	{
		//cout << name << "\t value: " << button->getValue() << endl; 
		if (Game::getInstance().getPlayer(0)->isTurn() && button->getValue())
		{
			// free reign, don't allow a pass
			if (m_cardsPlayed.size() == 0)
			{
				m_eventString2 = "You cannot pass on YOUR free reign turn!";
			}
			else
			{
				std::vector<Card> played;
				/*Game::getInstance().getPlayer(0)->pass();
				m_turnsPassed++;
				Util::Log("Player "+ ofToString(0) + " passes!");
				
				if (m_turnsPassed == Game::getInstance().getPlayerCount()-1)
				{
					m_turnsPassed = 0;
					m_cardsPlayed.clear();

					Game::getInstance().setLastPlayed(played);
					Util::Log("Free Reign Turn!");
				}

				Game::getInstance().setPassCount(m_turnsPassed);*/
				Game::getInstance().getPlayer(0)->setTurn(false);
				Game::getInstance().getPlayer(1)->setTurn(true);
				
				pass(0, played);
			}
		}
	}

	else if (name == "Quit Image" && m_displayView == THIRTEEN_VIEW_GAME)
	{
		//cout << name << "\t value: " << button->getValue() << endl; 
		if (button->getValue())
			ofExit();
	}
}

void testApp::UI_StartScreenSetup()
{

	UI_StartScreen = new ofxUICanvas(gameStartX, gameStartY, szIMAGEBUTTON_W * 2, szIMAGEBUTTON_H);

	UI_StartScreen->addImageButton("Start Image", "images/start.png", false, szIMAGEBUTTON_W, szIMAGEBUTTON_H);
	UI_StartScreen->setWidgetPosition(OFX_UI_WIDGET_POSITION_RIGHT);
	UI_StartScreen->addImageButton("Settings Image", "images/settings.png", false, szIMAGEBUTTON_W, szIMAGEBUTTON_H);
	UI_StartScreen->setWidgetPosition(OFX_UI_WIDGET_POSITION_RIGHT);
	UI_StartScreen->autoSizeToFitWidgets();    

    ofAddListener(UI_StartScreen->newGUIEvent, this, &testApp::UI_StartScreenEvent);
	UI_StartScreen->setWidgetColor(OFX_UI_WIDGET_COLOR_BACK, ofColor(255,100));
}

void testApp::UI_StartScreenEvent(ofxUIEventArgs &e)
{
	string name = e.widget->getName(); 
	int kind = e.widget->getKind(); 
	ofxUIImageButton *button = (ofxUIImageButton *) e.widget;

	if (name == "Start Image")
	{
		//start();
		if (button->getValue())
		{
			start();
			UI_InGameSetup();
			UI_StartScreen->setVisible(false);
			//delete UI_StartScreen;
		}
	}
	if (name == "Settings Image")
	{
		//name = ofSystemTextBoxDialog("Please enter your name", "");
		if (button->getValue())
		{
			UI_StartScreen->setVisible(false);
			m_displayView = THIRTEEN_VIEW_SETTINGS;
			//m_displayView = THIRTEEN_VIEW_SELECT_PROFILE;
		}
	}
}

void testApp::UI_SettingsSetup()
{
	//UI_Settings = new ofxUICanvas(ofGetWindowWidth() / 4, 0, ofGetWindowWidth() / 2, ofGetWindowHeight());
	int width = 300;
	int height = 300;
	int x = (ofGetWidth() / 2) - (width/2);
	int y = ofGetHeight() / 2;
	UI_Settings = new ofxUICanvas(x, y, width, height);
	UI_Settings->setTheme(OFX_UI_THEME_HACKER);
	UI_Settings->setColorBack(ofxUIColor::black);

	UI_Settings->addLabel("User Settings", OFX_UI_FONT_LARGE);
	UI_Settings->addLabel("Enter your name below", OFX_UI_FONT_MEDIUM);
	UI_Settings->addTextInput("Name Input", userProfile.getName(m_profileId), 150, 2);
	//UI_Settings->addImageButton("Name Image", "images/name.png", false, szIMAGEBUTTON_W, szIMAGEBUTTON_H);
	
	UI_Settings->addSpacer();

	UI_Settings->addLabel("AI Settings", OFX_UI_FONT_LARGE);

	vector<string> aiAmount; aiAmount.push_back("1"); aiAmount.push_back("2"); aiAmount.push_back("3");
	ofxUIRadio *AIAmountRadio = (ofxUIRadio *)UI_Settings->addRadio("AI Amount", aiAmount, OFX_UI_ORIENTATION_HORIZONTAL);
	std::string defaultAIAmt = userProfile.getNbAIPlayersStr(m_profileId) + "";
	AIAmountRadio->activateToggle(defaultAIAmt);
	UI_Settings->addSpacer();

	vector<string> aiDifficulty; aiDifficulty.push_back("Easy"); aiDifficulty.push_back("Medium"); aiDifficulty.push_back("Hard"); aiDifficulty.push_back("Random");
	ofxUIRadio *AIDiffRadio = (ofxUIRadio *)UI_Settings->addRadio("AI Difficulty", aiDifficulty, OFX_UI_ORIENTATION_HORIZONTAL);
	std::string defaultAIDiff = userProfile.getAIChoiceStr(m_profileId);
	AIDiffRadio->activateToggle(defaultAIDiff);
	UI_Settings->addSpacer();

	UI_Settings->addImageButton("Back Image", "images/back.png", false, szIMAGEBUTTON_W, szIMAGEBUTTON_H);

	UI_StartScreen->autoSizeToFitWidgets();    
	ofAddListener(UI_Settings->newGUIEvent, this, &testApp::UI_SettingsEvent);
	UI_Settings->setWidgetColor(OFX_UI_WIDGET_COLOR_BACK, ofColor(255,100));

	UI_Settings->setVisible(false);

	
}

void testApp::UI_SettingsEvent(ofxUIEventArgs &e)
{
	string name = e.widget->getName(); 
	int kind = e.widget->getKind(); 
	

	if (kind == OFX_UI_WIDGET_TOGGLE)
	{
		ofxUIToggle *toggle = (ofxUIToggle *) e.widget;

		if (name == "1")
		{
			//m_numPlayers = 2;
			userProfile.setNbAIPlayers(m_profileId, 1);
		}
		if (name == "2")
		{
			//m_numPlayers = 3;
			userProfile.setNbAIPlayers(m_profileId, 2);
		}
		if (name == "3")
		{
			//m_numPlayers = 4;
			userProfile.setNbAIPlayers(m_profileId, 3);
		}
		if (name == "Easy")
		{
			userProfile.setAIChoice(m_profileId, AI_EASY);
		}
		if (name == "Medium")
		{
			userProfile.setAIChoice(m_profileId, AI_MEDIUM);
		}
		if (name == "Hard")
		{
			userProfile.setAIChoice(m_profileId, AI_HARD);
		}
		if (name == "Random")
		{
			userProfile.setAIChoice(m_profileId, AI_EASY);
		}
	}
	if (name == "Name Input")
	{
		ofxUITextInput *text = (ofxUITextInput *) e.widget;

		m_name = text->getTextString();
		userProfile.setName(m_profileId, m_name);
	}
	if (name == "Name Image")
	{
		ofxUIImageButton *button = (ofxUIImageButton *) e.widget;
		if(button->getValue())
		{
			m_name = ofSystemTextBoxDialog("Please enter your name", "");
		}
	}
	if (name == "Back Image")
	{
		ofxUIImageButton *button = (ofxUIImageButton *) e.widget;
		if (button->getValue())
		{
			//userProfile.save(profile, m_name, m_numPlayers, m_ai);
			UI_Settings->setVisible(false);
			m_displayView = THIRTEEN_VIEW_START;
			
		}
		
	}
}

void testApp::exit()
{
    //gui->saveSettings("GUI/guiSettings.xml");     
	OpponentModeller::getInstance().close();
	Util::WriteLog();

	try
	{
		//delete UI_InGame; 
		//delete UI_StartScreen; 
		//delete UI_Settings;
	}
	catch (const ios::failure &) {}
}

void testApp::setupDB()
{
	OpponentModeller::getInstance().insertSession(Game::getInstance().getPlayerCount());
	
	for (int i = 0; i < Game::getInstance().getPlayerCount(); i++)
	{
		OpponentModeller::getInstance().insertPlayer(i, Game::getInstance().getPlayer(i)->getType(), 0, 0);
	}
}

void testApp::updateDB()
{
	for (int i = 0; i < Game::getInstance().getPlayerCount(); i++)
	{
		OpponentModeller::getInstance().updatePlayer(i, Game::getInstance().getPlayer(i)->getTurn(), Game::getInstance().getPlayer(i)->getPlayCount());
	}
}

void testApp::setupProfile()
{
	m_profileId = 1;
	userProfile.Init();
}

// ----------------------------------------
// debug
// 
// Outputs the msg string too the debug console
// Input:
//  msg is a string to display on the debug console
// Output:
//  none
// Conditions:
//  none
void testApp::debug(std::string msg)
{
	if (DEBUG_MODE)
	{
		//cout << msg << ": " << ofGetElapsedTimef() << endl;
		stringstream ss;
		ss << msg << ": " << ofGetElapsedTimef() << endl;
		ofLog(OF_LOG_VERBOSE, ss.str());
		cout << ss.str() << endl;
	}
}

std::string testApp::handToString(std::vector<Card> h, unsigned int p)
{
	string tCard;

	if (h.size() > 0)
	{
		for (int i = 0; i < h.size(); i++)
		{
			if (i == h.size()-1)
				tCard += h[i].toString();
			else
				tCard += h[i].toString() + " ";
		}
	}
	else
		tCard = "Pass || HandSize = 0";

	tCard += " :" + ofToString(p);
	return tCard;
}



// ----------------------------------------
// 
// 
// 
// Input:
//  
// Output:
//  
// Conditions:
//  
void testApp::keyPressed(int key){
	if (key == OF_KEY_RIGHT)
	{		
	}
	if (key == OF_KEY_LEFT)
	{
	}
	if (key == OF_KEY_UP)
	{
	}
	if (key == OF_KEY_DOWN)
	{
	}
	if (key == 's' || key == 'S')
	{
		start();
	}
	if (key == 'r' || key == 'R')
	{
		restart();
	}
	if (key == 'f' || key == 'F')
	{
		m_bFullScreen = !m_bFullScreen;
		ofSetFullscreen(!m_bFullScreen);
	}
	if (key == 'z' || key == 'Z')
	{
		
	}
}

//--------------------------------------------------------------
void testApp::keyReleased(int key){

}

//--------------------------------------------------------------
void testApp::mouseMoved(int x, int y){

}

//--------------------------------------------------------------
void testApp::mouseDragged(int x, int y, int button){

}

//--------------------------------------------------------------
void testApp::mousePressed(int x, int y, int button){
}

// ----------------------------------------
// mouseReleased
// 
// Will read the mouse input and if they are within certain values then execute that option. The options are described below
// Input:
//  x is the x value of the mouse
//	y is the y value of the mouse
//	button is the button value of the mouse e.g. MB3
// Output:
//  
// Conditions:
//  

void testApp::mouseReleased(int x, int y, int button){
	
	//cout << " MX: " << x << " MY: " << y << endl;
	if (m_bGameStart)
	{		
		// ----------------------------------------
		//	Animate Card
		// ----------------------------------------
		if (x >= Player1_X && x <= Player1_X + Player1_W + CARD_WIDTH && //564
			y >= Player1_Y - (CARD_HEIGHT/2) && y <= Player1_Y + (CARD_HEIGHT))
		{
			animateCard(x, y);
			//cout << "x: " << x << endl;
		}	
	}	

	// X + W
	// 0---------
}

//--------------------------------------------------------------
void testApp::windowResized(int w, int h){

}

//--------------------------------------------------------------
void testApp::gotMessage(ofMessage msg){

}

//--------------------------------------------------------------
void testApp::dragEvent(ofDragInfo dragInfo){ 

}