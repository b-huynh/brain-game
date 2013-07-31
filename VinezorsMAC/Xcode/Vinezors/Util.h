#pragma once

#include "Polycode3DPhysics.h"
#include "Polycode.h"

using namespace Polycode;

#include <string>
#include <Cmath>

int randRangeInt(int min, int max);
double randRangeDouble(double min, double max);

enum Direction { NORTHWEST, NORTH, NORTHEAST, EAST, SOUTHEAST, SOUTH, SOUTHWEST, WEST, NO_DIRECTION };
Direction leftOf(Direction dir);
Direction rightOf(Direction dir);
Direction randDirection();

std::string toStringInt(int value);
std::string toStringDouble(double value);

static const Vector3 TUNNEL_REFERENCE_FORWARD = Vector3(0, 0, -1);
static const Vector3 TUNNEL_REFERENCE_UPWARD = Vector3(0, 1, 0);
static const Vector3 TUNNEL_REFERENCE_RIGHT = Vector3(1, 0, 0);
static const double TUNNEL_WIDTH = 5.0;
static const double TUNNEL_DEPTH = 5.0;
static const double TUNNEL_ANGLE_BUFFER = 1;
static const double VINE_T_OFFSET = 1.5;
static const double VINE_LENGTH = TUNNEL_WIDTH / 4;
static const double VINE_RADIUS = 0.5;

static const int NUM_SEGMENTS = 100;
static const int NBACK = 2;

static const double CAM_SPEED = 10;