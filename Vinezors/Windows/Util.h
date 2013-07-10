#pragma once

#include <string>

enum Direction { NORTHWEST, NORTH, NORTHEAST, EAST, SOUTHEAST, SOUTH, SOUTHWEST, WEST, NO_DIRECTION };
Direction leftOf(Direction dir);
Direction rightOf(Direction dir);

std::string toStringInt(int value);
std::string toStringDouble(double value);
