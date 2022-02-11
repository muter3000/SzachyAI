#pragma once
#include "Board.h"
class Check
{
public:
	int amount;
	int attackingPiece;
	int direction;
	int kingPos;

	bool* calculateCoverPositions(bool v[64])
	{
		for (int i = 0; attackingPiece + BoardDirections[direction] * i != kingPos;i++)
		{
			v[attackingPiece + BoardDirections[direction] * i]=true;
		}
		return v;
	}
private:
	const int BoardDirections[8] = { -9,-8,-7,-1,1,7,8,9 };
};

