#pragma once
#include "Piece.h"
#ifndef NOTDEBUG
#include <sstream>
#include <iostream>
#endif
typedef unsigned long long int U64;
namespace Castles
{
	const U64 q = (U64(1) << 63) ^ (U64(1) << 60);
	const U64 K = (U64(1)) ^ (U64(1) << 2);
	const U64 k = (U64(1) << 56) ^ (U64(1) << 58);
	const U64 Q = (U64(1) << 7) ^ (U64(1) << 4);
}
const U64 bottomRight = U64(1)<<0;
const U64 bottomLeft = U64(1)<<7;
const U64 topLeft = U64(1)<<63;
const U64 topRight = U64(1)<<56;
class BitBoard
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
#endif
	friend class BitboardAlgo;
	friend class Search;
	U64 enPassantSquare;
	short unsigned int castlingRights; // 0-Q 1-K 2-q 3-k

	struct LastMove
	{
		U64 start;
		U64 end;
		PieceType movedPiece;
		short int moveFlag; //0-standard 1-capture 2-promo 3-en passant 4-promoCapture 5-Q 6-K 7-q 8-k
		short unsigned int castlingRigths;
		U64 enPassantSquare;
		PieceType capturedPiece;
		PieceType promotionTarget;
	};

	U64 wPawns;
	U64 wNights;
	U64 wBishops;
	U64 wRooks;
	U64 wQueens;
	U64 wKing;

	U64 bPawns;
	U64 bNights;
	U64 bBishops;
	U64 bRooks;
	U64 bQueens;
	U64 bKing;

	U64 wPieces;
	U64 bPieces;

	U64 allPieces;

	int turnCount;
	LastMove movesInGame[256];

	PieceType getWhitePieceOnTile(U64 tile);
	PieceType getBlackPieceOnTile(U64 tile);

public:
	BitBoard(std::map<unsigned int, Piece*> piecesMap, int _castlingRigths);
	BitBoard();

	void makeMove(Move m);
	void unMakeMove();
};

