#pragma once

const unsigned int tSize = 84;
//rnbqkbnr/pppppppp/8/8/8/8/PPPPPPPP/RNBQKBNR
const std::string startPosition = "rnbqkbnr/pppppppp/8/8/8/8/PPPPPPPP/RNBQKBNR w KQkq - 0 1";

enum class PieceType
{
	bPawn=0,
	bBishop,
	bNight,
	bRook,
	bQueen,
	bKing,
	wPawn,
	wBishop,
	wNight,
	wRook,
	wQueen,
	wKing
};
