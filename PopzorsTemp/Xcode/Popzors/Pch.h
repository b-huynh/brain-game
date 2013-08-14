#pragma once

// Important Header Files
#import "PolycodeView.h"
#include "Polycode3DPhysics.h"
#include "Polycode.h"

using namespace Polycode;

// Special Constants
const Color BASE_COLOR = Color(255,255,255,255);
const Color BLAND_COLOR = Color(0,192,192,255);
const Color SELECT_COLOR = Color(255,255,0,255);

const Color Cpot1 = Color(192,0,0,255);
const Color Cpot2 = Color(0,192,0,255);
const Color Cpot3 = Color(0,0,192,255);
const Color Cpot4 = Color(192,192,0,255);

//const Color GROUND_COLOR = Color(255, 255, 255, 100);
const Color GROUND_COLOR = Color(128, 128, 128, 100);
const Color FEEDBACK_COLOR_GOOD = Color(0,255,0,100);
const Color FEEDBACK_COLOR_BAD = Color(255,0,0,100);

const Number SIGNAL_START = 2.0;
const Number SIGNAL_LENGTH = 0.5;

int randRangeInt(int min, int max);
double randRangeDouble(double min, double max);
Color getRandomPotColor();
std::string getSoundAccordingToColor(Color col);

const std::string Spot1 = "resources/blip.wav";
const std::string Spot2 = "resources/bloop.wav";
const std::string Spot3 = "resources/blurp.wav";
const std::string Spot4 = "resources/boing.wav";
const std::string SpotBland = "resources/womp.wav";

std::string toStringInt(int value);
std::string toStringDouble(double value);

String getTextureNameByColor(Color col);

//Save/Load File Directory (for debugging);
std::string getSaveDir();