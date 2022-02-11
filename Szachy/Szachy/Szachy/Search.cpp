#include "Search.h"

float Search::evaluate(BitboardAlgo& b_algo, int depthLeft)
{
	b_algo.calculateMoves();
	float eval = 0;
	if (b_algo.legalMoveIndex == 0)
	{
		if (b_algo.wTurn ? b_algo.calculateAttackOnWhiteTile(b_algo.bitBoard.wKing, b_algo.bitBoard) : b_algo.calculateAttackOnBlackTile(b_algo.bitBoard.bKing, b_algo.bitBoard))
		{
			return (b_algo.wTurn ? -100 : 100) *depthLeft;
		}
		else
		{
			return 0;
		}
	}
	U64 bNightsBishops = b_algo.bitBoard.bNights | b_algo.bitBoard.bBishops;
	while (bNightsBishops)
	{
		unsigned short int i = bitScanForward(bNightsBishops);
		eval -= PieceValue::BishopNight;
		bNightsBishops ^= U64(1) << i;
	}
	U64 bPawns = b_algo.bitBoard.bPawns;
	while (bPawns)
	{
		unsigned short int i = bitScanForward(bPawns);
		eval -= PieceValue::Pawn;
		bPawns ^= U64(1) << i;
	}
	U64 bQueens = b_algo.bitBoard.bQueens;
	while (bQueens)
	{
		unsigned short int i = bitScanForward(bQueens);
		eval -= PieceValue::Queen;
		bQueens ^= U64(1) << i;
	}
	U64 bRooks = b_algo.bitBoard.bRooks;
	while (bRooks)
	{
		unsigned short int i = bitScanForward(bRooks);
		eval -= PieceValue::Rook;
		bRooks ^= U64(1) << i;
	}
	U64 wNightsBishops = b_algo.bitBoard.wNights | b_algo.bitBoard.wBishops;
	while (wNightsBishops)
	{
		unsigned short int i = bitScanForward(wNightsBishops);
		eval += PieceValue::BishopNight;
		wNightsBishops ^= U64(1) << i;
	}
	U64 wPawns = b_algo.bitBoard.wPawns;
	while (wPawns)
	{
		unsigned short int i = bitScanForward(wPawns);
		eval += PieceValue::Pawn;
		wPawns ^= U64(1) << i;
	}
	U64 wQueens = b_algo.bitBoard.wQueens;
	while (wQueens)
	{
		unsigned short int i = bitScanForward(wQueens);
		eval += PieceValue::Queen;
		wQueens ^= U64(1) << i;
	}
	U64 wRooks = b_algo.bitBoard.wRooks;
	while (wRooks)
	{
		unsigned short int i = bitScanForward(wRooks);
		eval += PieceValue::Rook;
		wRooks ^= U64(1) << i;
	}
	U64 bBlockedPawns = (b_algo.bitBoard.bPawns >> 8) & b_algo.bitBoard.allPieces;
	while (bBlockedPawns)
	{
		unsigned short int i = bitScanForward(bBlockedPawns);
		eval -= 0.5;
		bBlockedPawns ^= U64(1) << i;
	}
	U64 wBlockedPawns = (b_algo.bitBoard.wPawns << 8) & b_algo.bitBoard.allPieces;
	while (wBlockedPawns)
	{
		unsigned short int i = bitScanForward(wBlockedPawns);
		eval += 0.5;
		wBlockedPawns ^= U64(1) << i;
	}
	return eval;
}

std::string Search::intToCoord(int tile)
{
	int x = 7 - (tile % 8);
	int y = 7 - (tile / 8) + 1;
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

int Search::perf(int depth,BitboardAlgo& b)
{
	int n_moves, i;
	U64 nodes = 0;
	if (depth == 0)
		return 1ULL;
	b.calculateMoves();
	if (b.legalMoveIndex == 0)
		return 0;
	n_moves = b.legalMoveIndex;
	Move movesToMake[256];
	for (size_t i = 0; i < n_moves; i++)
	{
		movesToMake[i] = b.legalMoves[i];
	}
	for (i = 0; i < n_moves; i++) {
		b.makeMove(movesToMake[i]);
		nodes += perf(depth - 1,b);
		b.unmakeMove();
	}
	return nodes;
}

void Search::divide(int depth, BitboardAlgo& b)
{
	b.calculateMoves();
	int n_moves, i;
	n_moves = b.legalMoveIndex;
	Move movesToMake[256];
	for (size_t i = 0; i < n_moves; i++)
	{
		movesToMake[i] = b.legalMoves[i];
	}
	for (i = 0; i < n_moves; i++)
	{
		std::cout << intToCoord(63-(movesToMake[i].getData() & unsigned int(0x3f))) << " " << intToCoord(63-((movesToMake[i].getData() & unsigned int(0xfc0)) >> 6))<<"\n";
		b.makeMove(movesToMake[i]);
		std::cout<<perf(depth - 1, b)<<std::endl;
		b.unmakeMove();
	}
}

Move Search::getBestMove(int depth, bool wTurn, BitboardAlgo& b)
{
	b.calculateMoves();
	bestMoveOrder.clear();
	int n_moves = b.legalMoveIndex;
	for (size_t i = 0; i < n_moves; i++)
	{
		bestMoveOrder.push_back(std::pair<Move,float>(b.legalMoves[i],0));
	}
	for (size_t i = 0; i < n_moves; i++)
	{
		b.makeMove(bestMoveOrder[i].first);
		bestMoveOrder[i].second = wTurn?alphaBetaMin(b, -200, 200, depth):alphaBetaMax(b, -200, 200, depth);
		b.unmakeMove();
	}
	std::sort(bestMoveOrder.begin(),bestMoveOrder.end(), [&](std::pair<Move, float> a , std::pair<Move, float> b) {
		return (a.second > b.second);
		});
	return wTurn?bestMoveOrder[0].first:bestMoveOrder[n_moves-1].first;
}
float Search::alphaBetaMax(BitboardAlgo& b, float alpha, float beta, int depthleft)
{
	if (depthleft == 0) return evaluate(b,depthleft+1);
	b.calculateMoves();
	int n_moves = b.legalMoveIndex;
	if (n_moves == 0)
		return evaluate(b,depthleft+1);
	Move movesToMake[256];
	for (size_t i = 0; i < n_moves; i++)
	{
		movesToMake[i] = b.legalMoves[i];
	}
	float value = -10000;
	for (size_t i = 0; i < n_moves; i++) {
		b.makeMove(movesToMake[i]);		float score = alphaBetaMin(b,alpha, beta, depthleft - 1);
		b.unmakeMove();
		value = std::max(value, score);
		alpha = std::max(alpha, value);
		if (beta <= alpha)
			break;
	}
	return value;
}

float Search::alphaBetaMin(BitboardAlgo& b, float alpha, float beta, int depthleft)
{
	if (depthleft == 0) return evaluate(b, depthleft + 1);
	b.calculateMoves();
	int n_moves = b.legalMoveIndex;
	if (n_moves == 0)
		return evaluate(b, depthleft + 1);
	Move movesToMake[256];
	for (size_t i = 0; i < n_moves; i++)
	{
		movesToMake[i] = b.legalMoves[i];
	}
	float value = 10000;
	for (size_t i = 0; i < n_moves; i++) {
		b.makeMove(movesToMake[i]);
		float score = alphaBetaMax(b, alpha, beta, depthleft - 1);
		b.unmakeMove();
		value = std::min(value, score);
		beta = std::min(beta, value);
		if (beta <= alpha)
			break;
	}
	return value;
}
