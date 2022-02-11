#include "BitBoard.h"
#include <iostream>
PieceType BitBoard::getWhitePieceOnTile(U64 tile)
{
	if (tile & wPawns)
		return PieceType::wPawn;
	else if (tile & wBishops)
		return PieceType::wBishop;
	else if (tile & wNights)
		return PieceType::wNight;
	else if (tile & wRooks)
		return PieceType::wRook;
	else
		return PieceType::wQueen;
}
PieceType BitBoard::getBlackPieceOnTile(U64 tile)
{
	if (tile & bPawns)
		return PieceType::bPawn;
	else if (tile & bBishops)
		return PieceType::bBishop;
	else if (tile & bNights)
		return PieceType::bNight;
	else if (tile & bRooks)
		return PieceType::bRook;
	else
		return PieceType::bQueen;
}
BitBoard::BitBoard(std::map<unsigned int, Piece*> piecesMap,int _castlingRigths)
{
	allPieces = 0;
	wPawns = 0;
	wNights = 0;
	wBishops = 0;
	wRooks = 0;
	wQueens = 0;
	wKing = 0;
	
	bPawns = 0;
	bNights = 0;
	bBishops = 0;
	bRooks = 0;
	bQueens = 0;
	bKing = 0;
	
	turnCount = 0;

	wPieces = 0;
	bPieces = 0;
	enPassantSquare = 0;
	castlingRights = _castlingRigths;

	for (const auto& kv : piecesMap)
	{
		if (kv.second) 
		{
			unsigned int pos = kv.first;
			switch (kv.second->getPieceType())
			{
			case PieceType::bPawn:
				bPawns |= (U64(1) << (63 - pos));
				break;
			case PieceType::bBishop:
				bBishops |= (U64(1) << (63 - pos));
				break;
			case PieceType::bNight:
				bNights |= (U64(1) << (63 - pos));
				break;
			case PieceType::bRook:
				bRooks |= (U64(1) << (63 - pos));
				break;
			case PieceType::bQueen:
				bQueens |= (U64(1) << (63 - pos));
				break;
			case PieceType::bKing:
				bKing |= (U64(1) << (63 - pos));

				break;
			case PieceType::wPawn:
				wPawns |= (U64(1) << (63 - pos));
				break;
			case PieceType::wBishop:
				wBishops |= (U64(1) << (63 - pos));
				break;
			case PieceType::wNight:
				wNights |= (U64(1) << (63 - pos));
				break;
			case PieceType::wRook:
				wRooks |= (U64(1) << (63 - pos));
				break;
			case PieceType::wQueen:
				wQueens |= (U64(1) << (63 - pos));
				break;
			case PieceType::wKing:
				wKing |= (U64(1) << (63 - pos));
				break;
			}
			if (kv.second->getPieceColor())
			{
				wPieces |= (U64(1) << (63 - pos));
			}
			else
			{
				bPieces |= (U64(1) << (63 - pos));
			}
			allPieces |= (U64(1) << (63 - pos));
		}
		
	}
}

BitBoard::BitBoard()
{
	wPawns = U64(65280);
	wNights = U64(66);
	wBishops = U64(36);
	wRooks = U64(129);
	wQueens = U64(16);
	wKing = U64(8);
	bPawns = U64(71776119061217280);
	bNights = U64(4755801206503243776);
	bBishops = U64(2594073385365405696);
	bRooks = U64(9295429630892703744);
	bQueens = U64(1152921504606846976);
	bKing = U64(576460752303423488);
	wPieces = U64(65535);
	bPieces = U64(18446462598732840960);
	allPieces =	U64(18446462598732906495);
	enPassantSquare = 0;
	castlingRights = 15;
}

void BitBoard::makeMove(Move m)
{
	turnCount++;
	unsigned int data = m.getData();
	U64 start(U64(1) << ((data & unsigned int(0x3f))));
	U64 end(U64(1) << ((data & unsigned int(0xfc0)) >> 6));
	bool capture = (data & unsigned int(0x100000)) >> 20;
	short int specialMove = (data & unsigned int(0xE000)) >> 13;
	PieceType movedPiece = static_cast <PieceType>((data & unsigned int(0xF0000)) >> 16);
	PieceType promotionTarget = static_cast<PieceType>(((data & unsigned int(0x1E00000)) >> 21));
	bool color = static_cast<int>(movedPiece) / 6;
	U64 startToEnd = start | end;
	U64 captureU64 = ~end;
	movesInGame[turnCount].enPassantSquare = enPassantSquare;
	movesInGame[turnCount].castlingRigths = castlingRights;
	movesInGame[turnCount].movedPiece = movedPiece;
	movesInGame[turnCount].moveFlag = 0;
	movesInGame[turnCount].start = start;
	movesInGame[turnCount].end = end;
	enPassantSquare = 0;
	if (capture)
	{
		if (color)
		{
			PieceType capturedPiece = getBlackPieceOnTile(end);
			movesInGame[turnCount].capturedPiece = capturedPiece;
			movesInGame[turnCount].moveFlag = 1;
			switch (capturedPiece)
			{
			case PieceType::bPawn:
				bPawns &= captureU64;
				break;
			case PieceType::bBishop:
				bBishops &= captureU64;
				break;
			case PieceType::bNight:
				bNights &= captureU64;
				break;
			case PieceType::bRook:
				bRooks &= captureU64;
				if (end & topLeft)
				{
					castlingRights &= (0xB);
				}
				else if (end & topRight)
				{
					castlingRights &= (0x7);
				}
				break;
			case PieceType::bQueen:
				bQueens &= captureU64;
				break;
			}
			bPieces &= captureU64;
		}
		else
		{
			PieceType capturedPiece = getWhitePieceOnTile(end);
			movesInGame[turnCount].capturedPiece = capturedPiece;
			movesInGame[turnCount].moveFlag = 1;
			switch (capturedPiece)
			{
			case PieceType::wPawn:
				wPawns &= captureU64;
				break;
			case PieceType::wBishop:
				wBishops &= captureU64;
				break;
			case PieceType::wNight:
				wNights &= captureU64;
				break;
			case PieceType::wRook:
				if (end & bottomLeft)
				{
					castlingRights &= (0xE);
				}
				else if (end & bottomRight)
				{
					castlingRights &= (0xD);
				}
				wRooks &= captureU64;
				break;
			case PieceType::wQueen:
				wQueens &= captureU64;
				break;
			}
			wPieces &= captureU64;
		}
		allPieces &= captureU64;
	}
	switch (specialMove)
	{
	case 4:
		switch (movedPiece)
		{
		case PieceType::bPawn:
			bPawns ^= startToEnd;
			bPieces ^= startToEnd;
			enPassantSquare = end << 8;
			break;
		case PieceType::wPawn:
			wPawns ^= startToEnd;
			wPieces ^= startToEnd;
			enPassantSquare = end >> 8;
			break;
		}
		break;
	case 1:
		captureU64 = ~(color ? end>>8 : end << 8);
		if (color)
		{
			movesInGame[turnCount].moveFlag = 3;
			bPawns &= captureU64;
			bPieces &= captureU64;
		}
		else
		{
			movesInGame[turnCount].moveFlag = 3;
			wPawns &= captureU64;
			wPieces &= captureU64;
		}
		allPieces &= captureU64;
	case 0:
	{
		switch (movedPiece)
		{
		case PieceType::bPawn:
			bPawns ^= startToEnd;
			bPieces ^= startToEnd;
			break;
		case PieceType::bBishop:
			bBishops ^= startToEnd;
			bPieces ^= startToEnd;
			break;
		case PieceType::bNight:
			bNights ^= startToEnd;
			bPieces ^= startToEnd;
			break;
		case PieceType::bRook:
			bRooks ^= startToEnd;
			if (start & topLeft)
			{
				castlingRights &= (0xB);
			}
			else if (start & topRight)
			{
				castlingRights &= (0x7);
			}
			bPieces ^= startToEnd;
			break;
		case PieceType::bQueen:
			bQueens ^= startToEnd;
			bPieces ^= startToEnd;
			break;
		case PieceType::bKing:
			bKing ^= startToEnd;
			castlingRights &= (0x3);
			bPieces ^= startToEnd;
			break;
		case PieceType::wPawn:
			wPawns ^= startToEnd;
			wPieces ^= startToEnd;
			break;
		case PieceType::wBishop:
			wBishops ^= startToEnd;
			wPieces ^= startToEnd;
			break;
		case PieceType::wNight:
			wNights ^= startToEnd;
			wPieces ^= startToEnd;
			break;
		case PieceType::wRook:
			wRooks ^= startToEnd;
			if (start & bottomLeft)
			{
				castlingRights &= (0xE);
			}
			else if (start & bottomRight)
			{
				castlingRights &= (0xD);
			}
			wPieces ^= startToEnd;
			break;
		case PieceType::wQueen:
			wQueens ^= startToEnd;
			wPieces ^= startToEnd;
			break;
		case PieceType::wKing:
			wKing ^= startToEnd;
			castlingRights &= (0xC);
			wPieces ^= startToEnd;
			break;
		}
		enPassantSquare = 0;
		break; 
	}
	case 2:
		{
		short int castleType = (data & unsigned int(0x6000000))>>25;
		switch (castleType)
		{
		case 0: //Q
		{
			wKing ^= startToEnd;
			wRooks ^= Castles::Q;
			movesInGame[turnCount].moveFlag = 5;
			castlingRights &= (0xC);
			wPieces ^= Castles::Q;
			wPieces ^= startToEnd;
			allPieces ^= Castles::Q;
			break;
		}
		case 1: //K
		{
			wKing ^= startToEnd;
			wRooks ^= Castles::K;
			movesInGame[turnCount].moveFlag = 6;
			castlingRights &= (0xC);
			wPieces ^= Castles::K;
			wPieces ^= startToEnd;
			allPieces ^= Castles::K;
			break;
		}
		case 2: //q
		{
			bKing ^= startToEnd;
			bRooks ^= Castles::q;
			movesInGame[turnCount].moveFlag = 7;
			castlingRights &= (0x3);
			bPieces ^= Castles::q;
			bPieces ^= startToEnd;
			allPieces ^= Castles::q;
			break;
		}
		case 3: //k
		{
			bKing ^= startToEnd;
			bRooks ^= Castles::k;
			movesInGame[turnCount].moveFlag = 8;
			bPieces ^= Castles::k;
			bPieces ^= startToEnd;
			castlingRights &= (0x3);
			allPieces ^= Castles::k;
			break;
		}
		}
		}
		break;
	case 3:
		switch (promotionTarget)
		{
		case PieceType::bBishop:
			bBishops ^= end;
			bPawns ^= start;
			bPieces ^= startToEnd;
			movesInGame[turnCount].promotionTarget = PieceType::bBishop;
			break;
		case PieceType::bNight:
			bNights ^= end;
			bPawns ^= start;
			bPieces ^= startToEnd;
			movesInGame[turnCount].promotionTarget = PieceType::bNight;
			break;
		case PieceType::bRook:
			bRooks ^= end;
			bPawns ^= start;
			bPieces ^= startToEnd;
			movesInGame[turnCount].promotionTarget = PieceType::bRook;
			break;
		case PieceType::bQueen:
			bQueens ^= end;
			bPawns ^= start;
			bPieces ^= startToEnd;
			movesInGame[turnCount].promotionTarget = PieceType::bQueen;
			break;
		case PieceType::wBishop:
			wBishops ^= end;
			wPawns ^= start;
			wPieces ^= startToEnd;
			movesInGame[turnCount].promotionTarget = PieceType::wBishop;
			break;
		case PieceType::wNight:
			wNights ^= end;
			wPawns ^= start;
			wPieces ^= startToEnd;
			movesInGame[turnCount].promotionTarget = PieceType::wNight;
			break;
		case PieceType::wRook:
			wRooks ^= end;
			wPawns ^= start;
			wPieces ^= startToEnd;
			movesInGame[turnCount].promotionTarget = PieceType::wRook;
			break;
		case PieceType::wQueen:
			wQueens ^= end;
			wPawns ^= start;
			wPieces ^= startToEnd;
			movesInGame[turnCount].promotionTarget = PieceType::wQueen;
			break;
		}
		enPassantSquare = 0;
		if(capture)
			movesInGame[turnCount].moveFlag = 4;
		else
			movesInGame[turnCount].moveFlag = 2;
		break;
	}
	allPieces ^= startToEnd;
}

void BitBoard::unMakeMove()
{
	U64 startToEnd = movesInGame[turnCount].start ^ movesInGame[turnCount].end;
	switch (movesInGame[turnCount].moveFlag)
	{
	case 0:
	{
		switch (movesInGame[turnCount].movedPiece)
		{
		case PieceType::bPawn:
			bPawns ^= startToEnd;
			bPieces ^= startToEnd;
			break;
		case PieceType::bBishop:
			bBishops ^= startToEnd;
			bPieces ^= startToEnd;
			break;
		case PieceType::bNight:
			bNights ^= startToEnd;
			bPieces ^= startToEnd;
			break;
		case PieceType::bRook:
			bRooks ^= startToEnd;
			bPieces ^= startToEnd;
			break;
		case PieceType::bQueen:
			bQueens ^= startToEnd;
			bPieces ^= startToEnd;
			break;
		case PieceType::bKing:
			bKing ^= startToEnd;
			bPieces ^= startToEnd;
			break;
		case PieceType::wPawn:
			wPawns ^= startToEnd;
			wPieces ^= startToEnd;
			break;
		case PieceType::wBishop:
			wBishops ^= startToEnd;
			wPieces ^= startToEnd;
			break;
		case PieceType::wNight:
			wNights ^= startToEnd;
			wPieces ^= startToEnd;
			break;
		case PieceType::wRook:
			wRooks ^= startToEnd;
			wPieces ^= startToEnd;
			break;
		case PieceType::wQueen:
			wQueens ^= startToEnd;
			wPieces ^= startToEnd;
			break;
		case PieceType::wKing:
			wKing ^= startToEnd;
			wPieces ^= startToEnd;
			break;
		}
		allPieces ^= startToEnd;
		break; 
	}
	case 1:
	{
		switch (movesInGame[turnCount].movedPiece)
		{
		case PieceType::bPawn:
			bPawns ^= startToEnd;
			bPieces ^= startToEnd;
			break;
		case PieceType::bBishop:
			bBishops ^= startToEnd;
			bPieces ^= startToEnd;
			break;
		case PieceType::bNight:
			bNights ^= startToEnd;
			bPieces ^= startToEnd;
			break;
		case PieceType::bRook:
			bRooks ^= startToEnd;
			bPieces ^= startToEnd;
			break;
		case PieceType::bQueen:
			bQueens ^= startToEnd;
			bPieces ^= startToEnd;
			break;
		case PieceType::bKing:
			bKing ^= startToEnd;
			bPieces ^= startToEnd;
			break;
		case PieceType::wPawn:
			wPawns ^= startToEnd;
			wPieces ^= startToEnd;
			break;
		case PieceType::wBishop:
			wBishops ^= startToEnd;
			wPieces ^= startToEnd;
			break;
		case PieceType::wNight:
			wNights ^= startToEnd;
			wPieces ^= startToEnd;
			break;
		case PieceType::wRook:
			wRooks ^= startToEnd;
			wPieces ^= startToEnd;
			break;
		case PieceType::wQueen:
			wQueens ^= startToEnd;
			wPieces ^= startToEnd;
			break;
		case PieceType::wKing:
			wKing ^= startToEnd;
			wPieces ^= startToEnd;
			break;
		}
		switch (movesInGame[turnCount].capturedPiece)
		{
		case PieceType::bPawn:
			bPawns ^= movesInGame[turnCount].end;
			bPieces ^= movesInGame[turnCount].end;
			break;
		case PieceType::bBishop:
			bBishops ^= movesInGame[turnCount].end;
			bPieces ^= movesInGame[turnCount].end;
			break;
		case PieceType::bNight:
			bNights ^= movesInGame[turnCount].end;
			bPieces ^= movesInGame[turnCount].end;
			break;
		case PieceType::bRook:
			bRooks ^= movesInGame[turnCount].end;
			bPieces ^= movesInGame[turnCount].end;
			break;
		case PieceType::bQueen:
			bQueens ^= movesInGame[turnCount].end;
			bPieces ^= movesInGame[turnCount].end;
			break;
		case PieceType::wPawn:
			wPawns ^= movesInGame[turnCount].end;
			wPieces ^= movesInGame[turnCount].end;
			break;
		case PieceType::wBishop:
			wBishops ^= movesInGame[turnCount].end;
			wPieces ^= movesInGame[turnCount].end;
			break;
		case PieceType::wNight:
			wNights ^= movesInGame[turnCount].end;
			wPieces ^= movesInGame[turnCount].end;
			break;
		case PieceType::wRook:
			wRooks ^= movesInGame[turnCount].end;
			wPieces ^= movesInGame[turnCount].end;
			break;
		case PieceType::wQueen:
			wQueens ^= movesInGame[turnCount].end;
			wPieces ^= movesInGame[turnCount].end;
			break;
		}
		allPieces |= startToEnd;
		break;
	}
	case 2:
		switch (movesInGame[turnCount].promotionTarget)
		{
		case PieceType::bBishop:
			bBishops ^= movesInGame[turnCount].end;
			bPawns ^= movesInGame[turnCount].start;
			bPieces ^= startToEnd;
			break;
		case PieceType::bNight:
			bNights ^= movesInGame[turnCount].end;
			bPawns ^= movesInGame[turnCount].start;
			bPieces ^= startToEnd;
			break;
		case PieceType::bRook:
			bRooks ^= movesInGame[turnCount].end;
			bPawns ^= movesInGame[turnCount].start;
			bPieces ^= startToEnd;
			break;
		case PieceType::bQueen:
			bQueens ^= movesInGame[turnCount].end;
			bPawns ^= movesInGame[turnCount].start;
			bPieces ^= startToEnd;
			break;
		case PieceType::wBishop:
			wBishops ^= movesInGame[turnCount].end;
			wPawns ^= movesInGame[turnCount].start;
			wPieces ^= startToEnd;
			break;
		case PieceType::wNight:
			wNights ^= movesInGame[turnCount].end;
			wPawns ^= movesInGame[turnCount].start;
			wPieces ^= startToEnd;
			break;
		case PieceType::wRook:
			wRooks ^= movesInGame[turnCount].end;
			wPawns ^= movesInGame[turnCount].start;
			wPieces ^= startToEnd;
			break;
		case PieceType::wQueen:
			wQueens ^= movesInGame[turnCount].end;
			wPawns ^= movesInGame[turnCount].start;
			wPieces ^= startToEnd;
			break;
		}
		allPieces ^= startToEnd;
		break;
	case 3:
	{
		switch (movesInGame[turnCount].movedPiece)
		{
		case PieceType::bPawn:
			bPawns ^= startToEnd;
			wPawns ^= (movesInGame[turnCount].end << 8);
			wPieces ^= (movesInGame[turnCount].end << 8);
			allPieces ^= (movesInGame[turnCount].end << 8);
			bPieces ^= startToEnd;
			break;
		case PieceType::wPawn:
			wPawns ^= startToEnd;
			bPawns ^= (movesInGame[turnCount].end >> 8);
			bPieces ^= (movesInGame[turnCount].end >> 8);
			allPieces ^= (movesInGame[turnCount].end >> 8);
			wPieces ^= startToEnd;
			break;
		}
		allPieces ^= startToEnd;
		break;
	}
	case 4:
		switch (movesInGame[turnCount].promotionTarget)
		{
		case PieceType::bBishop:
			bBishops ^= movesInGame[turnCount].end;
			bPawns ^= movesInGame[turnCount].start;
			bPieces ^= startToEnd;
			break;
		case PieceType::bNight:
			bNights ^= movesInGame[turnCount].end;
			bPawns ^= movesInGame[turnCount].start;
			bPieces ^= startToEnd;
			break;
		case PieceType::bRook:
			bRooks ^= movesInGame[turnCount].end;
			bPawns ^= movesInGame[turnCount].start;
			bPieces ^= startToEnd;
			break;
		case PieceType::bQueen:
			bQueens ^= movesInGame[turnCount].end;
			bPawns ^= movesInGame[turnCount].start;
			bPieces ^= startToEnd;
			break;
		case PieceType::wBishop:
			wBishops ^= movesInGame[turnCount].end;
			wPawns ^= movesInGame[turnCount].start;
			wPieces ^= startToEnd;
			break;
		case PieceType::wNight:
			wNights ^= movesInGame[turnCount].end;
			wPawns ^= movesInGame[turnCount].start;
			wPieces ^= startToEnd;
			break;
		case PieceType::wRook:
			wRooks ^= movesInGame[turnCount].end;
			wPawns ^= movesInGame[turnCount].start;
			wPieces ^= startToEnd;
			break;
		case PieceType::wQueen:
			wQueens ^= movesInGame[turnCount].end;
			wPawns ^= movesInGame[turnCount].start;
			wPieces ^= startToEnd;
			break;
		}
		switch (movesInGame[turnCount].capturedPiece)
		{
		case PieceType::bPawn:
			bPawns ^= movesInGame[turnCount].end;
			bPieces ^= movesInGame[turnCount].end;
			break;
		case PieceType::bBishop:
			bBishops ^= movesInGame[turnCount].end;
			bPieces ^= movesInGame[turnCount].end;
			break;
		case PieceType::bNight:
			bNights ^= movesInGame[turnCount].end;
			bPieces ^= movesInGame[turnCount].end;
			break;
		case PieceType::bRook:
			bRooks ^= movesInGame[turnCount].end;
			bPieces ^= movesInGame[turnCount].end;
			break;
		case PieceType::bQueen:
			bQueens ^= movesInGame[turnCount].end;
			bPieces ^= movesInGame[turnCount].end;
			break;
		case PieceType::wPawn:
			wPawns ^= movesInGame[turnCount].end;
			wPieces ^= movesInGame[turnCount].end;
			break;
		case PieceType::wBishop:
			wBishops ^= movesInGame[turnCount].end;
			wPieces ^= movesInGame[turnCount].end;
			break;
		case PieceType::wNight:
			wNights ^= movesInGame[turnCount].end;
			wPieces ^= movesInGame[turnCount].end;
			break;
		case PieceType::wRook:
			wRooks ^= movesInGame[turnCount].end;
			wPieces ^= movesInGame[turnCount].end;
			break;
		case PieceType::wQueen:
			wQueens ^= movesInGame[turnCount].end;
			wPieces ^= movesInGame[turnCount].end;
			break;
		}
		allPieces |= startToEnd;
		break;
	case 5: //Q
		wKing ^= startToEnd;		
		wPieces ^= startToEnd;
		allPieces ^= startToEnd;
		wRooks ^= Castles::Q;
		wPieces ^= Castles::Q;
		allPieces ^= Castles::Q;
		break;
	case 6: //K
		wKing ^= startToEnd;
		wPieces ^= startToEnd;
		allPieces ^= startToEnd;
		wRooks ^= Castles::K;
		wPieces ^= Castles::K;
		allPieces ^= Castles::K;
		break;
	case 7: //q
		bKing ^= startToEnd;
		bPieces ^= startToEnd;
		allPieces ^= startToEnd;
		bRooks ^= Castles::q;
		bPieces ^= Castles::q;
		allPieces ^= Castles::q;
		break;
	case 8: //k
		bKing ^= startToEnd;
		bPieces ^= startToEnd;
		allPieces ^= startToEnd;
		bRooks ^= Castles::k;
		bPieces ^= Castles::k;
		allPieces ^= Castles::k;
		break;
	}
	enPassantSquare = movesInGame[turnCount].enPassantSquare;
	castlingRights = movesInGame[turnCount].castlingRigths;

	turnCount--;
}
