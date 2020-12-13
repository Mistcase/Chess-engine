#include "Moves.h"

void Castling::MakeCastlingWS(Position* position)
{
	position->figures[INDEX_KING][WHITE] ^= 0x50;
	position->figures[INDEX_ROOK][WHITE] ^= 0xA0;
	//position->key ^= (chessTables::ZobristKeys[INDEX_KING][WHITE][4] ^ chessTables::ZobristKeys[INDEX_KING][WHITE][6] ^ chessTables::ZobristKeys[INDEX_ROOK][WHITE][7] ^ chessTables::ZobristKeys[INDEX_ROOK][WHITE][5]);
}

void Castling::MakeCastlingWL(Position* position)
{
	position->figures[INDEX_KING][WHITE] ^= 0x14;
	position->figures[INDEX_ROOK][WHITE] ^= 0x9;
	//position->key ^= (chessTables::ZobristKeys[INDEX_KING][WHITE][4] ^ chessTables::ZobristKeys[INDEX_KING][WHITE][2] ^ chessTables::ZobristKeys[INDEX_ROOK][WHITE][0] ^ chessTables::ZobristKeys[INDEX_ROOK][WHITE][3]);
}

void Castling::MakeCastlingBS(Position* position)
{
	position->figures[INDEX_KING][BLACK] ^= 0x5000000000000000;
	position->figures[INDEX_ROOK][BLACK] ^= 0xA000000000000000;
	//position->key ^= (chessTables::ZobristKeys[INDEX_KING][BLACK][60] ^ chessTables::ZobristKeys[INDEX_KING][BLACK][62] ^ chessTables::ZobristKeys[INDEX_ROOK][BLACK][63] ^ chessTables::ZobristKeys[INDEX_ROOK][BLACK][61]);
}

void Castling::MakeCastlingBL(Position* position)
{
	position->figures[INDEX_KING][BLACK] ^= 0x1400000000000000;
	position->figures[INDEX_ROOK][BLACK] ^= 0x900000000000000;
	//position->key ^= (chessTables::ZobristKeys[INDEX_KING][BLACK][60] ^ chessTables::ZobristKeys[INDEX_KING][BLACK][58] ^ chessTables::ZobristKeys[INDEX_ROOK][BLACK][56] ^ chessTables::ZobristKeys[INDEX_ROOK][BLACK][59]);
}

bool Castling::CastlingIsPossibleWS(Position* position)
{
	return (!(position->allAttacks[BLACK] & 0x70)) && (position->castlings[WHITE] & CASTLING_MASK_OO) && (!(position->allFiguresBothMask & 0x60));
}

bool Castling::CastlingIsPossibleWL(Position* position)
{
	return (!(position->allAttacks[BLACK] & 0x1C)) && (position->castlings[WHITE] & CASTLING_MASK_OOO) && (!(position->allFiguresBothMask & 0xE));
}

bool Castling::CastlingIsPossibleBS(Position* position)
{
	return (!(position->allAttacks[WHITE] & 0x7000000000000000)) && (position->castlings[BLACK] & CASTLING_MASK_OO) && (!(position->allFiguresBothMask & 0x6000000000000000));
}

bool Castling::CastlingIsPossibleBL(Position* position)
{
	return (!(position->allAttacks[WHITE] & 0x1C00000000000000)) && (position->castlings[BLACK] & CASTLING_MASK_OOO) && (!(position->allFiguresBothMask & 0xE00000000000000));
}

void Castling::initCastlingsPointers()
{
	castlingsPointersMakeOrUndo[WHITE][SHORT] = MakeCastlingWS;
	castlingsPointersMakeOrUndo[WHITE][LONG] = MakeCastlingWL;
	castlingsPointersMakeOrUndo[BLACK][SHORT] = MakeCastlingBS;
	castlingsPointersMakeOrUndo[BLACK][LONG] = MakeCastlingBL;

	castlingIsPossible[WHITE][SHORT] = CastlingIsPossibleWS;
	castlingIsPossible[WHITE][LONG] = CastlingIsPossibleWL;
	castlingIsPossible[BLACK][SHORT] = CastlingIsPossibleBS;
	castlingIsPossible[BLACK][LONG] = CastlingIsPossibleBL;

	castlingsKingMoveMasks[WHITE][SHORT] = 0x50;
	castlingsKingMoveMasks[WHITE][LONG] = 0x14;
	castlingsKingMoveMasks[BLACK][SHORT] = 0x5000000000000000;
	castlingsKingMoveMasks[BLACK][LONG] = 0x1400000000000000;
}

void MakeChessMove(Position* position, ChessMove& chessMove)
{
	//Saving old position data
	chessMove.oldCastlings[WHITE] = position->castlings[WHITE];
	chessMove.oldCastlings[BLACK] = position->castlings[BLACK];
	chessMove.oldBrokenFieldValue = position->brokenField;
	chessMove.oldDrawMoves = position->drawMoves;

	if (chessMove.moveMask & 0x10)
		position->castlings[WHITE] = CASTLING_IMPOSSIBLE;
	if (chessMove.moveMask & 0x1000000000000000)
		position->castlings[BLACK] = CASTLING_IMPOSSIBLE;
	if (chessMove.moveMask & 0x80)
		position->castlings[WHITE] = position->castlings[WHITE] & CASTLING_MASK_OOO;
	if (chessMove.moveMask & 0x1)
		position->castlings[WHITE] = position->castlings[WHITE] & CASTLING_MASK_OO;
	if (chessMove.moveMask & 0x8000000000000000)
		position->castlings[BLACK] = position->castlings[BLACK] & CASTLING_MASK_OOO;
	if (chessMove.moveMask & 0x100000000000000)
		position->castlings[BLACK] = position->castlings[BLACK] & CASTLING_MASK_OO;


	position->brokenField = 0;
	position->drawMoves++;
	switch (chessMove.moveType)
	{
	case move_types::Quiet:
		position->figures[chessMove.figureIndex][chessMove.figureColor] ^= chessMove.moveMask;
		if (chessMove.figureIndex == INDEX_PAWN)
		{
			position->drawMoves = 0;

			uint64_t RANKS[2];
			if (chessMove.figureColor == WHITE)
			{
				RANKS[0] = RANK_2;
				RANKS[1] = RANK_4;
			}
			else
			{
				RANKS[0] = RANK_7;
				RANKS[1] = RANK_5;
			}
			if ((chessMove.moveMask & RANKS[0]) && (chessMove.moveMask & RANKS[1]))
				position->brokenField = (1ULL << (bitScanForward(chessMove.moveMask) + 8));
		}
		break;

	case move_types::Capture:
		position->drawMoves = 0;
		position->figures[chessMove.figureIndex][chessMove.figureColor] ^= chessMove.moveMask;
		position->figures[chessMove.victimIndex][chessMove.victimColor] &= (~(position->figures[chessMove.victimIndex][chessMove.victimColor] & chessMove.moveMask));
		break;

	case move_types::PawnSuperCapture:
		position->drawMoves = 0;
		position->figures[INDEX_PAWN][chessMove.figureColor] ^= chessMove.moveMask;
		position->figures[INDEX_PAWN][!chessMove.figureColor] ^= pawns_moves::bfDisabledPawnsMasks[chessMove.brokenFieldIndex];
		break;

	case move_types::Transformation:
		position->drawMoves = 0;
		position->figures[INDEX_PAWN][chessMove.figureColor] ^= chessMove.moveMask;
		position->figures[INDEX_PAWN][chessMove.figureColor] ^= (1ULL << chessMove.transFieldIndex);
		position->figures[chessMove.transNewFigureIndex][chessMove.figureColor] ^= (1ULL << chessMove.transFieldIndex);
		if (chessMove.victimColor != COLOR_NO_EXISTS)
			position->figures[chessMove.victimIndex][chessMove.victimColor] ^= (1ULL << chessMove.transFieldIndex);
		break;

	case move_types::Castling:
		Castling::castlingsPointersMakeOrUndo[chessMove.castlingColor][chessMove.castlingType](position);
		break;
	}
	position->refresh();
	position->activePlayer = !position->activePlayer;
}

void UndoChessMove(Position* position, ChessMove& chessMove)
{
	switch (chessMove.moveType)
	{
	case move_types::Quiet:
		position->figures[chessMove.figureIndex][chessMove.figureColor] ^= chessMove.moveMask;
		break;

	case move_types::Capture:
		position->figures[chessMove.victimIndex][chessMove.victimColor] |= position->figures[chessMove.figureIndex][chessMove.figureColor] & chessMove.moveMask;
		position->figures[chessMove.figureIndex][chessMove.figureColor] ^= chessMove.moveMask;
		break;

	case move_types::PawnSuperCapture:
		position->figures[INDEX_PAWN][chessMove.figureColor] ^= chessMove.moveMask;
		position->figures[INDEX_PAWN][!chessMove.figureColor] ^= pawns_moves::bfDisabledPawnsMasks[chessMove.brokenFieldIndex];
		position->brokenField = (1ULL << chessMove.brokenFieldIndex);
		break;

	case move_types::Castling:
		Castling::castlingsPointersMakeOrUndo[chessMove.castlingColor][chessMove.castlingType](position);
		break;

	case move_types::Transformation:
		position->figures[INDEX_PAWN][chessMove.figureColor] ^= (1ULL << chessMove.transFieldIndex);
		position->figures[chessMove.transNewFigureIndex][chessMove.figureColor] ^= (1ULL << chessMove.transFieldIndex);
		position->figures[INDEX_PAWN][chessMove.figureColor] ^= chessMove.moveMask;
		if (chessMove.victimColor != COLOR_NO_EXISTS)
			position->figures[chessMove.victimIndex][chessMove.victimColor] ^= (1ULL << chessMove.transFieldIndex);
		break;
	}

	//Restore old position data
	position->castlings[WHITE] = chessMove.oldCastlings[WHITE];
	position->castlings[BLACK] = chessMove.oldCastlings[BLACK];
	position->brokenField = chessMove.oldBrokenFieldValue;
	position->drawMoves = chessMove.oldDrawMoves;

	position->refresh();
	position->activePlayer = !position->activePlayer;
}

void move_generation::GeneratePossibleMoves(Position * position, MovesList * list)
{
	bool activeColor = position->activePlayer;
	for (uint_fast8_t figIndex = INDEX_PAWN; figIndex <= INDEX_KING; figIndex++)
	{
		uint64_t figures = position->figures[figIndex][activeColor];
		while (figures)
		{
			size_t square = bitScanForward(figures);
			uint64_t possibleMovesMask;

			switch (figIndex)
			{
			case INDEX_PAWN:
				possibleMovesMask = GetPawnMoves(position->activePlayer, square, position->allFiguresBothMask);
				break;
			case INDEX_KNIGHT:
				possibleMovesMask = GetKnightAttacks(square);
				break;
			case INDEX_BISHOP:
				possibleMovesMask = GetBishopAttacks(square, position->allFiguresBothMask);
				break;
			case INDEX_ROOK:
				possibleMovesMask = GetRookAttacks(square, position->allFiguresBothMask);
				break;
			case INDEX_QUEEN:
				possibleMovesMask = GetBishopAttacks(square, position->allFiguresBothMask) | GetRookAttacks(square, position->allFiguresBothMask);
				break;
			case INDEX_KING:
				possibleMovesMask = GetKingAttacks(square);
				break;
			}
			possibleMovesMask &= ~position->allFiguresBothMask;

			while (possibleMovesMask)
			{
				ChessMove chessMove;
				uint64_t moveMask = CreateMove((1ULL << square), (1ULL << bitScanForward(possibleMovesMask)));
				chessMove.figureColor = activeColor;
				chessMove.figureIndex = figIndex;
				chessMove.moveType = move_types::Quiet;
				chessMove.moveMask = moveMask;
				list->push(chessMove);
				DisableSignificantBit(possibleMovesMask);
			}
			DisableSignificantBit(figures);
		}
	}
}

void move_generation::GeneratePossibleCaptures(Position * position, MovesList * list)
{
	bool activeColor = position->activePlayer, oppColor = !position->activePlayer;
	uint64_t possibleCapturesMask;

	for (uint_fast8_t figIndex = INDEX_PAWN; figIndex <= INDEX_KING; figIndex++)
	{
		uint64_t figures = position->figures[figIndex][activeColor];
		while (figures)
		{
			uint64_t attacks = 0;
			size_t square = bitScanForward(figures);

			switch (figIndex)
			{
			case INDEX_PAWN:
				attacks = GetPawnAttacks(square, position->activePlayer);
				break;
			case INDEX_KNIGHT:
				attacks = GetKnightAttacks(square);
				break;
			case INDEX_BISHOP:
				attacks = GetBishopAttacks(square, position->allFiguresBothMask);
				break;
			case INDEX_ROOK:
				attacks = GetRookAttacks(square, position->allFiguresBothMask);
				break;
			case INDEX_QUEEN:
				attacks = GetBishopAttacks(square, position->allFiguresBothMask) | GetRookAttacks(square, position->allFiguresBothMask);
				break;
			case INDEX_KING:
				attacks = GetKingAttacks(square);
				break;
			}
			possibleCapturesMask = attacks & position->allFigures[oppColor];

			while (possibleCapturesMask)
			{
				ChessMove chessMove;
				chessMove.brokenFieldIndex = 0;
				chessMove.figureColor = activeColor;
				chessMove.figureIndex = figIndex;
				chessMove.moveType = move_types::Capture;
				chessMove.moveMask = CreateMove((1ULL << square), (1ULL << bitScanForward(possibleCapturesMask)));

				for (size_t index = INDEX_PAWN; index <= INDEX_KING; index++)
				{
					if (position->figures[index][oppColor] & chessMove.moveMask)
					{
						chessMove.victimIndex = (uint_fast8_t)index;
						break;
					}
				}

				chessMove.victimColor = oppColor;

				list->push(chessMove);
				DisableSignificantBit(possibleCapturesMask);
			}
			DisableSignificantBit(figures);
		}
	}
}

void move_generation::GeneratePossibleCastlings(Position * position, MovesList * list)
{
	bool activeColor = position->activePlayer;
	for (size_t castlingType = 0; castlingType < Castling::CASTLINGS_TYPES_COUNT; castlingType++)
	{
		if (Castling::castlingIsPossible[activeColor][castlingType](position))
		{
			ChessMove chessMove;
			chessMove.brokenFieldIndex = 0;
			chessMove.moveType = move_types::Castling;
			chessMove.moveMask = Castling::castlingsKingMoveMasks[activeColor][castlingType];
			chessMove.castlingType = (uint_fast8_t)castlingType;
			chessMove.castlingColor = activeColor;
			list->push(chessMove);
		}
	}
}

void move_generation::GeneratePossiblePawnSuperCaptures(Position * position, MovesList * list)
{
	bool activeColor = position->activePlayer;
	if (position->brokenField)
	{
		uint64_t attackerPawnsMask = position->figures[INDEX_PAWN][activeColor] & pawns_moves::bfAttackPositions[bitScanForward(position->brokenField)];
		while (attackerPawnsMask)
		{
			ChessMove chessMove;
			chessMove.figureColor = activeColor;
			chessMove.moveType = move_types::PawnSuperCapture;
			chessMove.moveMask = (1ULL << bitScanForward(attackerPawnsMask)) | position->brokenField;
			chessMove.brokenFieldIndex = (uint_fast8_t)(bitScanForward(position->brokenField));
			list->push(chessMove);
			DisableSignificantBit(attackerPawnsMask);
		}
	}
}

void move_generation::GeneratePossibleTransformations(Position * position, MovesList * list)
{
	uint64_t START_RANK_MASK = 0;
	uint64_t pawns = 0;

	if (position->activePlayer == WHITE && (position->figures[INDEX_PAWN][WHITE] & RANK_7))
	{
		pawns = position->figures[INDEX_PAWN][WHITE];
		START_RANK_MASK = RANK_7;
	}
	else if (position->activePlayer == BLACK && (position->figures[INDEX_PAWN][BLACK] & RANK_2))
	{
		pawns = position->figures[INDEX_PAWN][BLACK];
		START_RANK_MASK = RANK_2;
	}

	while (pawns & START_RANK_MASK)
	{
		size_t sq = position->activePlayer == WHITE ? bitScanReverse(pawns) : bitScanForward(pawns);
		uint64_t attacks = GetPawnAttacks(sq, position->activePlayer);

		uint64_t capturesMask = attacks & position->allFigures[!position->activePlayer],
				 movesMask = GetPawnMoves(position->activePlayer, sq, position->allFiguresBothMask);
		while (capturesMask)
		{
			size_t currentCapture = bitScanForward(capturesMask);
			ChessMove chessMove;
			chessMove.moveType = move_types::Transformation;
			chessMove.moveMask = (1ULL << sq) | (1ULL << currentCapture);
			chessMove.figureIndex = INDEX_PAWN;
			chessMove.figureColor = position->activePlayer;
			chessMove.victimColor = !position->activePlayer;
			chessMove.transNewFigureIndex = INDEX_QUEEN;
			chessMove.transFieldIndex = (uint_fast8_t)currentCapture;
			
			for (size_t index = INDEX_PAWN; index <= INDEX_KING; index++)
			{
				if (position->figures[index][!position->activePlayer] & chessMove.moveMask)
				{
					chessMove.victimIndex = (uint_fast8_t)index;
					break;
				}
			}
			list->push(chessMove);

			DisableSignificantBit(capturesMask);
		}
		while (movesMask)
		{
			size_t currentMove = bitScanForward(movesMask);
			ChessMove chessMove;
			chessMove.moveType = move_types::Transformation;
			chessMove.moveMask = (1ULL << sq) | (1ULL << currentMove);
			chessMove.figureIndex = INDEX_PAWN;
			chessMove.figureColor = position->activePlayer;
			chessMove.victimColor = COLOR_NO_EXISTS;
			chessMove.transNewFigureIndex = INDEX_QUEEN;
			chessMove.transFieldIndex = (uint_fast8_t)currentMove;
			list->push(chessMove);

			DisableSignificantBit(movesMask);
		}

		pawns ^= (1ULL << sq);
	}
}

void move_sorting::mvv_lva(MovesList * list)
{
	//std::cout << "Sort start's:" << list->getCounterFilled() << "\n";
	MovesList oldList = *list;
	*list = MovesList();
	for (int victim = INDEX_QUEEN; victim >= INDEX_PAWN; victim--)
	{
		for (uint_fast8_t attacker = INDEX_PAWN; attacker < FIGURES_TYPES_COUNT; attacker++)
		{
			for (size_t i = 0; i < oldList.getCounterFilled(); i++)
			{
				if (oldList.getVariant(i)->figureIndex == attacker && oldList.getVariant(i)->victimIndex == (int_fast8_t)victim)
				{
					list->push(*oldList.getVariant(i));
					//std::cout << "Sort: " << (size_t)attacker << "->" << victim << std::endl;
				}
			}
		}
	}
	//std::cout << "Sort ends:" << list->getCounterFilled() << "\n\n";
}

void move_sorting::pick(Position* position, MovesList* list)
{
	//size_t indexMaxValue = 0;
	//for (size_t i = 0; i < list->getCounterFilled(); i++)
	//{
	//	if (EvaluateFast(position, list->getVariant(i)) >= EvaluateFast(position, list->getVariant(indexMaxValue)))
	//		indexMaxValue = i;
	//}
	//ChessMove temp = *list->getVariant(0);
	//*list->getVariant(0) = *list->getVariant(indexMaxValue);
	//*list->getVariant(indexMaxValue) = temp;

}

void(*Castling::castlingsPointersMakeOrUndo[PLAYERS_COUNT][CASTLINGS_TYPES_COUNT])(Position*);
bool(*Castling::castlingIsPossible[PLAYERS_COUNT][CASTLINGS_TYPES_COUNT])(Position*);
uint64_t Castling::castlingsKingMoveMasks[PLAYERS_COUNT][CASTLINGS_TYPES_COUNT];