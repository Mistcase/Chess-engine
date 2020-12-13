#pragma once
#ifndef _ConstAttacks_H_INCLUDED_
#define _ConstAttacks_H_INCLUDED_

#include <cstdint>

#include "Consts.h"
#include "CoordinationFunctional.h"

namespace _const_attacks_funcs_
{
	void initConstAttacks();

	uint64_t getKingAttacks(unsigned int x, unsigned int y);

	uint64_t getKnightAttacks(unsigned int x, unsigned int y);

	uint64_t getPawnAttacks(unsigned int x, unsigned int y, int color);

	uint64_t getTablePawnMove(unsigned int x, unsigned int y, int color);


	void EnableLegalBits(uint64_t& result, int* results, unsigned int count);
}

extern uint64_t pawnsAttacks[2][64], knightsAttacks[64], kingsAttacks[64];

#define GetPawnAttacks(square, color) pawnsAttacks[color][square]
#define GetKnightAttacks(square) knightsAttacks[square]
#define GetKingAttacks(square) kingsAttacks[square]

#endif