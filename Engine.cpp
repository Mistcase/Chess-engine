#include "Engine.h"

void Engine::startCalculations(int depth)
{
	if (this->units[0].setPosition(this->position))
		this->units[0].go(depth);
}

void Engine::stopCalculations()
{
	for (int i = 0; i < COMPUTING_UNITS_LIMIT; i++)
		this->units[i].stop();
}

bool Engine::setOption(int optionType, void * value)
{
	switch (optionType)
	{
	case ENGINE_OPTION_THREADS:
		this->threadsCounter = *(int*)(value);
		break;
	case ENGINE_OPTION_MULTIPV:
		this->multiPv = *(int*)(value);
		break;
	case ENGINE_OPTION_MINIMUM_THINKING_TIME:
		this->minimumThinkingTime = *(int*)(value);
		break;
	case ENGINE_OPTION_DEBUG_FILE_NAME:
		this->debugFileName = *(std::string*)(value);
		break;
	case ENGINE_OPTION_PONDER:
		this->ponder = *(bool*)(value);
		break;
	default:
		return false;
	}
	return true;
}

void Engine::setPosition(Position pos)
{
	this->position = pos;
}

bool Engine::setPosition(std::string fenString)
{
	std::string fenFields[6];
	std::string str = fenString;

	for (int index = 0; index < 6; index++)
	{
		std::string temp;
		SplitOffOneWord(str, &fenFields[index], &temp);
		str = temp;

		if (fenFields[index].empty())
			return false;
	}

	this->position = Position();

	unsigned int fieldIndex = 56, innerCounter = 0;
	for (unsigned int i = 0; i < fenFields[0].size(); i++)
	{
		if (fenFields[0][i] >= 49 && fenFields[0][i] <= 57)
		{
			//numbers
			std::string temp;
			temp += fenFields[0][i];

			int def = atoi(temp.c_str());
			if (def > 0 && def <= 8)
				innerCounter += def;
			else return false;
		}
		else
		{
			unsigned int realIndex = fieldIndex + innerCounter;
			if (fenFields[0][i] >= 65 && fenFields[0][i] <= 90)
			{
				//White
				switch (fenFields[0][i])
				{
				case 'P':
					this->position.figures[INDEX_PAWN][WHITE] |= (1ULL << realIndex);
					break;
				case 'N':
					this->position.figures[INDEX_KNIGHT][WHITE] |= (1ULL << realIndex);
					break;
				case 'B':
					this->position.figures[INDEX_BISHOP][WHITE] |= (1ULL << realIndex);
					break;
				case 'R':
					this->position.figures[INDEX_ROOK][WHITE] |= (1ULL << realIndex);
					break;
				case 'Q':
					this->position.figures[INDEX_QUEEN][WHITE] |= (1ULL << realIndex);
					break;
				case 'K':
					this->position.figures[INDEX_KING][WHITE] |= (1ULL << realIndex);
					break;
				}
			}
			else if (fenFields[0][i] >= 97 && fenFields[0][i] <= 122)
			{
				switch (fenFields[0][i])
				{
				case 'p':
					this->position.figures[INDEX_PAWN][BLACK] |= (1ULL << realIndex);
					break;
				case 'n':
					this->position.figures[INDEX_KNIGHT][BLACK] |= (1ULL << realIndex);
					break;
				case 'b':
					this->position.figures[INDEX_BISHOP][BLACK] |= (1ULL << realIndex);
					break;
				case 'r':
					this->position.figures[INDEX_ROOK][BLACK] |= (1ULL << realIndex);
					break;
				case 'q':
					this->position.figures[INDEX_QUEEN][BLACK] |= (1ULL << realIndex);
					break;
				case 'k':
					this->position.figures[INDEX_KING][BLACK] |= (1ULL << realIndex);
					break;
				}
			}
			else if (fenFields[0][i] == 47)
			{
				//      '/'
			}
			else return false;

			if (fenFields[0][i] != 47)
				innerCounter++;
			if (innerCounter == 8)
			{
				innerCounter = 0;
				fieldIndex -= 8;
			}
		}
	}

	this->position.activePlayer = fenFields[1] == "w" ? WHITE : BLACK;

	if (fenFields[2].size() > 4)
		return false;
	for (unsigned int i = 0; i < fenFields[2].size(); i++)
	{
		switch (fenFields[2][i])
		{
		case 'K':
			this->position.castlings[WHITE] |= CASTLING_MASK_OO;
			break;
		case 'Q':
			this->position.castlings[WHITE] |= CASTLING_MASK_OOO;
			break;
		case 'k':
			this->position.castlings[BLACK] |= CASTLING_MASK_OO;
			break;
		case 'q':
			this->position.castlings[BLACK] |= CASTLING_MASK_OOO;
			break;
		case '-':
			this->position.castlings[WHITE] = 0;
			this->position.castlings[BLACK] = 0;
			break;
		default:
			return false;
		}
	}

	this->position.brokenField = fenFields[3] != "-" ? GetBoardPosition(fenFields[3]) : 0;

	this->position.drawMoves = atoi(fenFields[4].c_str());
	this->position.movesCount = atoi(fenFields[5].c_str());

	this->position.refresh();

	return true;
}

bool Engine::makeMove(std::string strMove)
{
	ChessMove chessMove;
	chessMove.moveMask = GetBoardPosition(std::string(strMove, 0, 2)) | GetBoardPosition(std::string(strMove, 2, 2));

	this->position.refresh();

	//Identify active figure
	for (int index = INDEX_PAWN; index <= INDEX_KING; index++)
	{
		if (this->position.figures[index][this->position.activePlayer] & chessMove.moveMask)
		{
			chessMove.figureIndex = index;
			chessMove.figureColor = this->position.activePlayer;
			break;
		}
	}

	//Transformation
	if (strMove.size() == 5)
	{
		char newFigure = strMove[4];
		chessMove.transFieldIndex = static_cast<int_fast8_t>(bitScanForward(chessMove.moveMask & (RANK_1 | RANK_8)));
		chessMove.transNewFigureIndex = (newFigure == 'q' ? INDEX_QUEEN : newFigure == 'r' ? INDEX_ROOK : newFigure == 'b' ? INDEX_BISHOP : newFigure == 'k' ? INDEX_KNIGHT : FIGURE_INDEX_IMPOSSIBLE);
		chessMove.moveType = move_types::Transformation;
		if (position.allFigures[!position.activePlayer] & (1ULL << chessMove.transFieldIndex))
		{
			chessMove.victimColor = !position.activePlayer;
			for (size_t index = INDEX_PAWN; index <= INDEX_KING; index++)
			{
				if (position.figures[index][!position.activePlayer] & chessMove.moveMask)
				{
					chessMove.victimIndex = (uint_fast8_t)index;
					break;
				}
			}
		}
		else
		{
			chessMove.victimColor = COLOR_NO_EXISTS;
		}
	}
	//Capture
	else if (this->position.allFigures[!this->position.activePlayer] & chessMove.moveMask)
	{
		chessMove.moveType = move_types::Capture;
		chessMove.victimColor = !this->position.activePlayer;
		for (int index = 0; index <= INDEX_KING; index++)
		{
			if (this->position.figures[index][!position.activePlayer] & chessMove.moveMask)
			{
				chessMove.victimIndex = index;
				break;
			}
		}
	}
	//Castling
	else if (chessMove.figureIndex == INDEX_KING && (strMove == "e1g1" || strMove == "e1c1" || strMove == "e8g8" || strMove == "e8c8"))
	{
		chessMove.moveType = move_types::Castling;

		if (strMove == "e1g1")
		{
			chessMove.castlingColor = WHITE;
			chessMove.castlingType = Castling::SHORT;
		}
		if (strMove == "e1c1")
		{
			chessMove.castlingColor = WHITE;
			chessMove.castlingType = Castling::LONG;
		}
		if (strMove == "e8g8")
		{
			chessMove.castlingColor = BLACK;
			chessMove.castlingType = Castling::SHORT;
		}
		if (strMove == "e8c8")
		{
			chessMove.castlingColor = BLACK;
			chessMove.castlingType = Castling::LONG;
		}
	}
	else
	{
		//PawnSuperCapture
		if ((chessMove.figureIndex == INDEX_PAWN && chessMove.figureColor == WHITE && (1ULL << bitScanForward(chessMove.moveMask) & RANK_5) && (bitScanForward(chessMove.moveMask) + 8 != bitScanReverse(chessMove.moveMask)))
			|| (chessMove.figureIndex == INDEX_PAWN && chessMove.figureColor == BLACK && (1ULL << bitScanReverse(chessMove.moveMask) & RANK_4) && (bitScanReverse(chessMove.moveMask) - 8 != bitScanForward(chessMove.moveMask))))
		{
			chessMove.moveType = move_types::PawnSuperCapture;
			chessMove.brokenFieldIndex = (uint_fast8_t)(this->position.activePlayer == WHITE ? bitScanReverse(chessMove.moveMask) : bitScanForward(chessMove.moveMask));
		}
		//Quiet
		else
		{
			chessMove.moveType = move_types::Quiet;
		}
	}

	MakeChessMove(&this->position, chessMove);

	return true;
}

Position * Engine::getPosition()
{
	return &this->position;
}
