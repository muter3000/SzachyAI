#pragma once
#include "BitBoard.h"
#ifndef NOTDEBUG
#include <sstream>
#include <iostream>
#endif

const U64 KCheck = U64(0x6);
const U64 QCheck = U64(0x70);

const U64 kCheck = U64(0x600000000000000);
const U64 qCheck = U64(0x7000000000000000);

const U64 notHFile = U64(0xfefefefefefefefe);
const U64 notAFile = U64(0x7f7f7f7f7f7f7f7f);
const U64 notGHFile = U64(0xFCFCFCFCFCFCFCFC);
const U64 notABFile = U64(0x3f3f3f3f3f3f3f3f);

const U64 not8Rank = U64(0x00ffffffffffffff);
const U64 not1Rank = U64(0xffffffffffffff00);

const U64 debruijn64 = U64(0x03f79d71b4cb0a89);

const int index64[64] = {
	0, 47,  1, 56, 48, 27,  2, 60,
   57, 49, 41, 37, 28, 16,  3, 61,
   54, 58, 35, 52, 50, 42, 21, 44,
   38, 32, 29, 23, 17, 11,  4, 62,
   46, 55, 26, 59, 40, 36, 15, 53,
   34, 51, 20, 43, 31, 22, 10, 45,
   25, 39, 14, 33, 19, 30,  9, 24,
   13, 18,  8, 12,  7,  6,  5, 63
};

const unsigned short int directions[4] = { 7,9,1,8 };

class BitboardAlgo
{
private:

#ifndef NOTDEBUG
	void debug_printU64(U64 toPrint)
	{
		int n = 0;
		U64 f = toPrint;
		std::stringstream a;
		for (size_t i = 0; i < 64; i++)
		{
			a << (f & 1);
			f >>= 1;
			n++;
			if (n == 8)
			{
				a << "\n";
				n = 0;
			}
		}
		for (int i = a.str().length() - 1; i >= 0; i--)
		{
			std::cout << a.str()[i];
		}
		std::cout << "\n";
	};
#endif // !NOTDEBUG

	friend class Search;


	int bitScanForward(U64 bb) {
		if (!bb == 0)
			return index64[((bb ^ (bb - 1)) * debruijn64) >> 58];
		else
			return 0;
	}

	U64 opAttack;
	U64 coverTiles;
	U64 captureTile;

	bool wTurn;
	BitBoard bitBoard;

	Move* pseudoLegalMoves;
	Move* legalMoves;
	unsigned short int pseudoLegalMoveIndex = 0;
	unsigned short int legalMoveIndex = 0;

	U64 calculateQueenMoves(unsigned int pos, U64 samePieces);
	U64 calculateRookMoves(unsigned int pos, U64 samePieces);
	U64 calculateBishopMoves(unsigned int pos, U64 samePieces);

	void calculateWhitePawnMoves();
	void calculateWhiteNightMoves();
	void calculateWhiteKingMoves();
	void calculateWhiteSlidingPiecesMoves();
	void calculateWhiteCastles();

	bool calculateAttackOnWhiteTile(const U64 tile, BitBoard& b);

	void calculateBlackPawnMoves();
	void calculateBlackNightMoves();
	void calculateBlackKingMoves();
	void calculateBlackSlidingPiecesMoves();
	void calculateBlackCastles();

	bool calculateAttackOnBlackTile(const U64 tile, BitBoard& b);

	void makePromotionMoves(unsigned int startSquare, unsigned int targetSquare,unsigned int capture);
public:

	BitboardAlgo()
		:bitBoard(), opAttack(0), captureTile(0), coverTiles(0), wTurn(true)
	{
		pseudoLegalMoves = new Move[256];
		legalMoves = new Move[256];
	}

	BitboardAlgo(std::map<unsigned int, Piece*> pieces, bool _wTurn, int _castlingRigths)
		:bitBoard(pieces,_castlingRigths), opAttack(0), captureTile(0), coverTiles(0), wTurn(_wTurn)
	{
		pseudoLegalMoves = new Move[256];
		legalMoves = new Move[256];
	}
	~BitboardAlgo()
	{
		delete[] legalMoves;
		delete[] pseudoLegalMoves;
	}


	std::pair<bool, int> getLegalMoves(bool** availableMoves);
	void calculateMoves();

	void makeMove(Move m)
	{
		opAttack = 0;
		bitBoard.makeMove(m);
		wTurn = wTurn ? false : true;
	}
	void unmakeMove()
	{
		bitBoard.unMakeMove();
		wTurn = wTurn ? false : true;
	}
	U64 getAllPieces() { return bitBoard.allPieces; }
	U64 getOpAttacks() { return	opAttack; }

	void printLegalMoveIndex();
};

