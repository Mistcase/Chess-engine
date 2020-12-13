#pragma once
#ifndef _COORDINATION_FUNCTIONAL_INCLUDED_
#define _COORDINATION_FUNCTIONAL_INCLUDED_

inline int getLocationIndex(int x, int y)
{
	return (x > 0 && y > 0 && x <= 8 && y <= 8) ? 8 * (y - 1) + x - 1 : -1;
}

#endif