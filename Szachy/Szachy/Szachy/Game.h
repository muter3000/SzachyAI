#pragma once
#include <iostream>
#include <SFML/Graphics.hpp>
#include <SFML/Audio.hpp>
#include <list>
#include <map>
#include <thread>

#include "Board.h"

#include "BoxBase.h"
#include "TextBox.h"
#include "InputBox.h"
#include "Button.h"

#include "Pawn.h"
#include "King.h"
#include "Queen.h"
#include "Rook.h"
#include "Night.h"
#include "Bishop.h"

#include "BitboardAlgo.h"
#include "Search.h"

class Game
{
public:
	Game();
	virtual ~Game();

	void pollEvents();
	void update();
	void render();
	const bool isRunning() const;
private:
	const int BoardDirections[8] = { -9,-8,-7,-1,1,7,8,9 };

	sf::RenderWindow* window;
	sf::VideoMode videoMode;
	sf::Event ev;

	Board* board;
	sf::Vector2i boardPos;

	std::map<unsigned int,Piece*> pieces;

	std::list<BoxBase*> boxes;
	bool** availableMoves;

	InputBox* activeInputBox;
	Piece* activePiece;

	TextBox* turnBox;
	TextBox* pgnBox;
	TextBox* castleBox;
	Button* loadPositionButton;
	Button* exportPgn;
	Button* exportFen;
	Button* multiplayer;
	Button* aiBlack;
	Button* aiWhite;

	InputBox* fenInputBox;

	int turnCount;
	int enPassantSquare;

	bool wTurn;
	bool wCastleQ;
	bool bCastleQ;
	bool wCastleK;
	bool bCastleK;

	bool wBot=false;
	bool bBot=true;
	bool botCalculation = false;
	bool moveInProgress = false;
	bool positionLoadSuccess;

	bool calculationComplete;
	bool mousePressed;
	std::pair<bool,int> promotion;
	int promotingPawnPos;

	void initVariables();
	bool initWindow();
	void initBoxes();
	void initBoard();
	bool loadPosition(std::string position);
	void exportPosition();
	void makeBotMove(Move m);

	//PGN 
	void addMoveToPGN(Move m);
	std::string intToCoord(int tile);

	BitboardAlgo* b;
};

