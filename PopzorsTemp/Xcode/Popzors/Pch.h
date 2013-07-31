#pragma once

// Important Header Files
#import "PolycodeView.h"
#include "Polycode3DPhysics.h"
#include "Polycode.h"

using namespace Polycode;

// Special Constants
const Color BLAND_COLOR = Color(0,192,192,255);
const Color SELECT_COLOR = Color(255,255,0,255);

const Color Cpot1 = Color(192,0,0,255);
const Color Cpot2 = Color(0,192,0,255);
const Color Cpot3 = Color(0,0,192,255);
const Color Cpot4 = Color(192,192,0,255);

const Color GROUND_COLOR = Color(255, 255, 255, 100);
const Color FEEDBACK_COLOR_GOOD = Color(0,255,0,100);
const Color FEEDBACK_COLOR_BAD = Color(255,0,0,100);

const Number SIGNAL_START = 2.0;
const Number SIGNAL_LENGTH = 0.5;

int randRangeInt(int min, int max);
double randRangeDouble(double min, double max);
Color getRandomPotColor();
String getSoundAccordingToColor(Color col);

const String Spot1 = "resources/blip.wav";
const String Spot2 = "resources/bloop.wav";
const String Spot3 = "resources/blurp.wav";
const String Spot4 = "resources/boing.wav";

std::string toStringInt(int value);
std::string toStringDouble(double value);

String getTextureNameByColor(Color col);