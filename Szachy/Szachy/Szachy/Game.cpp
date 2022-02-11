#include "Game.h"
#include <string>
#include <sstream>

Game::Game()
{
	this->initVariables();
	this->initWindow();
	this->initBoxes();
	this->initBoard();
}

Game::~Game()
{
	delete this->board;
	delete this->window;
	for (auto& kv : pieces)
	{
		delete kv.second;
		kv.second = nullptr;
	}
	for (size_t i = 0; i < 64; i++)
	{
		if(availableMoves[i])
		delete[] availableMoves[i];
	}
	delete[] availableMoves;
	std::for_each(boxes.begin(), boxes.end(), [](BoxBase*& box) {delete box; box = nullptr; });
	delete b;
}

void Game::pollEvents()
{
	static sf::SoundBuffer buffer = []()
	{
		sf::SoundBuffer b;
		if (!b.loadFromFile("stuk.ogg"))
		{
			std::cerr << "Sound loading failed!\n";
		}
		return b;
	}();
	static sf::Sound sound = []() { sf::Sound s; s.setBuffer(buffer); return s; }();

	//Event polling
	while (this->window->pollEvent(this->ev))
	{
		sf::Vector2i mPos = sf::Mouse::getPosition(*window);
		switch (this->ev.type)
		{
		case sf::Event::MouseButtonPressed:
			if (!mousePressed)
			{
				if (activeInputBox)
				{
					activeInputBox->toggleActive();
					activeInputBox = nullptr;
				}
				if (fenInputBox->isInBounds(mPos))
				{
					activeInputBox = fenInputBox;
				}
				else if (exportPgn->isInBounds(mPos))
				{
					exportPgn->click();
					sf::Clipboard::setString(pgnBox->getText());
				}
				else if (exportFen->isInBounds(mPos))
				{
					exportFen->click();
					exportPosition();
				}
				else if (loadPositionButton->isInBounds(mPos))
				{
					loadPositionButton->click();
					positionLoadSuccess = loadPosition(fenInputBox->getText());
					if (positionLoadSuccess)
					{
						b->calculateMoves();
						auto test = b->getLegalMoves(availableMoves);
						if (!test.first) {
							if (test.second == 0)
							{
								turnBox->setText("Stalemate");
								pgnBox->setText(pgnBox->getText() + "1/2-1/2");
							}
							else
							{
								turnBox->setText("Checkmate");
								pgnBox->setText(pgnBox->getText() + (wTurn ? "0-1" : "1-0"));
							}

						}
						calculationComplete = true;
					}
					else
					{
						for (auto& kv : pieces)
						{
							delete kv.second;
							kv.second = nullptr;
						}
					}
				}
				else if (multiplayer->isInBounds(mPos))
				{
					multiplayer->click();
					positionLoadSuccess = loadPosition(fenInputBox->getText());
					if (positionLoadSuccess)
					{
						bBot = false;
						wBot = false;

						b->calculateMoves();
						auto test = b->getLegalMoves(availableMoves);
						if (!test.first) {
							if (test.second == 0)
							{
								turnBox->setText("Stalemate");
								pgnBox->setText(pgnBox->getText() + "1/2-1/2");
							}
							else
							{
								turnBox->setText("Checkmate");
								pgnBox->setText(pgnBox->getText() + (wTurn ? "0-1" : "1-0"));
							}

						}

						calculationComplete = true;
					}
					else
					{
						for (auto& kv : pieces)
						{
							delete kv.second;
							kv.second = nullptr;
						}
					}
				}
				else if (aiBlack->isInBounds(mPos))
				{
					aiBlack->click();
					positionLoadSuccess = loadPosition(fenInputBox->getText());
					if (positionLoadSuccess)
					{
						bBot = false;
						wBot = true;

						b->calculateMoves();
						auto test = b->getLegalMoves(availableMoves);
						if (!test.first) {
							if (test.second == 0)
							{
								turnBox->setText("Stalemate");
								pgnBox->setText(pgnBox->getText() + "1/2-1/2");
							}
							else
							{
								turnBox->setText("Checkmate");
								pgnBox->setText(pgnBox->getText() + (wTurn ? "0-1" : "1-0"));
							}

						}

						calculationComplete = false;
					}
					else
					{
						for (auto& kv : pieces)
						{
							delete kv.second;
							kv.second = nullptr;
						}
					}
				}
				else if (aiWhite->isInBounds(mPos))
				{
					aiWhite->click();
					positionLoadSuccess = loadPosition(fenInputBox->getText());
					if (positionLoadSuccess)
					{
						bBot = true;
						wBot = false;

						b->calculateMoves();
						auto test = b->getLegalMoves(availableMoves);
						if (!test.first) {
							if (test.second == 0)
							{
								turnBox->setText("Stalemate");
								pgnBox->setText(pgnBox->getText() + "1/2-1/2");
							}
							else
							{
								turnBox->setText("Checkmate");
								pgnBox->setText(pgnBox->getText() + (wTurn ? "0-1" : "1-0"));
							}

						}
						calculationComplete = false;
					}
					else
					{
						for (auto& kv : pieces)
						{
							delete kv.second;
							kv.second = nullptr;
						}
					}
				}
				else
				{
					Piece* clickedPiece = nullptr;

					unsigned int key((int)(mPos - boardPos).x / tSize + (int)(mPos - boardPos).y / tSize*8);
					
					std::map<unsigned int, Piece*>::iterator it = pieces.find(key);
					if (it != pieces.end())
						clickedPiece = pieces[key];
					if (clickedPiece)
					{
						activePiece = clickedPiece;
						board->setActive(activePiece->getPosInt());
						if ((wTurn ? !wBot : !bBot))
						{
							for (int i = 0; i < 64; i++)
							{
								if (availableMoves[activePiece->getPosInt()][i])
									board->setActive(i);
							}
						}
					}
				}
				if (activeInputBox)
					this->activeInputBox->toggleActive();
				mousePressed = true;
			}
			break;
		case sf::Event::MouseButtonReleased:
			mousePressed = false;
			for (int i = 0; i < 64; i++)
			{
				board->setUnactive(i);
			}
			if (activePiece)
			{
				int xPos = (int)(mPos - boardPos).x / tSize;
				int yPos = (int)(mPos - boardPos).y / tSize;

				board->setUnactive(activePiece->getPosInt());
				if (xPos + yPos * 8<64&&availableMoves[activePiece->getPosInt()][xPos + yPos * 8]&&(wTurn?!wBot:!bBot))
				{
					bool moveMade = false;
					if (activePiece->getPieceType() == PieceType::bPawn || activePiece->getPieceType() == PieceType::wPawn)
					{
						static_cast<Pawn*>(activePiece)->moved = true;
						if (xPos + yPos * 8 == enPassantSquare)
						{
							delete pieces[enPassantSquare + (wTurn ? BoardDirections[6] : BoardDirections[1])];
							pieces.erase(enPassantSquare + (wTurn ? BoardDirections[6] : BoardDirections[1]));
							enPassantSquare = -1;
							addMoveToPGN(Move(63 - activePiece->getPosInt() | ((63 - (xPos + yPos * 8)) << 6) | (static_cast<int>(activePiece->getPieceType()) << 16) | (unsigned int(1) << 13)));
							b->makeMove(Move(63 - activePiece->getPosInt() | ((63 - (xPos + yPos * 8)) << 6) | (static_cast<int>(activePiece->getPieceType()) << 16) | (unsigned int(1) << 13)));
							moveMade = true;
						}
						else if (xPos + yPos * 8 == (activePiece->getPosInt() + (wTurn ? BoardDirections[1] * 2 : BoardDirections[6] * 2)))
						{
							enPassantSquare = activePiece->getPosInt() + (wTurn ? BoardDirections[1] : BoardDirections[6]);
							addMoveToPGN(Move(63 - activePiece->getPosInt() | ((63 - (xPos + yPos * 8)) << 6) | (static_cast<int>(activePiece->getPieceType()) << 16) | (unsigned int(0) << 13)));
							b->makeMove(Move(63 - activePiece->getPosInt() | ((63 - (xPos + yPos * 8)) << 6) | (static_cast<int>(activePiece->getPieceType()) << 16) | (unsigned int (4) << 13)));
							moveMade = true;
						}
						else
						{
							enPassantSquare = -1;
						}
						if (wTurn ? yPos == 0 : yPos == 7)
						{
							promotion.first = true;
							promotion.second = yPos * 8 + xPos;
							promotingPawnPos = 63 - activePiece->getPosInt();
							for (int i = 0; i < 64; i++)
							{
								for (int j = 0; j < 64; j++)
								{
									availableMoves[i][j] = false;
								}
							}
							moveMade = true;
							calculationComplete = false;
						}
					}
					else if (activePiece->getPieceType() == PieceType::bKing || activePiece->getPieceType() == PieceType::wKing)
					{
						if (wTurn)
						{
							if (wCastleK)
							{
								if (xPos + yPos * 8 == 62)
								{
									pieces[61] = pieces[63];
									pieces.erase(63);
									pieces[61]->updatePos(boardPos, 61);
									addMoveToPGN(Move(63 - activePiece->getPosInt() | ((63 - (xPos + yPos * 8)) << 6) | (static_cast<int>(activePiece->getPieceType()) << 16) | (unsigned int(2) << 13) | (unsigned int(1) << 25)));
									b->makeMove(Move(63 - activePiece->getPosInt() | ((63 - (xPos + yPos * 8)) << 6) | (static_cast<int>(activePiece->getPieceType()) << 16) | (unsigned int(2) << 13)|(unsigned int(1)<<25)));
									moveMade = true;
								}
							}
							if (wCastleQ)
							{
								if (xPos + yPos * 8 == 58)
								{
									pieces[59] = pieces[56];
									pieces.erase(56);
									pieces[59]->updatePos(boardPos, 59);
									addMoveToPGN(Move(63 - activePiece->getPosInt() | ((63 - (xPos + yPos * 8)) << 6) | (static_cast<int>(activePiece->getPieceType()) << 16) | (unsigned int(2) << 13) | (unsigned int(0) << 25)));
									b->makeMove(Move(63 - activePiece->getPosInt() | ((63 - (xPos + yPos * 8)) << 6) | (static_cast<int>(activePiece->getPieceType()) << 16) | (unsigned int(2) << 13) | (unsigned int(0) << 25)));
									moveMade = true;
								}
							}
							wCastleK = false;
							wCastleQ = false;
						}
						else
						{
							if (bCastleK)
							{
								if (xPos + yPos * 8 == 6)
								{
									pieces[5] = pieces[7];
									pieces.erase(7);
									pieces[5]->updatePos(boardPos, 5);
									addMoveToPGN(Move(63 - activePiece->getPosInt() | ((63 - (xPos + yPos * 8)) << 6) | (static_cast<int>(activePiece->getPieceType()) << 16) | (unsigned int(2) << 13) | (unsigned int(3) << 25)));
									moveMade = true;
									b->makeMove(Move(63 - activePiece->getPosInt() | ((63 - (xPos + yPos * 8)) << 6) | (static_cast<int>(activePiece->getPieceType()) << 16) | (unsigned int(2) << 13) | (unsigned int(3) << 25)));
									moveMade = true;
								}
							}
							if (bCastleQ)
							{
								if (xPos + yPos * 8 == 2)
								{
									pieces[3] = pieces[0];
									pieces.erase(0);
									pieces[3]->updatePos(boardPos, 3);
									addMoveToPGN(Move(63 - activePiece->getPosInt() | ((63 - (xPos + yPos * 8)) << 6) | (static_cast<int>(activePiece->getPieceType()) << 16) | (unsigned int(2) << 13) | (unsigned int(2) << 25)));
									b->makeMove(Move(63 - activePiece->getPosInt() | ((63 - (xPos + yPos * 8)) << 6) | (static_cast<int>(activePiece->getPieceType()) << 16) | (unsigned int(2) << 13) | (unsigned int(2) << 25)));
									moveMade = true;
								}
							}
							bCastleK = false;
							bCastleQ = false;
						}
							
					}
					if (wTurn)
					{
						if (activePiece->getPosInt() == 63 || (xPos + yPos * 8) == 63)
						{
							wCastleK = false;
						}
						else if (activePiece->getPosInt() == 56 || (xPos + yPos * 8) == 56)
						{
							wCastleQ = false;
						}
					}
					else 
					{
						if (activePiece->getPosInt() == 7 || (xPos + yPos * 8) == 7)
						{
							wCastleK = false;
						}
						else if (activePiece->getPosInt() == 0 || (xPos + yPos * 8) == 0)
						{
							wCastleQ = false;
						}
					}
					calculationComplete = false;
					sound.play();
					if(!promotion.first&&!moveMade)
						addMoveToPGN(Move(63 - activePiece->getPosInt() | ((63 - (xPos + yPos * 8)) << 6) | (static_cast<int>(activePiece->getPieceType()) << 16) | (pieces[xPos + yPos * 8] ? 1 : 0) << 20));
					if(!moveMade)
					b->makeMove(Move(63-activePiece->getPosInt() | ((63-(xPos + yPos * 8))<<6) | (static_cast<int>(activePiece->getPieceType()) << 16)|(pieces[xPos + yPos * 8]?1:0)<<20));

					delete pieces[xPos + yPos * 8];
					pieces[xPos + yPos * 8] = activePiece;

					pieces.erase(activePiece->getPos().x + activePiece->getPos().y * 8);
					activePiece->updatePos(boardPos, xPos+ yPos*8);
					if (wTurn)
					{
						wTurn = false;
					}
					else
					{
						wTurn = true;
					}
					int castleFlag[4] = { wCastleK,wCastleQ,bCastleK,bCastleQ };
					std::string castleString = "";
					for (int i = 0; i < 4; i++)
					{
						switch (i)
						{
						case 0:
							if (castleFlag[i])
								castleString += "K";
							break;
						case 1:
							if (castleFlag[i])
								castleString += "Q";
							break;
						case 2:
							if (castleFlag[i])
								castleString += "k";
							break;
						case 3:
							if (castleFlag[i])
								castleString += "q";
							break;
						}
					}
					this->castleBox->setText(castleString);
					if (wTurn&&!promotion.first)
						turnBox->setText("White to move");
					else if(!wTurn&&!promotion.first)
						turnBox->setText("Black to move");
					else
					{
						turnBox->setText("Promotion: F1-Q, F2-B, F3-N, F4-R");
					}
				}
				else
				{
					activePiece->updatePos(boardPos, activePiece->getPos().x + activePiece->getPos().y*8);
				}
				activePiece = nullptr;
			}
			break;
		case sf::Event::Closed:
			this->window->close();
			break;
		case sf::Event::KeyPressed:
			switch (this->ev.key.code)
			{
			case sf::Keyboard::Escape:
				this->window->close();
				break;
			case sf::Keyboard::Insert:
				if (activeInputBox)
				{
					activeInputBox->setText(sf::Clipboard::getString());
				}
				break;
			case sf::Keyboard::Delete:
				if (activeInputBox)
				{
					activeInputBox->setText("");
				}
				break;
			}
			if (promotion.first)
			{
				switch (this->ev.key.code)
				{
				case sf::Keyboard::F1:
					delete pieces[promotion.second]; 
					pieces[promotion.second] = new Queen(sf::Vector2i(promotion.second%8,promotion.second/8), !wTurn, boardPos);
					promotion.first = false;
					addMoveToPGN(Move(promotingPawnPos | ((63 - promotion.second) << 6) | (static_cast<int>(wTurn ? PieceType::bPawn : PieceType::wPawn) << 16) | (pieces[promotion.second] ? 1 : 0) << 20 | unsigned int(3) << 13 | static_cast<int>(wTurn ? PieceType::bQueen : PieceType::wQueen) << 21));
					b->makeMove(Move(promotingPawnPos | ((63 - promotion.second) << 6) | (static_cast<int>(wTurn?PieceType::bPawn:PieceType::wPawn) << 16) | (pieces[promotion.second] ? 1 : 0) << 20 | unsigned int(3) << 13 | static_cast<int>(wTurn ? PieceType::bQueen : PieceType::wQueen) << 21));
					if (wTurn)
					{
						turnBox->setText("White to move");
					}
					else
					{
						turnBox->setText("Black to move");
					}
					break;
				case sf::Keyboard::F2:
					delete pieces[promotion.second];
					pieces[promotion.second] = new Bishop(sf::Vector2i(promotion.second % 8, promotion.second / 8), !wTurn, boardPos);
					promotion.first = false;
					addMoveToPGN(Move(promotingPawnPos | ((63 - promotion.second) << 6) | (static_cast<int>(wTurn ? PieceType::bPawn : PieceType::wPawn) << 16) | (pieces[promotion.second] ? 1 : 0) << 20 | unsigned int(3) << 13 | static_cast<int>(wTurn ? PieceType::bBishop : PieceType::wBishop) << 21));
					b->makeMove(Move(promotingPawnPos | ((63 - promotion.second) << 6) | (static_cast<int>(wTurn ? PieceType::bPawn : PieceType::wPawn) << 16) | (pieces[promotion.second] ? 1 : 0) << 20 | unsigned int(3) << 13 | static_cast<int>(wTurn ? PieceType::bBishop : PieceType::wBishop) << 21));
					if (wTurn) 
					{
						turnBox->setText("White to move");
					}
					else
					{
						turnBox->setText("Black to move");
					}
					break;
				case sf::Keyboard::F3:
					delete pieces[promotion.second];
					pieces[promotion.second] = new Night(sf::Vector2i(promotion.second % 8, promotion.second / 8), !wTurn, boardPos);
					promotion.first = false;
					addMoveToPGN(Move(promotingPawnPos | ((63 - promotion.second) << 6) | (static_cast<int>(wTurn ? PieceType::bPawn : PieceType::wPawn) << 16) | (pieces[promotion.second] ? 1 : 0) << 20 | unsigned int(3) << 13 | static_cast<int>(wTurn ? PieceType::bNight : PieceType::wNight) << 21));
					b->makeMove(Move(promotingPawnPos | ((63 - promotion.second) << 6) | (static_cast<int>(wTurn ? PieceType::bPawn : PieceType::wPawn) << 16) | (pieces[promotion.second] ? 1 : 0) << 20 | unsigned int(3) << 13 | static_cast<int>(wTurn ? PieceType::bNight : PieceType::wNight) << 21));
					if (wTurn) 
					{
						turnBox->setText("White to move");
					}
					else 
					{
						turnBox->setText("Black to move");
					}
					break;
				case sf::Keyboard::F4:
					delete pieces[promotion.second];
					pieces[promotion.second] = new Rook(sf::Vector2i(promotion.second % 8, promotion.second / 8), !wTurn, boardPos);
					promotion.first = false;
					addMoveToPGN(Move(promotingPawnPos | ((63 - promotion.second) << 6) | (static_cast<int>(wTurn ? PieceType::bPawn : PieceType::wPawn) << 16) | (pieces[promotion.second] ? 1 : 0) << 20 | (unsigned int(3) << 13) | static_cast<int>(wTurn ? PieceType::bRook : PieceType::wRook) << 21));
					b->makeMove(Move(promotingPawnPos | ((63 - promotion.second) << 6) | (static_cast<int>(wTurn ? PieceType::bPawn : PieceType::wPawn) << 16) | (pieces[promotion.second] ? 1 : 0) << 20 | (unsigned int(3) << 13) | static_cast<int>(wTurn ? PieceType::bRook : PieceType::wRook) << 21));
					if (wTurn) 
					{
						turnBox->setText("White to move");
					}
					else 
					{
						turnBox->setText("Black to move");
					}
					break;
				}
			}
			break;
		case sf::Event::TextEntered:
			if (activeInputBox)
			{
				if (!activeInputBox->getActive())
					activeInputBox->toggleActive();
				activeInputBox->inputText(ev);
			}
			break;
		}
	}
}

void Game::update()
{
	this->pollEvents();
	if (positionLoadSuccess&&!calculationComplete&&!pieces.empty()&&!promotion.first)
	{
		b->calculateMoves();
		auto test = b->getLegalMoves(availableMoves);

		if (!test.first) {
			if (test.second == 0)
			{
				turnBox->setText("Stalemate");
				pgnBox->setText(pgnBox->getText() + "1/2-1/2");
			}
			else
			{
				turnBox->setText("Checkmate");
				pgnBox->setText(pgnBox->getText() + (wTurn ? "0-1" : "1-0"));
			}
		}
		else
		{
			if (bBot && !wTurn &&!botCalculation)
			{
				botCalculation = true;
				std::thread botMove([&]() {Search s;
				Move m = s.getBestMove(3, false, *b);
				makeBotMove(m); botCalculation = false; });
				botMove.detach();
			}
			if (wBot && wTurn && !botCalculation)
			{
				botCalculation = true;
				std::thread botMove([&]() {Search s;
				Move m = s.getBestMove(3, true, *b);
				makeBotMove(m); botCalculation = false; });
				botMove.detach();
			}
		}
		calculationComplete = true;
	}
	if (activePiece)
	{
		this->activePiece->updatePosLive(sf::Mouse::getPosition(*window));
	}
}

void Game::render()
{
	this->window->clear(sf::Color(51,51,51));
	this->board->render(window);
	this->loadPositionButton->draw(window);
	if (!moveInProgress)
	for (const auto& kv : pieces)
	{
		if(kv.second&&kv.second!=activePiece)
		kv.second->render(this->window);
	}
	if (activePiece)
		this->activePiece->render(this->window);
	std::for_each(boxes.begin(), boxes.end(), [&](BoxBase*& box) {box->draw(window); });
	this->window->display();
}

void Game::initVariables()
{
	this->mousePressed = false;
	this->activeInputBox = nullptr;
	this->activePiece = nullptr;
	this->boardPos = sf::Vector2i(20, 20);
	this->window = nullptr;
	enPassantSquare = -1;
	availableMoves = new bool* [64];
	for (int i = 0; i < 64; i++)
	{
		availableMoves[i] = new bool[64];
	}
}

bool Game::initWindow()
{
	this->videoMode = sf::VideoMode(1280, 710);
	if (this->window = new sf::RenderWindow(videoMode, "Szachy", sf::Style::Titlebar))
	{
		return true;
	}
	std::cerr << "Window initialization failed";
	return false;
}

void Game::initBoxes()
{
	sf::Vector2f boxSize = sf::Vector2f((videoMode.width - 60 - tSize * 8) / 2 - 10, 18);
	float firstBoxDistance = tSize * 8 + 40;
	float secondBoxDistance = (float)tSize * 8 + 40 + ((videoMode.width - 60 - tSize * 8) / 2) + 10;
	turnBox = new TextBox(sf::Vector2f(firstBoxDistance, 20), "-", boxSize);
	boxes.push_back(turnBox);
	castleBox = new TextBox(sf::Vector2f(secondBoxDistance, 20), "KQkq", boxSize);
	boxes.push_back(castleBox);
	loadPositionButton = new Button(sf::Vector2f(firstBoxDistance, 60), "Load position from the box below", sf::Vector2f(boxSize.x*2+20,boxSize.y*2));
	boxes.push_back(loadPositionButton);
	fenInputBox = new InputBox(sf::Vector2f(firstBoxDistance, 120), startPosition, sf::Vector2f(boxSize.x * 2 + 20, boxSize.y));
	boxes.push_back(fenInputBox);
	pgnBox = new TextBox(sf::Vector2f(firstBoxDistance, 160),"", sf::Vector2f(boxSize.x * 2 + 20, boxSize.y*10));
	boxes.push_back(pgnBox);
	exportPgn = new Button(sf::Vector2f(firstBoxDistance, 350), "Export PGN", sf::Vector2f(boxSize.x * 2 + 20, boxSize.y * 2));
	boxes.push_back(exportPgn);
	exportFen = new Button(sf::Vector2f(firstBoxDistance, 395), "Export FEN", sf::Vector2f(boxSize.x * 2 + 20, boxSize.y * 2));
	boxes.push_back(exportFen);
	multiplayer = new Button(sf::Vector2f(firstBoxDistance, 440), "Play against another human player", sf::Vector2f(boxSize.x * 2 + 20, boxSize.y * 4));
	boxes.push_back(multiplayer);
	aiWhite = new Button(sf::Vector2f(firstBoxDistance, 530), "Play against AI with white", sf::Vector2f(boxSize.x * 2 + 20, boxSize.y * 4));
	boxes.push_back(aiWhite);
	aiBlack = new Button(sf::Vector2f(firstBoxDistance, 620), "Play against AI with black", sf::Vector2f(boxSize.x * 2 + 20, boxSize.y * 4));
	boxes.push_back(aiBlack);
}

void Game::initBoard()
{
	this->board = new Board(this->boardPos);
	wTurn = true;
	wCastleK = true;
	bCastleK = true;
	wCastleQ = true;
	bCastleQ = true;
	positionLoadSuccess = loadPosition(startPosition);
	if (positionLoadSuccess)
	{
		b->calculateMoves();
		auto test = b->getLegalMoves(availableMoves);
		if (!test.first) {
			if (test.second == 0)
			{
				turnBox->setText("Stalemate");
				pgnBox->setText(pgnBox->getText() + "1/2-1/2");
			}
			else
			{
				turnBox->setText("Checkmate");
				pgnBox->setText(pgnBox->getText() + (wTurn ? "0-1" : "1-0"));
			}
		}
		if (wTurn ? !wBot : !bBot)
			calculationComplete = true;
	}
	else
	{
		for (auto& kv : pieces)
		{
			delete kv.second;
			kv.second = nullptr;
		}
	}
}

bool Game::loadPosition(std::string position)
{
	this->wCastleK = false;
	this->wCastleQ = false;
	this->bCastleK = false;
	this->bCastleQ = false;
	turnCount = 0;
	pgnBox->setText("");
	if (!pieces.empty())
	{
		for (auto& kv : pieces)
		{
			delete kv.second;
			kv.second = nullptr;
		}
	}
	pieces.erase(pieces.begin(),pieces.end());
	int x = 0;
	int y = 0;
	int rowCounter = 0;
	std::istringstream iss(position);
	std::vector<std::string> results((std::istream_iterator<std::string>(iss)),
		std::istream_iterator<std::string>());
	try
	{
		for (int i = 0; i < results[0].length(); i++)
		{
			switch (results[0][i])
			{
			case 'p':
			{
				Piece* pawn = new Pawn(sf::Vector2i(x++, y), false, boardPos);
				pieces[pawn->getPos().x + pawn->getPos().y * 8] = pawn;
				rowCounter++;
			}
			break;
			case 'r':
			{
				Piece* rook = new Rook(sf::Vector2i(x++, y), false, boardPos);
				pieces[rook->getPos().x + rook->getPos().y * 8] = rook;
				rowCounter++;
			}
			break;
			case 'n':
			{
				Piece* night = new Night(sf::Vector2i(x++, y), false, boardPos);
				pieces[night->getPos().x + night->getPos().y * 8] = night;
				rowCounter++;
			}
			break;
			case 'b':
			{
				Piece* bishop = new Bishop(sf::Vector2i(x++, y), false, boardPos);
				pieces[bishop->getPos().x + bishop->getPos().y * 8] = bishop;
				rowCounter++;
			}
			break;
			case 'q':
			{
				Piece* queen = new Queen(sf::Vector2i(x++, y), false, boardPos);
				pieces[queen->getPos().x + queen->getPos().y * 8] = queen;
				rowCounter++;
			}
			break;
			case 'k':
			{
				Piece* king = new King(sf::Vector2i(x++, y), false, boardPos);
				pieces[king->getPos().x + king->getPos().y * 8] = king;
				rowCounter++;
			}
			break;
			case 'P':
			{
				Piece* pawn = new Pawn(sf::Vector2i(x++, y), true, boardPos);
				pieces[pawn->getPos().x + pawn->getPos().y * 8] = pawn;
				rowCounter++;
			}
			break;
			case 'R':
			{
				Piece* rook = new Rook(sf::Vector2i(x++, y), true, boardPos);
				pieces[rook->getPos().x + rook->getPos().y * 8] = rook;
				rowCounter++;
			}
			break;
			case 'N':
			{
				Piece* night = new Night(sf::Vector2i(x++, y), true, boardPos);
				pieces[night->getPos().x + night->getPos().y * 8] = night;
				rowCounter++;
			}
			break;
			case 'B':
			{
				Piece* bishop = new Bishop(sf::Vector2i(x++, y), true, boardPos);
				pieces[bishop->getPos().x + bishop->getPos().y * 8] = bishop;
				rowCounter++;
			}
			break;
			case 'Q':
			{
				Piece* queen = new Queen(sf::Vector2i(x++, y), true, boardPos);
				pieces[queen->getPos().x + queen->getPos().y * 8] = queen;
				rowCounter++;
			}
			break;
			case 'K':
			{
				Piece* king = new King(sf::Vector2i(x++, y), true, boardPos);
				pieces[king->getPos().x + king->getPos().y * 8] = king;
				rowCounter++;
			}
			break;
			case '/':
				if (rowCounter == 8)
				{
					x = 0;
					y++;
					rowCounter = 0;
				}
				else
				{
					throw std::logic_error("Not enough pieces in row " + std::to_string(y));
				}
				break;
			default:
				if (std::isdigit(results[0][i]))
				{
					x += static_cast<int>(results[0][i]-48);
					rowCounter+= static_cast<int>(results[0][i]-48);
				}
				else
				{
					throw std::logic_error("Not a number character in FEN");
				}
				break;
			}
		}
		if (y != 7)
			throw std::logic_error("Not enough rows in FEN");
		if (results[1][0] == 'w')
		{
			this->wTurn = true;
		}
		else if (results[1][0] == 'b')
		{
			this->wTurn = false;
		}
		else
		{
			throw std::logic_error("Bad turn symbol");
		}
		if (results[2].length() > 4|| results[2].length() == 0)
		{
			throw std::logic_error("Bad castle string");
		}
		for (int i = 0; i < results[2].length(); i++)
		{
			switch (results[2][i])
			{
			case 'K':
				this->wCastleK = true;
				break;
			case 'Q':
				this->wCastleQ = true;
				break;
			case 'k':
				this->bCastleK = true;
				break;
			case 'q':
				this->bCastleQ = true;
				break;
			case '-' :
				this->wCastleK = false;
				this->wCastleQ = false;
				this->bCastleK = false;
				this->bCastleQ = false;
				break;
			default:
				throw std::logic_error("Bad castle string");
				break;
			}
		}
		this->castleBox->setText(results[2]);
		if (wTurn)
			turnBox->setText("White to move");
		else
			turnBox->setText("Black to move");
		b = new BitboardAlgo(pieces, wTurn, ((wCastleQ) | (wCastleK << 1) | (bCastleQ << 2) | (bCastleK << 3)));
		return true;
	}
	catch (std::logic_error e)
	{
		castleBox->setText(e.what());
		return false;
	}
}

void Game::exportPosition()
{
	std::string fen = "";
	for (size_t i = 0; i < 8; i++)
	{
		int space = 0;
		for (size_t j = 0; j < 8; j++)
		{
			if (pieces[i * 8 + j])
			{
				if (space)
				{
					fen += static_cast<char>(space + 48);
					space = 0;
				}
				switch (pieces[i*8+j]->getPieceType())
				{
				case PieceType::bPawn:
					fen += "p";
					break;
				case PieceType::bBishop:
					fen += "b";
					break;
				case PieceType::bNight:
					fen += "n";
					break;
				case PieceType::bRook:
					fen += "r";
					break;
				case PieceType::bQueen:
					fen += "q";
					break;
				case PieceType::bKing:
					fen += "k";
					break;
				case PieceType::wPawn:
					fen += "P";
					break;
				case PieceType::wBishop:
					fen += "B";
					break;
				case PieceType::wNight:
					fen += "N";
					break;
				case PieceType::wRook:
					fen += "R";
					break;
				case PieceType::wQueen:
					fen += "Q";
					break;
				case PieceType::wKing:
					fen += "K";
					break;
				}
			}
			else
			{
				space++;
			}
		}
		if (space)
			fen += static_cast<char>(space + 48);
		fen += "/";
	}
	fen.pop_back();
	fen += " ";
	if (wTurn)
		fen += "w ";
	else
		fen += "b ";
	if (wCastleK)
		fen += "K";
	if (wCastleQ)
		fen += "Q";
	if (bCastleK)
		fen += "k";
	if (bCastleQ)
		fen += "q";
	else if (!wCastleK && !wCastleQ && !bCastleK)
		fen += "-";
	fen += " ";
	if (enPassantSquare!=-1)
		fen += intToCoord(enPassantSquare);
	else
		fen += "-";
	sf::Clipboard cl;
	cl.setString(fen);
}

void Game::makeBotMove(Move m)
{
	moveInProgress = true;
	static sf::SoundBuffer buffer = []()
	{
		sf::SoundBuffer b;
		if (!b.loadFromFile("stuk.ogg"))
		{
			std::cerr << "Sound loading failed!\n";
		}
		return b;
	}();
	static sf::Sound sound = []() { sf::Sound s; s.setBuffer(buffer); return s; }();
	addMoveToPGN(m);
	b->makeMove(m);
	unsigned int data = m.getData();
	int start = 63 - (data & unsigned int(0x3f));
	int end = 63 - ((data & unsigned int(0xfc0)) >> 6);
	bool capture = (data & unsigned int(0x100000)) >> 20;
	short int specialMove = (data & unsigned int(0xE000)) >> 13;
	PieceType movedPiece = static_cast <PieceType>((data & unsigned int(0xF0000)) >> 16);
	PieceType promotionTarget = static_cast<PieceType>(((data & unsigned int(0x1E00000)) >> 21));
	bool color = static_cast<int>(movedPiece) / 6;
	if (movedPiece == PieceType::bPawn || movedPiece == PieceType::wPawn)
	{
		static_cast<Pawn*>(pieces[start])->moved = true;
		if (end == enPassantSquare)
		{
			delete pieces[enPassantSquare + (wTurn ? BoardDirections[6] : BoardDirections[1])];
			pieces.erase(enPassantSquare + (wTurn ? BoardDirections[6] : BoardDirections[1]));
			enPassantSquare = -1;
		}
		else if (end == (start + (wTurn ? BoardDirections[1] * 2 : BoardDirections[6] * 2)))
		{
			enPassantSquare = start + (wTurn ? BoardDirections[1] : BoardDirections[6]);
		}
		else
		{
			enPassantSquare = -1;
		}
		if (specialMove==3)
		{
			switch (promotionTarget)
			{
			case PieceType::wBishop:
			case PieceType::bBishop:
				delete pieces[end];
				pieces[end] = new Bishop(sf::Vector2i(end % 8, end / 8), wTurn, boardPos);
				
				if (wTurn)
				{
					turnBox->setText("White to move");
				}
				else
				{
					turnBox->setText("Black to move");
				}
				break;
			case PieceType::wNight:
			case PieceType::bNight:
				delete pieces[end];
				pieces[end] = new Night(sf::Vector2i(end % 8, end / 8), wTurn, boardPos);
				
				if (wTurn)
				{
					turnBox->setText("White to move");
				}
				else
				{
					turnBox->setText("Black to move");
				}
				break;
			case PieceType::wRook:
			case PieceType::bRook:
				delete pieces[end];
				pieces[end] = new Rook(sf::Vector2i(end % 8, end / 8), wTurn, boardPos);
				
				if (wTurn)
				{
					turnBox->setText("White to move");
				}
				else
				{
					turnBox->setText("Black to move");
				}
				break;
			case PieceType::wQueen:
			case PieceType::bQueen:
				delete pieces[end];
				pieces[end] = new Queen(sf::Vector2i(end % 8, end / 8), wTurn, boardPos);
				
				if (wTurn)
				{
					turnBox->setText("White to move");
				}
				else
				{
					turnBox->setText("Black to move");
				}
				break;
			}
		}
	}
	else if (movedPiece == PieceType::bKing || movedPiece == PieceType::wKing)
	{
		if (wTurn)
		{
			if (wCastleK)
			{
				if (end == 62)
				{
					pieces[61] = pieces[63];
					pieces.erase(63);
					pieces[61]->updatePos(boardPos, 61);
				}
			}
			if (wCastleQ)
			{
				if (end == 58)
				{
					pieces[59] = pieces[56];
					pieces.erase(56);
					pieces[59]->updatePos(boardPos, 59);
				}
			}
			wCastleK = false;
			wCastleQ = false;
		}
		else
		{
			if (bCastleK)
			{
				if (end == 6)
				{
					pieces[5] = pieces[7];
					pieces.erase(7);
					pieces[5]->updatePos(boardPos, 5);
				}
			}
			if (bCastleQ)
			{
				if (end == 2)
				{
					pieces[3] = pieces[0];
					pieces.erase(0);
					pieces[3]->updatePos(boardPos, 3);
				}
			}
			bCastleK = false;
			bCastleQ = false;
		}

	}
	if (wTurn)
	{
		if (start == 63 || end == 63)
		{
			wCastleK = false;
		}
		else if (start == 56 || end == 56)
		{
			wCastleQ = false;
		}
	}
	else
	{
		if (start == 7 || end == 7)
		{
			wCastleK = false;
		}
		else if (start == 0 || end == 0)
		{
			wCastleQ = false;
		}
	}
	calculationComplete = false;
	sound.play();
	delete pieces[end];
	pieces[end] = pieces[start];
	pieces[end]->updatePos(boardPos, end);
	pieces.erase(start);
	if (wTurn)
	{
		wTurn = false;
	}
	else
	{
		wTurn = true;
	}
	int castleFlag[4] = { wCastleK,wCastleQ,bCastleK,bCastleQ };
	std::string castleString = "";
	for (int i = 0; i < 4; i++)
	{
		switch (i)
		{
		case 0:
			if (castleFlag[i])
				castleString += "K";
			break;
		case 1:
			if (castleFlag[i])
				castleString += "Q";
			break;
		case 2:
			if (castleFlag[i])
				castleString += "k";
			break;
		case 3:
			if (castleFlag[i])
				castleString += "q";
			break;
		}
	}
	this->castleBox->setText(castleString);
	if (wTurn)
		turnBox->setText("White to move");
	else
		turnBox->setText("Black to move");
	b->calculateMoves();
	auto test = b->getLegalMoves(availableMoves);
	if (!test.first) {
		if (test.second == 0)
		{
			turnBox->setText("Stalemate");
			pgnBox->setText(pgnBox->getText() + "1/2-1/2");
		}
		else
		{
			turnBox->setText("Checkmate");
			pgnBox->setText(pgnBox->getText() + (wTurn ? "0-1" : "1-0"));
		}
	}
	moveInProgress = false;
}


void Game::addMoveToPGN(Move m)
{
	std::string lastPGN = pgnBox->getText();
	unsigned int data = m.getData();
	unsigned int start(63-((data & unsigned int(0x3f))));
	unsigned int end(63-((data & unsigned int(0xfc0)) >> 6));
	bool capture = (data & unsigned int(0x100000)) >> 20;
	short int specialMove = (data & unsigned int(0xE000)) >> 13;
	PieceType movedPiece = static_cast <PieceType>((data & unsigned int(0xF0000)) >> 16);
	PieceType promotionTarget = static_cast<PieceType>(((data & unsigned int(0x1E00000)) >> 21));
	bool color = static_cast<int>(movedPiece) / 6;
	std::string endCoord = intToCoord(end);
	std::string startCoord = intToCoord(start);
	static int newLineCount = 0;
	if (wTurn)
	{
		turnCount++;
		lastPGN += (std::to_string(turnCount) + ". ");
		newLineCount += 3;
	}
	if (newLineCount > 85)
	{
		lastPGN += "\n";
		newLineCount = 0;
	}
	switch (specialMove)
	{
	case 0:
	case 4:
		switch (movedPiece)
		{
		case PieceType::bPawn:
		case PieceType::wPawn:
			if (capture)
			{
				std::string s = "";
				s += startCoord[0];
				s += "x";
				s += (endCoord + " ");
				lastPGN += s;
				newLineCount += 5;
			}
			else
			{
				lastPGN += (endCoord + " ");
				newLineCount += 2;
			}
			break;
		case PieceType::wRook:
		case PieceType::bRook:
			if (capture)
			{
				std::string s = "R";
				s += startCoord;
				s += "x";
				s += (endCoord + " ");
				lastPGN += s;
				newLineCount += 7;
			}
			else
			{
				std::string s = "R";
				s += startCoord;
				lastPGN += (s + endCoord + " ");
				newLineCount += 6;
			}
			break;
		case PieceType::bQueen:
		case PieceType::wQueen:
			if (capture)
			{
				std::string s = "Q";
				s += startCoord;
				s += "x";
				s += (endCoord + " ");
				lastPGN += s;
				newLineCount += 7;
			}
			else
			{
				std::string s = "Q";
				s += startCoord;
				lastPGN += (s + endCoord + " ");
				newLineCount += 6;
			}
			break;
		case PieceType::wBishop:
		case PieceType::bBishop:
			if (capture)
			{
				std::string s = "B";
				s += startCoord;
				s += "x";
				s += (endCoord + " ");
				lastPGN += s;
				newLineCount += 7;
			}
			else
			{
				std::string s = "B";
				s += startCoord;
				lastPGN += (s + endCoord + " ");
				newLineCount += 6;
			}
			break;
		case PieceType::wNight:
		case PieceType::bNight:
			if (capture)
			{
				std::string s = "N";
				s += startCoord;
				s += "x";
				s += (endCoord + " ");
				lastPGN += s;
				newLineCount += 7;
			}
			else
			{
				std::string s = "N";
				s += startCoord;
				lastPGN += (s + endCoord + " ");
				newLineCount += 6;
			}
			break;
		case PieceType::bKing:
		case PieceType::wKing:
			if (capture)
			{
				std::string s = "K";
				s += startCoord;
				s += "x";
				s += (endCoord + " ");
				lastPGN += s;
				newLineCount += 7;
			}
			else
			{
				std::string s = "K";
				s += startCoord;
				lastPGN += (s + endCoord + " ");
				newLineCount += 6;
			}
			break;
		}
		break;
	case 1:
			lastPGN += (intToCoord(start)[0] + intToCoord(end) + "e.p. ");
			break;
	case 2:
	{
		short int castleType = (data & unsigned int(0x6000000)) >> 25;
		switch (castleType)
		{
		case 0:
		case 2:
			lastPGN += "O-O-O ";
			break;
		case 1:
		case 3:
			lastPGN += "O-O ";
			break;
		}
		break;
	}
	case 3:
		switch (promotionTarget)
		{
		case PieceType::bBishop:
			lastPGN += intToCoord(end) + "=b ";
			break;
		case PieceType::bNight:
			lastPGN += intToCoord(end) + "=n ";
			break;
		case PieceType::bRook:
			lastPGN += intToCoord(end) + "=r ";
			break;
		case PieceType::bQueen:
			lastPGN += intToCoord(end) + "=q ";
			break;
		case PieceType::wBishop:
			lastPGN += intToCoord(end) + "=B ";
			break;
		case PieceType::wNight:
			lastPGN += intToCoord(end) + "=N ";
			break;
		case PieceType::wRook:
			lastPGN += intToCoord(end) + "=R ";
			break;
		case PieceType::wQueen:
			lastPGN += intToCoord(end) + "=Q ";
			break;
		}
		break;
	}
	pgnBox->setText(lastPGN);
}

std::string Game::intToCoord(int tile)
{
	int x = 7-(tile % 8);
	int y = 7-(tile / 8)+1;
	std::string pos = "";
	switch (x)
	{
	case 0:
		pos += "h";
		break;
	case 1:
		pos += "g";
		break;
	case 2:
		pos += "f";
		break;
	case 3:
		pos += "e";
		break;
	case 4:
		pos += "d";
		break;
	case 5:
		pos += "c";
		break;
	case 6:
		pos += "b";
		break;
	case 7:
		pos += "a";
		break;
	}
	pos += std::to_string(y);
	return pos;
}

const bool Game::isRunning() const
{
	return this->window->isOpen();
}