#include "Util.h"

#include <sstream>
#include <vector>
#include <cstdlib>
#include <sys/stat.h>

using namespace std;

int randRangeInt(int min, int max)
{
    return min + rand () % (max - min + 1);
}

double randRangeDouble(double min, double max)
{
    return min + static_cast<double>(rand()) / RAND_MAX * (max - min);
}

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

Direction oppositeOf(Direction dir)
{
	switch (dir)
	{
        case NORTHWEST:
            return SOUTHEAST;
        case NORTH:
            return SOUTH;
        case NORTHEAST:
            return SOUTHWEST;
        case EAST:
            return WEST;
        case SOUTHEAST:
            return NORTHWEST;
        case SOUTH:
            return NORTH;
        case SOUTHWEST:
            return NORTHEAST;
        case WEST:
            return EAST;
        default:
            return NO_DIRECTION;
	}
}

Direction randDirection()
{
    vector<Direction> dirs(8);
	dirs[0] = NORTHWEST;
	dirs[1] = NORTH;
	dirs[2] = NORTHEAST;
	dirs[3] = EAST;
	dirs[4] = SOUTHEAST;
	dirs[5] = SOUTH;
	dirs[6] = SOUTHWEST;
	dirs[7] = WEST;
    
    int randDirIndex = rand() % dirs.size();
    
    return dirs[randDirIndex];
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

string getSaveDir()
{
    char * dir = getenv("HOME");
    string result = "";
    if (dir)
        result = string(dir) + "/.braingame/";
    else
        return "";
    
    mkdir(result.c_str(), 0777);
    return result;
}