#pragma once
#include "BitBoard.h"
#include "Check.h"
#include "Piece.h"

#include "Pawn.h"
#include "King.h"
#include "Queen.h"
#include "Rook.h"
#include "Night.h"
#include "Bishop.h"
#include <thread>
#include <list>
#include <mutex>

typedef unsigned long long int U64;

class MoveCalcAlgo
{
public:
	MoveCalcAlgo(std::map<unsigned int, Piece*> _pieces, bool _wTurn, bool _wCastleQ, bool _wCastleK, bool _bCastleQ, bool _bCastleK,int _enP)
		:pieces(_pieces), wTurn(_wTurn), wCastleK(_wCastleK), wCastleQ(_wCastleQ), bCastleK(_bCastleK), bCastleQ(_bCastleQ),enPassantSquare(_enP),anyMove(false)
	{
		check.amount = 0;
		check.attackingPiece = -1;
		check.direction = -1;
		check.kingPos = -1;
	}
	void calculateMoves(int startIndex, int endIndex);
	//Move calculating
	bool opponentAttacks[64];
	bool coverTiles[64];
	bool** availableMoves;
	Check check;
	
	void calculateAttackParent();
	std::pair<bool,int> calculateMovesParent(bool** _availableMoves);
private:
	std::map<unsigned int, Piece*> pieces;
	void calculateAttack(int startIndex, int endIndex);
	bool wTurn;
	
	bool wCastleQ;
	bool bCastleQ;
	bool wCastleK;
	bool bCastleK;
	
	bool anyMove;

	int enPassantSquare;

	const int BoardDirections[8] = { -9,-8,-7,-1,1,7,8,9 };

	void calculateLinearMoves(int startingTile, PieceType pieceType);
	void calculatePawnMovements(int startingTile);
	void calculateNightMovements(int startingTile);
	void calculateKingMoves(int startingTile);
	//Attack calculation
	void calculateLinearAttack(int startingTile, PieceType pieceType);
	void calculatePawnAttack(int startingTile);
	void calculateNightAttack(int startingTile);
	void calculateKingAttack(int startingTile);
	void calculatePin(int startingTile, int direction, int steps);
};

