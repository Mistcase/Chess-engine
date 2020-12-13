#pragma once
#ifndef _RAYS_INCLUDED_
#define _RAYS_INCLUDED_

#include <cstdint>

#include "Consts.h"
#include "CoordinationFunctional.h"

namespace Rays
{
	enum
	{
		North, //Север
		South, //Юг
		West, //Запад
		East,//Восток

		NorthWest, //Северо-Запад
		NorthEast, //Северо-Восток
		SouthWest, //Юго-Запад
		SouthEast //Юго-Восток

	};
	namespace _values_
	{
		extern uint64_t precalcRays[BOARD_SIZE][COMMON_RAYS_DIRECTIONS_COUNT];
	}

	inline void init()
	{
		for (int sq = 0; sq < BOARD_SIZE; sq++)
		{
			Rays::_values_::precalcRays[sq][Rays::North] = 0;
			Rays::_values_::precalcRays[sq][Rays::South] = 0;
			Rays::_values_::precalcRays[sq][Rays::West] = 0;
			Rays::_values_::precalcRays[sq][Rays::East] = 0;
			Rays::_values_::precalcRays[sq][Rays::NorthWest] = 0;
			Rays::_values_::precalcRays[sq][Rays::NorthEast] = 0;
			Rays::_values_::precalcRays[sq][Rays::SouthWest] = 0;
			Rays::_values_::precalcRays[sq][Rays::SouthEast] = 0;

			unsigned int sqX = sq % 8 + 1;
			unsigned int sqY = sq / 8 + 1;
			unsigned int currentX, currentY;

			currentX = sqX;
			currentY = sqY + 1;
			while (currentY <= 8)
				Rays::_values_::precalcRays[sq][Rays::North] |= (1ULL << getLocationIndex(currentX, currentY++));

			currentX = sqX;
			currentY = sqY - 1;
			while (currentY > 0)
				Rays::_values_::precalcRays[sq][Rays::South] |= (1ULL << getLocationIndex(currentX, currentY--));

			currentX = sqX - 1;
			currentY = sqY;
			while (currentX > 0)
				Rays::_values_::precalcRays[sq][Rays::West] |= (1ULL << getLocationIndex(currentX--, currentY));

			currentX = sqX + 1;
			currentY = sqY;
			while (currentX <= 8)
				Rays::_values_::precalcRays[sq][Rays::East] |= (1ULL << getLocationIndex(currentX++, currentY));


			//IntermidiateRays
			currentX = sqX - 1;
			currentY = sqY + 1;
			while (currentX > 0 && currentY <= 8)
				Rays::_values_::precalcRays[sq][Rays::NorthWest] |= (1ULL << getLocationIndex(currentX--, currentY++));

			currentX = sqX + 1;
			currentY = sqY + 1;
			while (currentX <= 8 && currentY <= 8)
				Rays::_values_::precalcRays[sq][Rays::NorthEast] |= (1ULL << getLocationIndex(currentX++, currentY++));

			currentX = sqX - 1;
			currentY = sqY - 1;
			while (currentX > 0 && currentY > 0)
				Rays::_values_::precalcRays[sq][Rays::SouthWest] |= (1ULL << getLocationIndex(currentX--, currentY--));

			currentX = sqX + 1;
			currentY = sqY - 1;
			while (currentX <= 8 && currentY > 0)
				Rays::_values_::precalcRays[sq][Rays::SouthEast] |= (1ULL << getLocationIndex(currentX++, currentY--));
		}
	}
}

#define GetRay(sq, rayDirection) Rays::_values_::precalcRays[sq][rayDirection]

#endif