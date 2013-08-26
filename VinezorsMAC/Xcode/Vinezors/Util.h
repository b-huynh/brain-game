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
Direction oppositeOf(Direction dir);
Direction randDirection();

std::string toStringInt(int value);
std::string toStringDouble(double value);
std::string getSaveDir();

static const Vector3 TUNNEL_REFERENCE_FORWARD = Vector3(0, 0, -1);
static const Vector3 TUNNEL_REFERENCE_UPWARD = Vector3(0, 1, 0);
static const Vector3 TUNNEL_REFERENCE_RIGHT = Vector3(1, 0, 0);
static const int TUNNEL_MIN_ANGLE_TURN = 1;
static const int TUNNEL_MAX_ANGLE_TURN = 1;
static const double TUNNEL_WIDTH = 5.0;
static const double TUNNEL_DEPTH = 5.0;
static const double TUNNEL_SEGMENT_BUFFER = 1;
static const int TUNNEL_SEGMENTS_PER_SECTION = 5;
static const int TUNNEL_SEGMENTS_PER_POD = 3;
static const int INITIATION_SECTIONS = 1;
static const double VINE_T_OFFSET = 1.0;
static const double VINE_RADIUS = 0.5;
static const int POD_APPEARANCE = 3;

static const int TUNNEL_SECTIONS = 3;
static const int NBACK = 2;

static const double FOG_OFFSET = 15;

static int SCREEN_WIDTH = 1024;
static int SCREEN_HEIGHT = 800;
static double BAR_XPOS = 20;
static double BAR_YPOS = 20;
static double BAR_WIDTH = SCREEN_WIDTH - BAR_XPOS * 2;
static double BAR_HEIGHT = SCREEN_HEIGHT / 20;
static int LABEL1_POSX = 0;
static int LABEL1_POSY = SCREEN_HEIGHT - 55;
static int LABEL2_POSX = 0;
static int LABEL2_POSY = SCREEN_HEIGHT - 105;
static int LABEL3_POSX = SCREEN_WIDTH - 155;
static int LABEL3_POSY = SCREEN_HEIGHT - 55;
static const int STARTING_HP = 0;
static const int HP_NEGATIVE_LIMIT = -3;
static const int HP_POSITIVE_LIMIT = 6;
static const double DRAIN_SPEED = 2;
static const double CAM_SPEED = 10;
