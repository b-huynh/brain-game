#pragma once

enum Direction { NORTHWEST, NORTH, NORTHEAST, EAST, SOUTHEAST, SOUTH, SOUTHWEST, WEST, NO_DIRECTION };
Direction leftOf(Direction dir);
Direction rightOf(Direction dir);