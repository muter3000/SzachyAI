#include "MoveCalcAlgo.h"

void MoveCalcAlgo::calculateMoves(int startIndex, int endIndex)
{
	for (int i = 0; i < 64; i++)
	{
		for (int j = 0; j < 64; j++)
		{
			availableMoves[i][j] = false;
		}
	}
	for (int i = startIndex; i < endIndex; i++)
	{
		if (pieces[i] && pieces[i]->getPieceColor() == wTurn)
		{
			switch (pieces[i]->getPieceType())
			{
			case PieceType::bPawn:
			case PieceType::wPawn:
				calculatePawnMovements(i);
				break;
			case PieceType::wRook:
			case PieceType::bRook:
			case PieceType::bQueen:
			case PieceType::wQueen:
			case PieceType::wBishop:
			case PieceType::bBishop:
				calculateLinearMoves(i, pieces[i]->getPieceType());
				break;
			case PieceType::wNight:
			case PieceType::bNight:
				calculateNightMovements(i);
				break;
			case PieceType::bKing:
			case PieceType::wKing:
				calculateKingMoves(i);
				break;
			}
		}
	}
}

void MoveCalcAlgo::calculateLinearMoves(int startingTile, PieceType pieceType)
{
	auto p = pieces[startingTile];
	auto pieceColor = wTurn;
	std::pair<bool, int> pin = p->getPinned();
	switch (pieceType)
	{
	case PieceType::wBishop:
	case PieceType::bBishop:
	{
		int x = startingTile % 8;
		int y = startingTile / 8;
		int distanceToEdge[8] = { std::min<int>(x,y),y,std::min<int>(7 - x,y),x,7 - x,std::min<int>(x,7 - y),7 - y,std::min<int>(7 - x,7 - y) };
		std::vector<int> directionChoice{ 0,2,5,7 };
		if (pin.first)
		{
			switch (pin.second)
			{
			case 0:
			case 7:
				directionChoice = std::vector<int>{ 0, 7 };
				break;
			case 1:
			case 6:
				directionChoice.clear();
				break;
			case 2:
			case 5:
				directionChoice = std::vector<int>{ 2, 5 };
				break;
			case 3:
			case 4:
				directionChoice.clear();
				break;
			}
		}
		for (int i = 0; i < directionChoice.size(); i++)
		{
			for (int j = 1; j <= distanceToEdge[directionChoice[i]]; j++)
			{
				if (pieces[startingTile + BoardDirections[directionChoice[i]] * j])
				{
					if (pieces[startingTile + BoardDirections[directionChoice[i]] * j]->getPieceColor() == pieceColor)
					{
						break;
					}
					else
					{
						if (check.amount == 0 || coverTiles[startingTile + BoardDirections[directionChoice[i]] * j]) {
							availableMoves[startingTile][startingTile + BoardDirections[directionChoice[i]] * j] = true;
							anyMove = true;
						}
						break;
					}
				}
				else
				{
					if (check.amount == 0 || coverTiles[startingTile + BoardDirections[directionChoice[i]] * j]) {
						availableMoves[startingTile][startingTile + BoardDirections[directionChoice[i]] * j] = true;
						anyMove = true;
					}
				}
			}
		}
		break;
	}
	case PieceType::bRook:
	case PieceType::wRook:
	{
		int x = startingTile % 8;
		int y = startingTile / 8;
		int distanceToEdge[8] = { std::min<int>(x,y),y,std::min<int>(7 - x,y),x,7 - x,std::min<int>(x,7 - y),7 - y,std::min<int>(7 - x,7 - y) };
		std::vector<int> directionChoice{ 1,3,4,6 };
		if (pin.first)
		{
			switch (pin.second)
			{
			case 0:
			case 7:
				directionChoice.clear();
				break;
			case 1:
			case 6:
				directionChoice = std::vector<int>{ 1, 6 };
				break;
			case 2:
			case 5:
				directionChoice.clear();
				break;
			case 3:
			case 4:
				directionChoice = std::vector<int>{ 3, 4 };
				break;
			}
		}
		for (int i = 0; i < directionChoice.size(); i++)
		{
			for (int j = 1; j <= distanceToEdge[directionChoice[i]]; j++)
			{
				if (pieces[startingTile + BoardDirections[directionChoice[i]] * j])
				{
					if (pieces[startingTile + BoardDirections[directionChoice[i]] * j]->getPieceColor() == wTurn)
					{
						break;
					}
					else
					{
						if (check.amount == 0 || coverTiles[startingTile + BoardDirections[directionChoice[i]] * j]) {
							availableMoves[startingTile][startingTile + BoardDirections[directionChoice[i]] * j] = true;
							anyMove = true;
						}
						break;
					}
				}
				else
				{
					if (check.amount == 0 || coverTiles[startingTile + BoardDirections[directionChoice[i]] * j]) {
						availableMoves[startingTile][startingTile + BoardDirections[directionChoice[i]] * j] = true;
						anyMove = true;
					}
				}
			}
		}
		break;
	}
	case PieceType::bQueen:
	case PieceType::wQueen:
	{
		int x = startingTile % 8;
		int y = startingTile / 8;
		std::vector<int> directionChoice{ 0,1,2,3,4,5,6,7 };
		int distanceToEdge[8] = { std::min<int>(x,y),y,std::min<int>(7 - x,y),x,7 - x,std::min<int>(x,7 - y),7 - y,std::min<int>(7 - x,7 - y) };
		if (pin.first)
		{
			switch (pin.second)
			{
			case 0:
			case 7:
				directionChoice = std::vector<int>{ 0, 7 };
				break;
			case 1:
			case 6:
				directionChoice = std::vector<int>{ 1, 6 };
				break;
			case 2:
			case 5:
				directionChoice = std::vector<int>{ 2, 5 };
				break;
			case 3:
			case 4:
				directionChoice = std::vector<int>{ 3, 4 };
				break;
			}
		}
		for (int i = 0; i < directionChoice.size(); i++)
		{
			for (int j = 1; j <= distanceToEdge[directionChoice[i]]; j++)
			{
				if (pieces[startingTile + BoardDirections[directionChoice[i]] * j])
				{
					if (pieces[startingTile + BoardDirections[directionChoice[i]] * j]->getPieceColor() == wTurn)
					{
						break;
					}
					else
					{
						if (check.amount == 0 || coverTiles[startingTile + BoardDirections[directionChoice[i]] * j])
						{
							availableMoves[startingTile][startingTile + BoardDirections[directionChoice[i]] * j] = true;
							anyMove = true;
						}
						break;
					}
				}
				else
				{
					if (check.amount == 0 || coverTiles[startingTile + BoardDirections[directionChoice[i]] * j]) {
						availableMoves[startingTile][startingTile + BoardDirections[directionChoice[i]] * j] = true;
						anyMove = true;
					}
				}
			}
		}
		break;
	}
	}
}

void MoveCalcAlgo::calculatePawnMovements(int startingTile)
{
	auto p = pieces[startingTile];
	auto pieceColor = wTurn;
	int x = startingTile % 8;
	int y = startingTile / 8;
	if (pieceColor)
	{
		if (!p->getPinned().first)
		{
			if (y > 0 && !pieces[startingTile + BoardDirections[1]])
			{
				if (check.amount == 0 || coverTiles[startingTile + BoardDirections[1]])
				{
					availableMoves[startingTile][startingTile + BoardDirections[1]] = true;
					anyMove = true;
				}
			}
			if (!static_cast<Pawn*>(p)->moved && !pieces[startingTile + BoardDirections[1]] && !pieces[startingTile + BoardDirections[1] * 2])
			{
				if (check.amount == 0 || coverTiles[startingTile + BoardDirections[1] * 2])
				{
					availableMoves[startingTile][startingTile + BoardDirections[1] * 2] = true;
					anyMove = true;
				}
			}
			if (y > 0 && x > 0 && (pieces[startingTile + BoardDirections[0]] && pieces[startingTile + BoardDirections[0]]->getPieceColor() == false) || startingTile + BoardDirections[0] == enPassantSquare)
			{
				if (check.amount == 0 || coverTiles[startingTile + BoardDirections[0]])
				{
					availableMoves[startingTile][startingTile + BoardDirections[0]] = true;
					anyMove = true;
				}
			}
			if (y > 0 && 7 - x > 0 && (pieces[startingTile + BoardDirections[2]] && pieces[startingTile + BoardDirections[2]]->getPieceColor() == false) || startingTile + BoardDirections[2] == enPassantSquare)
			{
				if (check.amount == 0 || coverTiles[startingTile + BoardDirections[2]])
				{
					availableMoves[startingTile][startingTile + BoardDirections[2]] = true;
					anyMove = true;
				}
			}
		}
		else
		{
			switch (p->getPinned().second)
			{
			case 7:
				if (y > 0 && x > 0 && (pieces[startingTile + BoardDirections[0]] && pieces[startingTile + BoardDirections[0]]->getPieceColor() == false) || startingTile + BoardDirections[0] == enPassantSquare)
				{
					if (check.amount == 0 || coverTiles[startingTile + BoardDirections[0]])
					{
						availableMoves[startingTile][startingTile + BoardDirections[0]] = true;
						anyMove = true;
					}
				}
				break;
			case 6:
				if (y > 0 && !pieces[startingTile + BoardDirections[1]])
				{
					if (check.amount == 0 || coverTiles[startingTile + BoardDirections[1]])
					{
						availableMoves[startingTile][startingTile + BoardDirections[1]] = true;
						anyMove = true;
					}
				}
				if (!static_cast<Pawn*>(p)->moved && !pieces[startingTile + BoardDirections[1]] && !pieces[startingTile + BoardDirections[1] * 2])
				{
					if (check.amount == 0 || coverTiles[startingTile + BoardDirections[1] * 2])
					{
						availableMoves[startingTile][startingTile + BoardDirections[1] * 2] = true;
						anyMove = true;
					}
				}
			case 5:
				if (y > 0 && 7 - x > 0 && (pieces[startingTile + BoardDirections[2]] && pieces[startingTile + BoardDirections[2]]->getPieceColor() == false) || startingTile + BoardDirections[0] == enPassantSquare)
				{
					if (check.amount == 0 || coverTiles[startingTile + BoardDirections[2]])
					{
						availableMoves[startingTile][startingTile + BoardDirections[2]] = true;
						anyMove = true;
					}
				}
				break;
			default:
				break;
			}
		}
	}
	else
	{
		if (!p->getPinned().first)
		{
			if (7 - y > 0 && !pieces[startingTile + BoardDirections[6]])
			{
				if (check.amount == 0 || coverTiles[startingTile + BoardDirections[6]]) {
					availableMoves[startingTile][startingTile + BoardDirections[6]] = true;
					anyMove = true;
				}
			}
			if (!static_cast<Pawn*>(p)->moved && !pieces[startingTile + BoardDirections[6]] && !pieces[startingTile + BoardDirections[6] * 2])
			{
				if (check.amount == 0 || coverTiles[startingTile + BoardDirections[6] * 2]) {
					availableMoves[startingTile][startingTile + BoardDirections[6] * 2] = true;
					anyMove = true;
				}
			}
			if (7 - y > 0 && x > 0 && (pieces[startingTile + BoardDirections[5]] && pieces[startingTile + BoardDirections[5]]->getPieceColor() != false) || startingTile + BoardDirections[6] == enPassantSquare)
			{
				if (check.amount == 0 || coverTiles[startingTile + BoardDirections[5]]) {
					availableMoves[startingTile][startingTile + BoardDirections[5]] = true;
					anyMove = true;
				}
			}
			if (7 - y > 0 && 7 - x > 0 && (pieces[startingTile + BoardDirections[7]] && pieces[startingTile + BoardDirections[7]]->getPieceColor() != false) || startingTile + BoardDirections[7] == enPassantSquare)
			{
				if (check.amount == 0 || coverTiles[startingTile + BoardDirections[7]]) {
					availableMoves[startingTile][startingTile + BoardDirections[7]] = true;
					anyMove = true;
				}
			}
		}
		else
		{
			switch (p->getPinned().second)
			{
			case 2:
				if (7 - y > 0 && x > 0 && (pieces[startingTile + BoardDirections[5]] && pieces[startingTile + BoardDirections[5]]->getPieceColor() != false) || startingTile + BoardDirections[5] == enPassantSquare)
				{
					if (check.amount == 0 || coverTiles[startingTile + BoardDirections[5]]) {
						availableMoves[startingTile][startingTile + BoardDirections[5]] = true;
						anyMove = true;
					}
				}
				break;
			case 1:
				if (7 - y > 0 && !pieces[startingTile + BoardDirections[6]])
				{
					if (check.amount == 0 || coverTiles[startingTile + BoardDirections[7]]) {
						availableMoves[startingTile][startingTile + BoardDirections[6]] = true;
						anyMove = true;
					}
				}
				if (!static_cast<Pawn*>(p)->moved && !pieces[startingTile + BoardDirections[6]] && !pieces[startingTile + BoardDirections[6] * 2])
				{
					if (check.amount == 0 || coverTiles[startingTile + BoardDirections[5]]) {
						availableMoves[startingTile][startingTile + BoardDirections[5]] = true;
						anyMove = true;
					}
				}
				break;
			case 0:
				if (7 - y > 0 && 7 - x > 0 && (pieces[startingTile + BoardDirections[7]] && pieces[startingTile + BoardDirections[7]]->getPieceColor() != false) || startingTile + BoardDirections[0] == enPassantSquare)
				{
					if (check.amount == 0 || coverTiles[startingTile + BoardDirections[7]]) {
						availableMoves[startingTile][startingTile + BoardDirections[7]] = true;
						anyMove = true;
					}
				}
				break;
			default:
				break;
			}
		}
	}
}

void MoveCalcAlgo::calculateNightMovements(int startingTile)
{
	auto p = pieces[startingTile];
	if (!p->getPinned().first)
	{
		int x = startingTile % 8;
		int y = startingTile / 8;
		if (y >= 2 && x >= 1)
		{
			if ((!pieces[startingTile + BoardDirections[1] * 2 + BoardDirections[3]] || pieces[startingTile + BoardDirections[1] * 2 + BoardDirections[3]]->getPieceColor() != wTurn)
				&& (check.amount == 0 || coverTiles[startingTile + BoardDirections[1] * 2 + BoardDirections[3]])) {
				availableMoves[startingTile][startingTile + BoardDirections[1] * 2 + BoardDirections[3]] = true;
				anyMove = true;
			}
		}
		if (y >= 2 && 7 - x >= 1)
		{
			if ((!pieces[startingTile + BoardDirections[1] * 2 + BoardDirections[4]] || pieces[startingTile + BoardDirections[1] * 2 + BoardDirections[4]]->getPieceColor() != wTurn)
				&& (check.amount == 0 || coverTiles[startingTile + BoardDirections[1] * 2 + BoardDirections[4]])) {
				availableMoves[startingTile][startingTile + BoardDirections[1] * 2 + BoardDirections[4]] = true;

				anyMove = true;
			}
		}
		if (7 - y >= 2 && x >= 1)
		{
			if ((!pieces[startingTile + BoardDirections[6] * 2 + BoardDirections[3]] || pieces[startingTile + BoardDirections[6] * 2 + BoardDirections[3]]->getPieceColor() != wTurn)
				&& (check.amount == 0 || coverTiles[startingTile + BoardDirections[6] * 2 + BoardDirections[3]])) {
				availableMoves[startingTile][startingTile + BoardDirections[6] * 2 + BoardDirections[3]] = true;
				anyMove = true;
			}
		}
		if (7 - y >= 2 && 7 - x >= 1)
		{
			if ((!pieces[startingTile + BoardDirections[6] * 2 + BoardDirections[4]] || pieces[startingTile + BoardDirections[6] * 2 + BoardDirections[4]]->getPieceColor() != wTurn)
				&& (check.amount == 0 || coverTiles[startingTile + BoardDirections[6] * 2 + BoardDirections[4]])) {
				availableMoves[startingTile][startingTile + BoardDirections[6] * 2 + BoardDirections[4]] = true;
				anyMove = true;
			}
		}
		if (y >= 1 && x >= 2)
		{
			if ((!pieces[startingTile + BoardDirections[1] + BoardDirections[3] * 2] || pieces[startingTile + BoardDirections[1] + BoardDirections[3] * 2]->getPieceColor() != wTurn)
				&& (check.amount == 0 || coverTiles[startingTile + BoardDirections[1] + BoardDirections[3] * 2])) {
				availableMoves[startingTile][startingTile + BoardDirections[1] + BoardDirections[3] * 2] = true;
				anyMove = true;
			}
		}
		if (y >= 1 && 7 - x >= 2)
		{
			if ((!pieces[startingTile + BoardDirections[1] + BoardDirections[4] * 2] || pieces[startingTile + BoardDirections[1] + BoardDirections[4] * 2]->getPieceColor() != wTurn)
				&& (check.amount == 0 || coverTiles[startingTile + BoardDirections[1] + BoardDirections[4] * 2])) {
				availableMoves[startingTile][startingTile + BoardDirections[1] + BoardDirections[4] * 2] = true;
				anyMove = true;
			}
		}
		if (7 - y >= 1 && 7 - x >= 2)
		{
			if ((!pieces[startingTile + BoardDirections[6] + BoardDirections[4] * 2] || pieces[startingTile + BoardDirections[6] + BoardDirections[4] * 2]->getPieceColor() != wTurn)
				&& (check.amount == 0 || coverTiles[startingTile + BoardDirections[6] + BoardDirections[4] * 2])) {
				availableMoves[startingTile][startingTile + BoardDirections[6] + BoardDirections[4] * 2] = true;
				anyMove = true;
			}
		}
		if (7 - y >= 1 && x >= 2)
		{
			if ((!pieces[startingTile + BoardDirections[6] + BoardDirections[3] * 2] || pieces[startingTile + BoardDirections[6] + BoardDirections[3] * 2]->getPieceColor() != wTurn)
				&& (check.amount == 0 || coverTiles[startingTile + BoardDirections[6] + BoardDirections[3] * 2])) {
				availableMoves[startingTile][startingTile + BoardDirections[6] + BoardDirections[3] * 2] = true;
				anyMove = true;
			}
		}
	}
}

void MoveCalcAlgo::calculateKingMoves(int startingTile)
{
	int x = startingTile % 8;
	int y = startingTile / 8;
	int distanceToEdge[8] = { std::min<int>(x,y),y,std::min<int>(7 - x,y),x,7 - x,std::min<int>(x,7 - y),7 - y,std::min<int>(7 - x,7 - y) };
	for (int i = 0; i < 8; i++)
	{
		if (distanceToEdge[i] > 0 && !opponentAttacks[startingTile + BoardDirections[i]] && (!pieces[startingTile + BoardDirections[i]] || pieces[startingTile + BoardDirections[i]]->getPieceColor() != wTurn))
		{
			availableMoves[startingTile][startingTile + BoardDirections[i]] = true;
			anyMove = true;
		}
	}
	if (check.amount == 0)
	{
		if (wTurn)
		{
			if (wCastleK)
			{
				if (!pieces[startingTile + BoardDirections[4]] && !pieces[startingTile + BoardDirections[4] * 2] && !opponentAttacks[startingTile + BoardDirections[4]] && !opponentAttacks[startingTile + BoardDirections[4] * 2])
				{
					availableMoves[startingTile][startingTile + BoardDirections[4] * 2] = true;
					anyMove = true;
				}
			}
			if (wCastleQ)
			{
				if (!pieces[startingTile + BoardDirections[3]] && !pieces[startingTile + BoardDirections[3] * 2] && !opponentAttacks[startingTile + BoardDirections[3]] && !opponentAttacks[startingTile + BoardDirections[3] * 2])
				{
					availableMoves[startingTile][startingTile + BoardDirections[3] * 2] = true;
					anyMove = true;
				}
			}
		}
		else
		{
			if (bCastleK)
			{
				if (!pieces[startingTile + BoardDirections[4]] && !pieces[startingTile + BoardDirections[4] * 2] && !opponentAttacks[startingTile + BoardDirections[4]] && !opponentAttacks[startingTile + BoardDirections[4] * 2])
				{
					availableMoves[startingTile][startingTile + BoardDirections[4] * 2] = true;
					anyMove = true;
				}
			}
			if (bCastleQ)
			{
				if (!pieces[startingTile + BoardDirections[3]] && !pieces[startingTile + BoardDirections[3] * 2] && !opponentAttacks[startingTile + BoardDirections[3]] && !opponentAttacks[startingTile + BoardDirections[3] * 2])
				{
					availableMoves[startingTile][startingTile + BoardDirections[3] * 2] = true;
					anyMove = true;
				}
			}
		}
	}
}

void MoveCalcAlgo::calculateAttack(int startIndex, int endIndex)
{
	for (int i = startIndex; i < endIndex; i++)
	{
		if (pieces[i] && pieces[i]->getPieceColor() != wTurn)
		{
			switch (pieces[i]->getPieceType())
			{
			case PieceType::bPawn:
			case PieceType::wPawn:
				calculatePawnAttack(i);
				break;
			case PieceType::wRook:
			case PieceType::bRook:
			case PieceType::bQueen:
			case PieceType::wQueen:
			case PieceType::wBishop:
			case PieceType::bBishop:
				calculateLinearAttack(i, pieces[i]->getPieceType());
				break;
			case PieceType::wNight:
			case PieceType::bNight:
				calculateNightAttack(i);
				break;
			case PieceType::bKing:
			case PieceType::wKing:
				calculateKingAttack(i);
				break;
			}
		}
	}
}

void MoveCalcAlgo::calculateAttackParent()
{
	for (const auto& kv : pieces)
	{
		if (kv.second)
			kv.second->unPin();
	}
	check.amount = 0;
	check.attackingPiece = -1;
	check.direction = -1;

	auto maxThreads = 4;
	unsigned int lThreads = 0;
	if (maxThreads < 4)
	{
		lThreads = 1;
	}
	else if (maxThreads < 8)
	{
		lThreads = 4;
	}
	else if (maxThreads < 16)
	{
		lThreads = 8;
	}
	else
	{
		lThreads = 16;
	}
	std::list<std::thread> threads;
	for (unsigned int i = 0; i < lThreads; i++)
	{
		calculateAttack(64 / lThreads * i, 64 / lThreads * (i + 1));
	}
}

std::pair<bool, int> MoveCalcAlgo::calculateMovesParent(bool** _availableMoves)
{
	anyMove = false;
	availableMoves = (bool**)_availableMoves;
	if (check.amount > 1)
	{
		calculateKingMoves(check.kingPos);
	}
	else
	{
		if (check.amount == 1)
		{
			coverTiles[check.attackingPiece] = true;
			if (check.direction != -1)
			{
				check.calculateCoverPositions(coverTiles);
			}
		}
		auto maxThreads = 1;
		unsigned int lThreads = 0;
		if (maxThreads < 4)
		{
			lThreads = 1;
		}
		else if (maxThreads < 8)
		{
			lThreads = 4;
		}
		else if (maxThreads < 16)
		{
			lThreads = 8;
		}
		else
		{
			lThreads = 16;
		}
		calculateMoves(0, 64);
		return std::pair<bool,int>(anyMove,check.amount);
	}
}

void MoveCalcAlgo::calculateLinearAttack(int startingTile, PieceType pieceType)
{
	switch (pieceType)
	{
	case PieceType::wBishop:
	case PieceType::bBishop:
	{
		int x = startingTile % 8;
		int y = startingTile / 8;
		int distanceToEdge[4] = { std::min<int>(x,y),std::min<int>(7 - x,y),std::min<int>(x,7 - y),std::min<int>(7 - x,7 - y) };
		int directionChoice[4] = { 0,2,5,7 };
		for (int i = 0; i < 4; i++)
		{
			for (int j = 1; j <= distanceToEdge[i]; j++)
			{
				if (pieces[startingTile + BoardDirections[directionChoice[i]] * j])
				{
					if (pieces[startingTile + BoardDirections[directionChoice[i]] * j]->getPieceType() == (wTurn ? PieceType::wKing : PieceType::bKing))
					{
						check.amount++;
						check.attackingPiece = startingTile;
						check.direction = directionChoice[i];
						check.kingPos = startingTile + BoardDirections[directionChoice[i]] * j;
						opponentAttacks[startingTile + BoardDirections[directionChoice[i]] * j] = true;
					}
					else if (pieces[startingTile + BoardDirections[directionChoice[i]] * j]->getPieceColor() == wTurn)
					{
						calculatePin(startingTile + BoardDirections[directionChoice[i]] * j, directionChoice[i], distanceToEdge[i] - j);
						opponentAttacks[startingTile + BoardDirections[directionChoice[i]] * j] = true;
						break;
					}
					else
					{
						opponentAttacks[startingTile + BoardDirections[directionChoice[i]] * j] = true;
						break;
					}
				}
				else
				{
					opponentAttacks[startingTile + BoardDirections[directionChoice[i]] * j] = true;
				}
			}
		}
		break;
	}
	case PieceType::bRook:
	case PieceType::wRook:
	{
		int x = startingTile % 8;
		int y = startingTile / 8;
		int distanceToEdge[4] = { y,x,7 - x,7 - y };
		int directionChoice[4] = { 1,3,4,6 };
		for (int i = 0; i < 4; i++)
		{
			for (int j = 1; j <= distanceToEdge[i]; j++)
			{
				if (pieces[startingTile + BoardDirections[directionChoice[i]] * j])
				{
					if (pieces[startingTile + BoardDirections[directionChoice[i]] * j]->getPieceType() == (wTurn ? PieceType::wKing : PieceType::bKing))
					{
						check.amount++;
						check.attackingPiece = startingTile;
						check.direction = directionChoice[i];
						check.kingPos = startingTile + BoardDirections[directionChoice[i]] * j;
						opponentAttacks[startingTile + BoardDirections[directionChoice[i]] * j] = true;
					}
					else if (pieces[startingTile + BoardDirections[directionChoice[i]] * j]->getPieceColor() == wTurn)
					{
						calculatePin(startingTile + BoardDirections[directionChoice[i]] * j, directionChoice[i], distanceToEdge[i] - j);
						opponentAttacks[startingTile + BoardDirections[directionChoice[i]] * j] = true;
						break;
					}
					else
					{
						opponentAttacks[startingTile + BoardDirections[directionChoice[i]] * j] = true;
						break;
					}
				}
				else
				{
					opponentAttacks[startingTile + BoardDirections[directionChoice[i]] * j] = true;
				}
			}
		}
		break;
	}
	case PieceType::bQueen:
	case PieceType::wQueen:
	{
		int x = startingTile % 8;
		int y = startingTile / 8;
		int distanceToEdge[8] = { std::min<int>(x,y),y,std::min<int>(7 - x,y),x,7 - x,std::min<int>(x,7 - y),7 - y,std::min<int>(7 - x,7 - y) };
		for (int i = 0; i < 8; i++)
		{
			for (int j = 1; j <= distanceToEdge[i]; j++)
			{
				if (pieces[startingTile + BoardDirections[i] * j])
				{
					if (pieces[startingTile + BoardDirections[i] * j]->getPieceType() == (wTurn ? PieceType::wKing : PieceType::bKing))
					{
						check.amount++;
						check.attackingPiece = startingTile;
						check.direction = i;
						check.kingPos = startingTile + BoardDirections[i] * j;
						opponentAttacks[startingTile + BoardDirections[i] * j] = true;
					}
					else if (pieces[startingTile + BoardDirections[i] * j]->getPieceColor() == wTurn)
					{
						calculatePin(startingTile + BoardDirections[i] * j, i, distanceToEdge[i] - j);
						opponentAttacks[startingTile + BoardDirections[i] * j] = true;
						break;
					}
					else
					{
						opponentAttacks[startingTile + BoardDirections[i] * j] = true;
						break;
					}
				}
				else
				{
					opponentAttacks[startingTile + BoardDirections[i] * j] = true;
				}
			}
		}
		break;
	}
	}
}

void MoveCalcAlgo::calculatePawnAttack(int startingTile)
{
	int x = startingTile % 8;
	if (wTurn)
	{
		if (x > 0)
		{
			opponentAttacks[startingTile + BoardDirections[5]] = true;
			if (pieces[startingTile + BoardDirections[5]] && pieces[startingTile + BoardDirections[5]]->getPieceType() == (wTurn ? PieceType::wKing : PieceType::bKing))
			{
				check.amount++;
				check.attackingPiece = startingTile;
			}
		}
		if (7 - x > 0)
		{
			opponentAttacks[startingTile + BoardDirections[7]] = true;
			if (pieces[startingTile + BoardDirections[7]] && pieces[startingTile + BoardDirections[7]]->getPieceType() == (wTurn ? PieceType::wKing : PieceType::bKing))
			{
				check.amount++;
				check.attackingPiece = startingTile;
			}
		}
	}
	else
	{
		if (x > 0)
		{
			opponentAttacks[startingTile + BoardDirections[0]] = true;
			if (pieces[startingTile + BoardDirections[0]] && pieces[startingTile + BoardDirections[0]]->getPieceType() == (wTurn ? PieceType::wKing : PieceType::bKing))
			{
				check.amount++;
				check.attackingPiece = startingTile;
			}
		}
		if (7 - x > 0)
		{
			opponentAttacks[startingTile + BoardDirections[2]] = true;
			if (pieces[startingTile + BoardDirections[2]] && pieces[startingTile + BoardDirections[2]]->getPieceType() == (wTurn ? PieceType::wKing : PieceType::bKing))
			{
				check.amount++;
				check.attackingPiece = startingTile;
			}
		}
	}
}

void MoveCalcAlgo::calculateNightAttack(int startingTile)
{
	int x = startingTile % 8;
	int y = startingTile / 8;
	if (y >= 2 && x >= 1)
	{
		opponentAttacks[startingTile + BoardDirections[1] * 2 + BoardDirections[3]] = true;
		if (pieces[startingTile + BoardDirections[1] * 2 + BoardDirections[3]] && pieces[startingTile + BoardDirections[1] * 2 + BoardDirections[3]]->getPieceType() == (wTurn ? PieceType::wKing : PieceType::bKing))
		{
			check.amount++;
			check.attackingPiece = startingTile;
		}
	}
	if (y >= 2 && 7 - x >= 1)
	{
		opponentAttacks[startingTile + BoardDirections[1] * 2 + BoardDirections[4]] = true;
		if (pieces[startingTile + BoardDirections[1] * 2 + BoardDirections[4]] && pieces[startingTile + BoardDirections[1] * 2 + BoardDirections[4]]->getPieceType() == (wTurn ? PieceType::wKing : PieceType::bKing))
		{
			check.amount++;
			check.attackingPiece = startingTile;
		}
	}
	if (7 - y >= 2 && x >= 1)
	{
		opponentAttacks[startingTile + BoardDirections[6] * 2 + BoardDirections[3]] = true;
		if (pieces[startingTile + BoardDirections[6] * 2 + BoardDirections[3]] && pieces[startingTile + BoardDirections[6] * 2 + BoardDirections[3]]->getPieceType() == (wTurn ? PieceType::wKing : PieceType::bKing))
		{
			check.amount++;
			check.attackingPiece = startingTile;
		}
	}
	if (7 - y >= 2 && 7 - x >= 1)
	{
		opponentAttacks[startingTile + BoardDirections[6] * 2 + BoardDirections[4]] = true;
		if (pieces[startingTile + BoardDirections[6] * 2 + BoardDirections[4]] && pieces[startingTile + BoardDirections[6] * 2 + BoardDirections[4]]->getPieceType() == (wTurn ? PieceType::wKing : PieceType::bKing))
		{
			check.amount++;
			check.attackingPiece = startingTile;
		}
	}
	if (y >= 1 && x >= 2)
	{
		opponentAttacks[startingTile + BoardDirections[1] + BoardDirections[3] * 2] = true;
		if (pieces[startingTile + BoardDirections[1] + BoardDirections[3] * 2] && pieces[startingTile + BoardDirections[1] + BoardDirections[3] * 2]->getPieceType() == (wTurn ? PieceType::wKing : PieceType::bKing))
		{
			check.amount++;
			check.attackingPiece = startingTile;
		}
	}
	if (y >= 1 && 7 - x >= 2)
	{
		opponentAttacks[startingTile + BoardDirections[1] + BoardDirections[4] * 2] = true;
		if (pieces[startingTile + BoardDirections[1] + BoardDirections[4] * 2] && pieces[startingTile + BoardDirections[1] + BoardDirections[4] * 2]->getPieceType() == (wTurn ? PieceType::wKing : PieceType::bKing))
		{
			check.amount++;
			check.attackingPiece = startingTile;
		}
	}
	if (7 - y >= 1 && 7 - x >= 2)
	{
		opponentAttacks[startingTile + BoardDirections[6] + BoardDirections[4] * 2] = true;
		if (pieces[startingTile + BoardDirections[6] + BoardDirections[4] * 2] && pieces[startingTile + BoardDirections[6] + BoardDirections[4] * 2]->getPieceType() == (wTurn ? PieceType::wKing : PieceType::bKing))
		{
			check.amount++;
			check.attackingPiece = startingTile;
		}
	}
	if (7 - y >= 1 && x >= 2)
	{
		opponentAttacks[startingTile + BoardDirections[6] + BoardDirections[3] * 2] = true;
		if (pieces[startingTile + BoardDirections[6] + BoardDirections[3] * 2] && pieces[startingTile + BoardDirections[6] + BoardDirections[3] * 2]->getPieceType() == (wTurn ? PieceType::wKing : PieceType::bKing))
		{
			check.amount++;
			check.attackingPiece = startingTile;
		}
	}
}

void MoveCalcAlgo::calculateKingAttack(int startingTile)
{
	int x = startingTile % 8;
	int y = startingTile / 8;
	int distanceToEdge[8] = { std::min<int>(x,y),y,std::min<int>(7 - x,y),x,7 - x,std::min<int>(x,7 - y),7 - y,std::min<int>(7 - x,7 - y) };
	for (int i = 0; i < 8; i++)
	{
		if (distanceToEdge[i] > 0)
		{
			opponentAttacks[startingTile + BoardDirections[i]] = true;
		}
	}
}

void MoveCalcAlgo::calculatePin(int startingTile, int direction, int steps)
{
	auto p = pieces[startingTile];
	p->unPin();
	for (int i = 1; i <= steps; i++)
	{
		if (pieces[startingTile + BoardDirections[direction] * i])
		{
			if (pieces[startingTile + BoardDirections[direction] * i]->getPieceType() == (wTurn ? PieceType::wKing : PieceType::bKing))
			{
				p->setPinned(direction);
				break;
			}
			else
			{
				break;
			}
		}
	}
}
