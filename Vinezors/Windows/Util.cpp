#include "Util.h"

#include <sstream>

using namespace std;

Direction leftOf(Direction dir)
{
	switch (dir)	
	{
	case NORTHWEST:
		return WEST;
	case NORTH:
		return NORTHWEST;
	case NORTHEAST:
		return NORTH;
	case EAST:
		return NORTHEAST;
	case SOUTHEAST:
		return EAST;
	case SOUTH:
		return SOUTHEAST;
	case SOUTHWEST:
		return SOUTH;
	case WEST:
		return SOUTHWEST;
	default:
		return NO_DIRECTION;
	}
}

Direction rightOf(Direction dir)
{
	switch (dir)	
	{
	case NORTHWEST:
		return NORTH;
	case NORTH:
		return NORTHEAST;
	case NORTHEAST:
		return EAST;
	case EAST:
		return SOUTHEAST;
	case SOUTHEAST:
		return SOUTH;
	case SOUTH:
		return SOUTHWEST;
	case SOUTHWEST:
		return WEST;
	case WEST:
		return NORTHWEST;
	default:
		return NO_DIRECTION;
	}
}

string toStringInt(int value)
{
	stringstream ss;
	ss << value;
	return ss.str();
}

string toStringDouble(double value)
{
	stringstream ss;
	ss << value;
	return ss.str();
}