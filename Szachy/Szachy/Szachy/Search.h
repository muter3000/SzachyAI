#pragma once
#include "Move.h"
#include "BitboardAlgo.h"
#include <vector>

namespace PieceValue
{
	const unsigned short int Pawn = 1;
	const unsigned short int BishopNight = 3;
	const unsigned short int Rook = 5;
	const unsigned short int Queen = 9;
}

class Search
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

	std::vector<std::pair<Move, float>> bestMoveOrder;

	inline static const U64 debruijn64 = U64(0x03f79d71b4cb0a89);

	inline static const int index64[64] = {
		0, 47,  1, 56, 48, 27,  2, 60,
	   57, 49, 41, 37, 28, 16,  3, 61,
	   54, 58, 35, 52, 50, 42, 21, 44,
	   38, 32, 29, 23, 17, 11,  4, 62,
	   46, 55, 26, 59, 40, 36, 15, 53,
	   34, 51, 20, 43, 31, 22, 10, 45,
	   25, 39, 14, 33, 19, 30,  9, 24,
	   13, 18,  8, 12,  7,  6,  5, 63
	};

	inline static const unsigned short int directions[4] = { 7,9,1,8 };

	int bitScanForward(U64 bb) {
		if (!bb == 0)
			return index64[((bb ^ (bb - 1)) * debruijn64) >> 58];
		else
			return 0;
	}

	std::string intToCoord(int tile);
	float evaluate(BitboardAlgo& b,int depthLeft);
	float alphaBetaMax(BitboardAlgo& b, float alpha, float beta, int depthleft);
	float alphaBetaMin(BitboardAlgo& b, float alpha, float beta, int depthleft);
public:
	int perf(int depth,BitboardAlgo& b);
	void divide(int depth, BitboardAlgo& b);
	Move getBestMove(int depth, bool wTurn, BitboardAlgo& b);
};

