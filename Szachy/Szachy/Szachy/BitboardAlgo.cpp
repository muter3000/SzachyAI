#include "BitboardAlgo.h"
#include <iostream>
#include <sstream>

void BitboardAlgo::calculateMoves()
{
	pseudoLegalMoveIndex = 0;
	legalMoveIndex = 0;
	if (wTurn)
	{
		calculateWhiteSlidingPiecesMoves();
		calculateWhitePawnMoves();
		calculateWhiteNightMoves();
		calculateWhiteKingMoves();
		for (size_t i = 0; i < pseudoLegalMoveIndex; i++)
		{
			bitBoard.makeMove(pseudoLegalMoves[i]);
			if (!calculateAttackOnWhiteTile(bitBoard.wKing, bitBoard))
			{
				legalMoves[legalMoveIndex++] = pseudoLegalMoves[i];
			}
			bitBoard.unMakeMove();
		}
		calculateWhiteCastles();
	}
	else
	{
		calculateBlackPawnMoves();
		calculateBlackNightMoves();
		calculateBlackKingMoves();
		calculateBlackSlidingPiecesMoves();
		for (size_t i = 0; i < pseudoLegalMoveIndex; i++)
		{
			bitBoard.makeMove(pseudoLegalMoves[i]);
			if (!calculateAttackOnBlackTile(bitBoard.bKing, bitBoard))
			{
				legalMoves[legalMoveIndex++] = pseudoLegalMoves[i];
			}
			bitBoard.unMakeMove();
		}
		calculateBlackCastles();
	}
}

void BitboardAlgo::printLegalMoveIndex()
{
	std::cout<< legalMoveIndex<<std::endl;
}

bool BitboardAlgo::calculateAttackOnWhiteTile(const U64 tile, BitBoard& b)
{
	if (tile == 0)
		return true;
	U64 nightSet = (tile & notHFile) << 15;
	nightSet |= (tile & notAFile) >> 15;
	nightSet |= (tile & notAFile) << 17;
	nightSet |= (tile & notHFile) >> 17;
	nightSet |= (tile & notABFile) << 10;
	nightSet |= (tile & notGHFile) >> 10;
	nightSet |= (tile & notGHFile) << 6;
	nightSet |= (tile & notABFile) >> 6;
	if (nightSet & b.bNights)
		return true;
	U64 pawnSet = (tile & notHFile) << 7;
	pawnSet |= (tile & notAFile) << 9;
	if (pawnSet & b.bPawns)
		return true;
	U64 kingSet = tile;
	kingSet |= (tile & notAFile) << 1;
	kingSet |= (tile & notHFile) >> 1;
	kingSet |= kingSet << 8;
	kingSet |= kingSet >> 8;
	if (kingSet & b.bKing)
		return true;
	static const U64 columnChecks_top[3] = { notAFile ,notHFile,notHFile };
	U64 bishopSet = 0;
	U64 mask = (b.allPieces)&(~tile);
	for (size_t i = 0; i < 2; i++)
	{
		U64 iMask = ~((mask << directions[i]) | b.wPieces);
		for (size_t j = 1; (tile << (j * directions[i])) & (columnChecks_top[i]) & iMask; j++)
		{
			bishopSet |= tile << j * directions[i];
		}
	}
	static const U64 columnChecks_bottom[3] = { notHFile,notAFile,notAFile };
	for (size_t i = 0; i < 2; i++)
	{
		U64 iMask = ~((mask >> directions[i]) | b.wPieces);
		for (size_t j = 1; (tile >> (j * directions[i])) & (columnChecks_bottom[i]) & iMask; j++)
		{
			bishopSet |= tile >> j * directions[i];
		}
	}
	if (bishopSet & b.bBishops)
		return true;
	U64 rookSet = 0;
	U64 maskLeft = ~((mask << 1));
	for (size_t i = 1; (tile << (i)) & maskLeft&notHFile; i++)
	{
		rookSet |= tile << i;
	}
	U64 maskRight = ~((mask >> 1));
	for (size_t i = 1; (tile >> (i)) & maskRight&notAFile; i++)
	{
		rookSet |= tile >> i;
	}
	U64 maskbottom = ~((mask >> 8));
	for (size_t i = 1; (tile >> (i * 8)) & maskbottom&not8Rank; i++)
	{
		rookSet |= tile >> i * 8;
	}
	U64 masktop = ~((mask << 8));
	for (size_t i = 1; (tile << (i * 8)) & masktop&not1Rank; i++)
	{
		rookSet |= tile << i * 8;
	}
	if (rookSet & b.bRooks)
		return true;
	U64 queenSet = rookSet | bishopSet;
	if (queenSet & b.bQueens)
		return true;
	return false;
}

std::pair<bool, int> BitboardAlgo::getLegalMoves(bool** availableMoves)
{
	for (int i = 0; i < 64; i++)
	{
		for (int j = 0; j < 64; j++)
		{
			availableMoves[i][j] = false;
		}
	}
	for (size_t i = 0; i < legalMoveIndex; i++)
	{
		auto data = legalMoves[i].getData();
		availableMoves[63-(data & unsigned int(0x3f))][63-((data & unsigned int(0xfc0)) >> 6)] = true;
	}
	return std::pair<bool, int>(legalMoveIndex,wTurn?calculateAttackOnWhiteTile(bitBoard.wKing,bitBoard):calculateAttackOnBlackTile(bitBoard.bKing, bitBoard));
}

U64 BitboardAlgo::calculateQueenMoves(unsigned int pos, U64 samePieces)
{
	static const U64 columnChecks_top[3] = { notAFile ,notHFile,notHFile};
	U64 queen = U64(1) << pos;
	U64 allMoves = 0;
	U64 mask = (bitBoard.allPieces)&(~queen);
	for (size_t i = 0; i < 3; i++)
	{
		U64 iMask = ~((mask<<directions[i]) | samePieces);
		for (size_t j = 1; (queen << (j * directions[i])) & (columnChecks_top[i]) & iMask; j++)
		{
			allMoves |= queen << j * directions[i];
		}
	}
	static const U64 columnChecks_bottom[3] = { notHFile,notAFile,notAFile };
	for (size_t i = 0; i < 3; i++)
	{
		U64 iMask = ~((mask >> directions[i]) | samePieces);
		for (size_t j = 1; (queen >> (j * directions[i])) & (columnChecks_bottom[i]) & iMask; j++)
		{
			allMoves |= queen >> j * directions[i];
		}
	}
	U64 maskbottom = ~((mask>>8) | samePieces);
	for (size_t i = 1; (queen >> (i * 8)) & maskbottom&not8Rank; i++)
	{
		allMoves |= queen >> i * 8;
	}
	U64 masktop = ~((mask << 8) | samePieces);
	for (size_t i = 1; (queen << (i * 8)) & masktop&not1Rank; i++)
	{
		allMoves |= queen << i * 8;
	}
	return allMoves;
}

U64 BitboardAlgo::calculateRookMoves(unsigned int pos,U64 samePieces)
{
	static const U64 columnChecks[2] = { notAFile ,notHFile};
	U64 rook = U64(1) << pos;
	U64 allMoves = 0;
	U64 mask = (bitBoard.allPieces) & (~rook);
	U64 maskLeft = ~((mask << 1) | samePieces);
	for (size_t i = 1; (rook << (i)) & maskLeft&notHFile; i++)
	{
		allMoves |= rook << i;
	}
	U64 maskRight = ~((mask >> 1) | samePieces);
	for (size_t i = 1; (rook >> (i)) & maskRight&notAFile; i++)
	{
		allMoves |= rook >> i;
	}
	U64 maskbottom = ~((mask >> 8) | samePieces);
	for (size_t i = 1; (rook >> (i * 8)) & maskbottom&not8Rank; i++)
	{
		allMoves |= rook >> i * 8;
	}
	U64 masktop = ~((mask << 8) | samePieces);
	for (size_t i = 1; (rook << (i * 8)) & masktop&not1Rank; i++)
	{
		allMoves |= rook << i * 8;
	}

	return allMoves;
}

U64 BitboardAlgo::calculateBishopMoves(unsigned int pos,U64 samePieces)
{
	static const U64 columnChecks_top[3] = { notAFile ,notHFile,notHFile };
	U64 bishop = U64(1) << pos;
	U64 allMoves = 0;
	U64 mask = (bitBoard.allPieces) & (~bishop);
	for (size_t i = 0; i < 2; i++)
	{
		U64 iMask = ~((mask << directions[i]) | samePieces);
		for (size_t j = 1; (bishop << (j * directions[i])) & (columnChecks_top[i]) & iMask; j++)
		{
			allMoves |= bishop << j * directions[i];
		}
	}
	static const U64 columnChecks_bottom[3] = { notHFile,notAFile,notAFile };
	for (size_t i = 0; i < 2; i++)
	{
		U64 iMask = ~((mask >> directions[i]) | samePieces);
		for (size_t j = 1; (bishop >> (j * directions[i])) & (columnChecks_bottom[i]) & iMask; j++)
		{
			allMoves |= bishop >> j * directions[i];
		}
	}
	return allMoves;
}

void BitboardAlgo::calculateWhitePawnMoves()
{
	U64 pawns = bitBoard.wPawns;

	U64 mask = ~(bitBoard.allPieces);

	U64 singlePush = (pawns << 8) & (mask);
	U64 singlePushTargets = singlePush >> 8;
	U64 doublePush = (singlePushTargets << 16) & (mask);
	U64 doublePushTargets = (doublePush >> 16) & U64(0x0000000000000ff00);
	U64 pushPromotionTargets = singlePushTargets & U64(0x00ff000000000000);
	singlePushTargets ^= pushPromotionTargets;
	while (singlePushTargets)
	{
		unsigned int i = bitScanForward(singlePushTargets);
		pseudoLegalMoves[pseudoLegalMoveIndex++] = (i | (unsigned int(i + 8) << 6) | (static_cast<unsigned int>(PieceType::wPawn) << 16));
		singlePushTargets ^= (U64(1) << i);
	}
	while (doublePushTargets)
	{
		unsigned int i = bitScanForward(doublePushTargets);
		pseudoLegalMoves[pseudoLegalMoveIndex++] = (i | (unsigned int(i + 16) << 6) | (static_cast<unsigned int>(PieceType::wPawn) << 16) | ((unsigned int)4) << 13);
		doublePushTargets ^= (U64(1) << i);
	}
	while (pushPromotionTargets)
	{
		unsigned int i = bitScanForward(pushPromotionTargets);
		makePromotionMoves(i, i + 8,0);
		pushPromotionTargets ^= (U64(1) << i);
	}
	U64 leftCapTargets = ((((pawns & notAFile) << 9) & bitBoard.bPieces)>>9);
	U64 rightCapTargets = ((((pawns & notHFile) << 7) & bitBoard.bPieces) >> 7);
	U64 leftCapturePromotionTargets = leftCapTargets & U64(0x00ff000000000000);
	U64 rightCapturePromotionTargets = rightCapTargets & U64(0x00ff000000000000);
	leftCapTargets ^= leftCapturePromotionTargets;
	rightCapTargets ^= rightCapturePromotionTargets;
	while (leftCapTargets)
	{
		unsigned int i = bitScanForward(leftCapTargets);
		pseudoLegalMoves[pseudoLegalMoveIndex++] = (i | (unsigned int(i + 9) << 6) | (static_cast<unsigned int>(PieceType::wPawn) << 16) | ((unsigned int)1 << 20));
		leftCapTargets ^= (U64(1) << i);
	}
	while (rightCapTargets)
	{
		unsigned int i = bitScanForward(rightCapTargets);
		pseudoLegalMoves[pseudoLegalMoveIndex++] = (i | (unsigned int(i + 7) << 6) | (static_cast<unsigned int>(PieceType::wPawn) << 16) | ((unsigned int)1 << 20));
		rightCapTargets ^= (U64(1) << i);
	}
	while (leftCapturePromotionTargets)
	{
		unsigned int i = bitScanForward(leftCapturePromotionTargets);
		makePromotionMoves(i, i + 9, 1);
		leftCapturePromotionTargets ^= (U64(1) << i);
	}
	while (rightCapturePromotionTargets)
	{
		unsigned int i = bitScanForward(rightCapturePromotionTargets);
		makePromotionMoves(i, i + 7, 1);
		rightCapturePromotionTargets ^= (U64(1) << i);
	}
	U64 enPassantTargets = ((((pawns & notAFile) << 9) & bitBoard.enPassantSquare) >> 9) | ((((pawns & notHFile) << 7) & bitBoard.enPassantSquare) >> 7);
	while (enPassantTargets)
	{
		unsigned int i = bitScanForward(enPassantTargets);
		pseudoLegalMoves[pseudoLegalMoveIndex++] = (i | (bitScanForward(bitBoard.enPassantSquare)<<6) | (static_cast<unsigned int>(PieceType::wPawn) << 16) | unsigned int(1) << 13);
		enPassantTargets ^= (U64(1) << i);
	}
}

void BitboardAlgo::calculateBlackPawnMoves()
{
	U64 pawns = bitBoard.bPawns;

	U64 mask = ~(bitBoard.allPieces);

	U64 singlePush = (pawns >> 8) & (mask);
	U64 singlePushTargets = singlePush << 8;

	U64 doublePush = (singlePushTargets >> 16) & (mask);
	U64 doublePushTargets = (doublePush << 16) & U64(0x00ff000000000000);
	U64 pushPromotionTargets = singlePushTargets & U64(0x000000000000ff00);

	singlePushTargets ^= pushPromotionTargets;

	while (singlePushTargets)
	{
		unsigned int i = bitScanForward(singlePushTargets);
		pseudoLegalMoves[pseudoLegalMoveIndex++] = (i | (unsigned int(i - 8) << 6) | (static_cast<unsigned int>(PieceType::bPawn) << 16));
		singlePushTargets ^= (U64(1) << i);
	}
	while (doublePushTargets)
	{
		unsigned int i = bitScanForward(doublePushTargets);
		pseudoLegalMoves[pseudoLegalMoveIndex++] = unsigned int(i | (unsigned int(i - 16) << 6) | (static_cast<unsigned int>(PieceType::bPawn) << 16)| (unsigned int(4)) << 13);
		doublePushTargets ^= (U64(1) << i);
	}
	while (pushPromotionTargets)
	{
		unsigned int i = bitScanForward(pushPromotionTargets);
		makePromotionMoves(i, i - 8,0);
		pushPromotionTargets ^= (U64(1) << i);
	}
	U64 leftCapTargets = ((((pawns & notHFile) >> 9) & bitBoard.wPieces) << 9);
	U64 rightCapTargets = ((((pawns & notAFile) >> 7) & bitBoard.wPieces) << 7);
	U64 leftCapturePromotionTargets = leftCapTargets & U64(0x0000000000000ff00);
	U64 rightCapturePromotionTargets = rightCapTargets & U64(0x0000000000000ff00);
	rightCapTargets ^= rightCapturePromotionTargets;
	leftCapTargets ^= leftCapturePromotionTargets;
	while (leftCapTargets)
	{
		unsigned int i = bitScanForward(leftCapTargets);
		pseudoLegalMoves[pseudoLegalMoveIndex++] = (i | (unsigned int(i - 9) << 6) | (static_cast<unsigned int>(PieceType::bPawn) << 16) | ((unsigned int)1 << 20));
		leftCapTargets ^= (U64(1) << i);
	}
	while (rightCapTargets)
	{
		unsigned int i = bitScanForward(rightCapTargets);
		pseudoLegalMoves[pseudoLegalMoveIndex++] = (i | (unsigned int(i - 7) << 6) | (static_cast<unsigned int>(PieceType::bPawn) << 16) | ((unsigned int)1 << 20));
		rightCapTargets ^= (U64(1) << i);
	}
	while (leftCapturePromotionTargets)
	{
		unsigned int i = bitScanForward(leftCapturePromotionTargets);
		makePromotionMoves(i, i - 9, 1);
		leftCapturePromotionTargets ^= (U64(1) << i);
	}
	while (rightCapturePromotionTargets)
	{
		unsigned int i = bitScanForward(rightCapturePromotionTargets);
		makePromotionMoves(i, i - 7, 1);
		rightCapturePromotionTargets ^= (U64(1) << i);
	}
	U64 enPassantTargets = ((((pawns & notHFile) >> 9) & bitBoard.enPassantSquare) << 9) | ((((pawns & notAFile) >> 7) & bitBoard.enPassantSquare) << 7);
	while (enPassantTargets)
	{
		unsigned int i = bitScanForward(enPassantTargets);
		pseudoLegalMoves[pseudoLegalMoveIndex++] = (i | (bitScanForward(bitBoard.enPassantSquare)<<6) | (static_cast<unsigned int>(PieceType::bPawn) << 16) | unsigned int(1) << 13);
		enPassantTargets ^= (U64(1) << i);
	}
}

void BitboardAlgo::calculateWhiteNightMoves()
{
	U64 nights = bitBoard.wNights;
	while (nights)
	{
		unsigned int j = bitScanForward(nights);
		U64 night = U64(1) << j;
		U64 nightPossibleMoves = 0;
		nightPossibleMoves |= (night & notHFile) << 15;
		nightPossibleMoves |= (night & notAFile) >> 15;
		nightPossibleMoves |= (night & notAFile) << 17;
		nightPossibleMoves |= (night & notHFile) >> 17;
		nightPossibleMoves |= (night & notABFile) << 10;
		nightPossibleMoves |= (night & notGHFile) >> 10;
		nightPossibleMoves |= (night & notGHFile) << 6;
		nightPossibleMoves |= (night & notABFile) >> 6;

		U64 nightMoves = nightPossibleMoves & (~bitBoard.wPieces);
		U64 nightCaptures = nightMoves & bitBoard.bPieces;
		U64 nightSilentMoves = nightMoves ^ nightCaptures;

		while (nightCaptures)
		{
			unsigned int i = bitScanForward(nightCaptures);
			pseudoLegalMoves[pseudoLegalMoveIndex++] = (j | i << 6 | (static_cast<unsigned int>(PieceType::wNight) << 16) | ((unsigned int)1 << 20));
			nightCaptures ^= (U64(1) << i);
		}
		while (nightSilentMoves)
		{
			unsigned int i = bitScanForward(nightSilentMoves);
			pseudoLegalMoves[pseudoLegalMoveIndex++] = (j | i << 6 | (static_cast<unsigned int>(PieceType::wNight) << 16));
			nightSilentMoves ^= (U64(1) << i);
		}

		nights ^= (U64(1) << j);
	}
}

void BitboardAlgo::calculateBlackNightMoves()
{
	U64 nights = bitBoard.bNights;
	while (nights)
	{
		unsigned int j = bitScanForward(nights);
		U64 night = U64(1) << j;
		U64 nightPossibleMoves = 0;
		nightPossibleMoves |= (night & notHFile) << 15;
		nightPossibleMoves |= (night & notAFile) >> 15;
		nightPossibleMoves |= (night & notAFile) << 17;
		nightPossibleMoves |= (night & notHFile) >> 17;
		nightPossibleMoves |= (night & notABFile) << 10;
		nightPossibleMoves |= (night & notGHFile) >> 10;
		nightPossibleMoves |= (night & notGHFile) << 6;
		nightPossibleMoves |= (night & notABFile) >> 6;

		U64 nightMoves = nightPossibleMoves & ~(bitBoard.bPieces);
		U64 nightCaptures = nightMoves & bitBoard.wPieces;
		U64 nightSilentMoves = nightMoves ^ nightCaptures;

		while (nightCaptures)
		{
			unsigned int i = bitScanForward(nightCaptures);
			pseudoLegalMoves[pseudoLegalMoveIndex++] = (j | i << 6 | (static_cast<unsigned int>(PieceType::bNight) << 16) | ((unsigned int)1 << 20));
			nightCaptures ^= (U64(1) << i);
		}

		while (nightSilentMoves)
		{
			unsigned int i = bitScanForward(nightSilentMoves);
			pseudoLegalMoves[pseudoLegalMoveIndex++] = (j | i << 6 | (static_cast<unsigned int>(PieceType::bNight) << 16));
			nightSilentMoves ^= (U64(1) << i);
		}

		nights ^= (U64(1) << j);
	}
}

void BitboardAlgo::calculateWhiteKingMoves()
{
	unsigned int pos = bitScanForward(bitBoard.wKing);
	U64 kingPossibleMoves = bitBoard.wKing;
	kingPossibleMoves |= (kingPossibleMoves & notAFile) << 1 | (kingPossibleMoves & notHFile) >> 1;
	kingPossibleMoves |= kingPossibleMoves >> 8;
	kingPossibleMoves |= kingPossibleMoves << 8;
	kingPossibleMoves &= ~(bitBoard.wKing);
	U64 kingMoves = kingPossibleMoves & (~bitBoard.wPieces);
	U64 kingCaptures = kingMoves & bitBoard.bPieces;
	U64 kingSilientMoves = kingMoves ^ kingCaptures;
	while (kingCaptures)
	{
		unsigned int i = bitScanForward(kingCaptures);
		pseudoLegalMoves[pseudoLegalMoveIndex++] = (pos | (i  << 6) | (static_cast<unsigned int>(PieceType::wKing) << 16) | ((unsigned int)1 << 20));
		kingCaptures ^= (U64(1) << i);
	}
	while (kingSilientMoves)
	{
		unsigned int i = bitScanForward(kingSilientMoves);
		pseudoLegalMoves[pseudoLegalMoveIndex++] = (pos | (i << 6) | (static_cast<unsigned int>(PieceType::wKing) << 16));
		kingSilientMoves ^= (U64(1) << i);
	}
}

void BitboardAlgo::calculateBlackKingMoves()
{
	unsigned int pos = bitScanForward(bitBoard.bKing);
	U64 kingPossibleMoves = bitBoard.bKing;
	kingPossibleMoves |= (kingPossibleMoves & notAFile) << 1 | (kingPossibleMoves & notHFile) >> 1;
	kingPossibleMoves |= kingPossibleMoves >> 8;
	kingPossibleMoves |= kingPossibleMoves << 8;
	kingPossibleMoves &= ~(bitBoard.bKing);
	U64 kingMoves = kingPossibleMoves & (~bitBoard.bPieces);
	U64 kingCaptures = kingMoves & bitBoard.wPieces;
	U64 kingSilientMoves = kingMoves ^ kingCaptures;
	while (kingCaptures)
	{
		unsigned int i = bitScanForward(kingCaptures);
		pseudoLegalMoves[pseudoLegalMoveIndex++] = (pos | i << 6 | (static_cast<unsigned int>(PieceType::bKing) << 16) | ((unsigned int)1 << 20));
		kingCaptures ^= (U64(1) << i);
	}
	while (kingSilientMoves)
	{
		unsigned int i = bitScanForward(kingSilientMoves);
		pseudoLegalMoves[pseudoLegalMoveIndex++] = (pos | i << 6) | (static_cast<unsigned int>(PieceType::bKing) << 16);
		kingSilientMoves ^= (U64(1) << i);
	}
}

void BitboardAlgo::calculateWhiteSlidingPiecesMoves()
{
	U64 bishops = bitBoard.wBishops;
	U64 rooks = bitBoard.wRooks;
	U64 queens = bitBoard.wQueens;
	while (queens)
	{
		unsigned int index = bitScanForward(queens);
		U64 moves = calculateQueenMoves(index, bitBoard.wPieces);
		U64 captures = moves & bitBoard.bPieces;
		U64 silentMoves = moves ^ captures;
		while (captures)
		{
			unsigned int i = bitScanForward(captures);
			pseudoLegalMoves[pseudoLegalMoveIndex++] = (index | i << 6 | (static_cast<unsigned int>(PieceType::wQueen) << 16) | ((unsigned int)1 << 20));
			captures ^= (U64(1) << i);
		}
		while (silentMoves)
		{
			unsigned int i = bitScanForward(silentMoves);
			pseudoLegalMoves[pseudoLegalMoveIndex++] = (index | i << 6 | (static_cast<unsigned int>(PieceType::wQueen) << 16));
			silentMoves ^= (U64(1) << i);
		}
		queens ^= (U64(1) << index);
	}
	while (bishops)
	{
		unsigned int index = bitScanForward(bishops);
		U64 moves = calculateBishopMoves(index, bitBoard.wPieces);
		U64 captures = moves & bitBoard.bPieces;
		U64 silentMoves = moves ^ captures;
		while (captures)
		{
			unsigned int i = bitScanForward(captures);
			pseudoLegalMoves[pseudoLegalMoveIndex++] = (index | i << 6 | (static_cast<unsigned int>(PieceType::wBishop) << 16) | ((unsigned int)1 << 20));
			captures ^= (U64(1) << i);
		}
		while (silentMoves)
		{
			unsigned int i = bitScanForward(silentMoves);
			pseudoLegalMoves[pseudoLegalMoveIndex++] = (index | i << 6 | (static_cast<unsigned int>(PieceType::wBishop) << 16));
			silentMoves ^= (U64(1) << i);
		}
		bishops ^= (U64(1) << index);
	}
	while (rooks)
	{
		unsigned int index = bitScanForward(rooks);
		U64 moves = calculateRookMoves(index, bitBoard.wPieces);
		U64 captures = moves & bitBoard.bPieces;
		U64 silentMoves = moves ^ captures;
		while (captures)
		{
			unsigned int i = bitScanForward(captures);
			pseudoLegalMoves[pseudoLegalMoveIndex++] = (index | i << 6 | (static_cast<unsigned int>(PieceType::wRook) << 16) | ((unsigned int)1 << 20));
			captures ^= (U64(1) << i);
		}
		while (silentMoves)
		{
			unsigned int i = bitScanForward(silentMoves);
			pseudoLegalMoves[pseudoLegalMoveIndex++] = (index | i << 6 | (static_cast<unsigned int>(PieceType::wRook) << 16));
			silentMoves ^= (U64(1) << i);
		}
		rooks ^= (U64(1) << index);
	}
}

void BitboardAlgo::calculateWhiteCastles()
{
	unsigned int kingPos = bitScanForward(bitBoard.wKing);
	bool QCastle = true;
	bool KCastle = true;
	if ((!(bitBoard.allPieces & QCheck)) && bitBoard.castlingRights & (0x1))
	{
		for (int i = 0; i < 3; i++)
		{
			if (calculateAttackOnWhiteTile(bitBoard.wKing<<i, bitBoard))
			{
				QCastle = false;
				break;
			}
		}
		if (QCastle)
		{
			legalMoves[legalMoveIndex++] = 3 | 5 << 6 | unsigned int(2) << 13 | static_cast<unsigned int>(PieceType::wKing) << 16 | unsigned int(0) << 25;
		}
	}
	if ((!(bitBoard.allPieces & KCheck)) && bitBoard.castlingRights & (0x2))
	{
		for (int i = 0; i < 3; i++)
		{
			if (calculateAttackOnWhiteTile(bitBoard.wKing>>i, bitBoard))
			{
				KCastle = false;
				break;
			}
		}
		if (KCastle)
		{
			legalMoves[legalMoveIndex++] = 3 | 1 << 6 | unsigned int(2) << 13 | static_cast<unsigned int>(PieceType::wKing) << 16 | unsigned int(1) << 25;
		}
	}
}

void BitboardAlgo::calculateBlackSlidingPiecesMoves()
{
	U64 bishops = bitBoard.bBishops;
	U64 rooks = bitBoard.bRooks;
	U64 queens = bitBoard.bQueens;
	while (queens)
	{
		unsigned int index = bitScanForward(queens);
		U64 moves = calculateQueenMoves(index, bitBoard.bPieces);
		U64 captures = moves & bitBoard.wPieces;
		U64 silentMoves = moves ^ captures;
		while (captures)
		{
			unsigned int i = bitScanForward(captures);
			pseudoLegalMoves[pseudoLegalMoveIndex++] = (index | i << 6 | (static_cast<unsigned int>(PieceType::bQueen) << 16) | ((unsigned int)1 << 20));
			captures ^= (U64(1) << i);
		}
		while (silentMoves)
		{
			unsigned int i = bitScanForward(silentMoves);
			pseudoLegalMoves[pseudoLegalMoveIndex++] = (index | i << 6 | (static_cast<unsigned int>(PieceType::bQueen) << 16));
			silentMoves ^= (U64(1) << i);
		}
		queens ^= (U64(1) << index);
	}
	while (bishops)
	{
		unsigned int index = bitScanForward(bishops);
		U64 moves = calculateBishopMoves(index, bitBoard.bPieces);
		U64 captures = moves & bitBoard.wPieces;
		U64 silentMoves = moves ^ captures;
		while (captures)
		{
			unsigned int i = bitScanForward(captures);
			pseudoLegalMoves[pseudoLegalMoveIndex++] = (index | i << 6 | (static_cast<unsigned int>(PieceType::bBishop) << 16) | ((unsigned int)1 << 20));
			captures ^= (U64(1) << i);
		}
		while (silentMoves)
		{
			unsigned int i = bitScanForward(silentMoves);
			pseudoLegalMoves[pseudoLegalMoveIndex++] = (index | i << 6 | (static_cast<unsigned int>(PieceType::bBishop) << 16));
			silentMoves ^= (U64(1) << i);
		}
		bishops ^= (U64(1) << index);
	}
	while (rooks)
	{
		unsigned int index = bitScanForward(rooks);
		U64 moves = calculateRookMoves(index, bitBoard.bPieces);
		U64 captures = moves & bitBoard.wPieces;
		U64 silentMoves = moves ^ captures;
		while (captures)
		{
			unsigned int i = bitScanForward(captures);
			pseudoLegalMoves[pseudoLegalMoveIndex++] = (index | i << 6 | (static_cast<unsigned int>(PieceType::bRook) << 16) | ((unsigned int)1 << 20));
			captures ^= (U64(1) << i);
		}
		while (silentMoves)
		{
			unsigned int i = bitScanForward(silentMoves);
			pseudoLegalMoves[pseudoLegalMoveIndex++] = (index | i << 6 | (static_cast<unsigned int>(PieceType::bRook) << 16));
			silentMoves ^= (U64(1) << i);
		}
		rooks ^= (U64(1) << index);
	}
}

void BitboardAlgo::calculateBlackCastles()
{
	unsigned int kingPos = bitScanForward(bitBoard.bKing);
	bool qCastle = true;
	bool kCastle = true;
	if ((!(bitBoard.allPieces&qCheck))&&bitBoard.castlingRights & (0x4))
	{
		for (int i = 0; i < 3; i++)
		{
			if (calculateAttackOnBlackTile(bitBoard.bKing<<i, bitBoard))
			{
				qCastle = false;
				break;
			}
		}
		if (qCastle)
		{
			legalMoves[legalMoveIndex++] = 59 | 61 << 6 | unsigned int(2) << 13 | static_cast<unsigned int>(PieceType::bKing) << 16 | unsigned int(2) << 25;
		}
	}
	if ((!(bitBoard.allPieces & kCheck)) && (bitBoard.castlingRights & (0x8)))
	{
		for (int i = 0; i < 3; i++)
		{
			if (calculateAttackOnBlackTile(bitBoard.bKing >> i, bitBoard))
			{
				kCastle = false;
				break;
			}
		}
		if (kCastle)
		{
			legalMoves[legalMoveIndex++] = 59 | 57 << 6 | unsigned int(2) << 13 | static_cast<unsigned int>(PieceType::bKing) << 16 | unsigned int(3) << 25;
		}
	}
}

bool BitboardAlgo::calculateAttackOnBlackTile(const U64 tile,BitBoard& b)
{
	U64 nightSet = (tile & notHFile) << 15;
	nightSet |= (tile & notAFile) >> 15;
	nightSet |= (tile & notAFile) << 17;
	nightSet |= (tile & notHFile) >> 17;
	nightSet |= (tile & notABFile) << 10;
	nightSet |= (tile & notGHFile) >> 10;
	nightSet |= (tile & notGHFile) << 6;
	nightSet |= (tile & notABFile) >> 6;
	if (nightSet & b.wNights)
		return true;
	U64 pawnSet = (tile & notAFile) >> 7;
	pawnSet |= (tile & notHFile) >> 9;
	if (pawnSet & b.wPawns)
		return true;
	U64 kingSet = tile;
	kingSet |= (tile & notAFile) << 1;
	kingSet |= (tile & notHFile) >> 1;
	kingSet |= kingSet << 8;
	kingSet |= kingSet >> 8;
	if (kingSet & b.wKing)
		return true;
	static const U64 columnChecks_top[3] = { notAFile ,notHFile,notHFile };
	U64 bishopSet = 0;
	U64 mask = (b.allPieces) & (~tile);
	for (size_t i = 0; i < 2; i++)
	{
		U64 iMask = ~((mask << directions[i]) | b.bPieces);
		for (size_t j = 1; (tile << (j * directions[i])) & (columnChecks_top[i]) & iMask; j++)
		{
			bishopSet |= tile << j * directions[i];
		}
	}
	static const U64 columnChecks_bottom[3] = { notHFile,notAFile,notAFile };
	for (size_t i = 0; i < 2; i++)
	{
		U64 iMask = ~((mask >> directions[i]) | b.bPieces);
		for (size_t j = 1; (tile >> (j * directions[i])) & (columnChecks_bottom[i]) & iMask; j++)
		{
			bishopSet |= tile >> j * directions[i];
		}
	}
	if (bishopSet & b.wBishops)
		return true;
	U64 rookSet = 0;
	U64 maskLeft = ~((mask << 1));
	for (size_t i = 1; (tile << (i)) & maskLeft&notHFile; i++)
	{
		rookSet |= tile << i;
	}
	U64 maskRight = ~((mask >> 1));
	for (size_t i = 1; (tile >> (i)) & maskRight&notAFile; i++)
	{
		rookSet |= tile >> i;
	}
	U64 maskbottom = ~((mask >> 8));
	for (size_t i = 1; (tile >> (i * 8)) & maskbottom&not8Rank; i++)
	{
		rookSet |= tile >> i * 8;
	}
	U64 masktop = ~((mask << 8));
	for (size_t i = 1; (tile << (i * 8)) & masktop&not1Rank; i++)
	{
		rookSet |= tile << i * 8;
	}
	if (rookSet & b.wRooks)
		return true;
	U64 queenSet = rookSet | bishopSet;
	if (queenSet & b.wQueens)
		return true;
	return false;
}

void BitboardAlgo::makePromotionMoves(unsigned int startSquare, unsigned int targetSquare,unsigned int capture)
{
	if (wTurn)
	{
		pseudoLegalMoves[pseudoLegalMoveIndex++] = (startSquare | ((targetSquare) << 6) | (3 << 13) | (static_cast<unsigned int>(PieceType::wPawn) << 16) | (static_cast<unsigned int>(PieceType::wBishop) << 21)|  capture<<20);
		pseudoLegalMoves[pseudoLegalMoveIndex++] = (startSquare | ((targetSquare) << 6) | (3 << 13) | (static_cast<unsigned int>(PieceType::wPawn) << 16) | (static_cast<unsigned int>(PieceType::wQueen)  << 21)|  capture<<20);
		pseudoLegalMoves[pseudoLegalMoveIndex++] = (startSquare | ((targetSquare) << 6) | (3 << 13) | (static_cast<unsigned int>(PieceType::wPawn) << 16) | (static_cast<unsigned int>(PieceType::wRook)   << 21)|  capture<<20);
		pseudoLegalMoves[pseudoLegalMoveIndex++] = (startSquare | ((targetSquare) << 6) | (3 << 13) | (static_cast<unsigned int>(PieceType::wPawn) << 16) | (static_cast<unsigned int>(PieceType::wNight)  << 21)|  capture<<20);
	}
	else
	{
		pseudoLegalMoves[pseudoLegalMoveIndex++] = (startSquare | ((targetSquare) << 6) | (3 << 13) | (static_cast<unsigned int>(PieceType::bPawn) << 16) | (static_cast<unsigned int>(PieceType::bBishop) << 21)|  capture<<20);
		pseudoLegalMoves[pseudoLegalMoveIndex++] = (startSquare | ((targetSquare) << 6) | (3 << 13) | (static_cast<unsigned int>(PieceType::bPawn) << 16) | (static_cast<unsigned int>(PieceType::bQueen)  << 21)|  capture<<20);
		pseudoLegalMoves[pseudoLegalMoveIndex++] = (startSquare | ((targetSquare) << 6) | (3 << 13) | (static_cast<unsigned int>(PieceType::bPawn) << 16) | (static_cast<unsigned int>(PieceType::bRook)   << 21)|  capture<<20);
		pseudoLegalMoves[pseudoLegalMoveIndex++] = (startSquare | ((targetSquare) << 6) | (3 << 13) | (static_cast<unsigned int>(PieceType::bPawn) << 16) | (static_cast<unsigned int>(PieceType::bNight)  << 21)|  capture<<20);
	}
}
