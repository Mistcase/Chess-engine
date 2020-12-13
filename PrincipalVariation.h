#pragma once
#ifndef _PRINCIPAL_VARIATION_INCLUDED_
#define _PRINCIPAL_VARIATION_INCLUDED_

#include <string>
#include <iostream>

#include "Consts.h"
#include "Coods.h"
#include "Bitscan.h"
#include "Position.h"

class LinePV
{
public:
	void writeLine(LinePV* sourceLine, uint64_t& bestMove);
	void reset();

	std::string createString(Position startposValue);
	Flag find(Bitboard moveMask);
private:
	uint64_t moves[MAX_PLY];
	size_t count = 0;
};

#endif