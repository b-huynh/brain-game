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
const Color GROUND_COLOR = Color(225, 225, 225, 100);
const Color FEEDBACK_COLOR_GOOD = Color(64,255,64,100);
const Color FEEDBACK_COLOR_BAD = Color(255,64,64,100);

const Number SIGNAL_START = 2.0;
const Number SIGNAL_LENGTH = 0.5;

static int SCREEN_WIDTH = 1024;
static int SCREEN_HEIGHT = 800;
static double BAR_XPOS = 20;
static double BAR_YPOS = 20;
static double BAR_WIDTH = SCREEN_WIDTH - BAR_XPOS * 2;
static int BAR_HEIGHT = SCREEN_HEIGHT / 20;
static int LABEL1_POSX = 0;
static int LABEL1_POSY = SCREEN_HEIGHT - 55;
static int LABEL2_POSX = 0;
static int LABEL2_POSY = SCREEN_HEIGHT - 105;
static int LABEL3_POSX = SCREEN_WIDTH - 55;
static int LABEL3_POSY = SCREEN_HEIGHT - 55;

const double POPPY_RADIUS = 0.40;

int randRangeInt(int min, int max);
double randRangeDouble(double min, double max);
Color getRandomPotColor();
std::string getSoundAccordingToColor(Color col);
int getColorId(Color color);

const std::string Spot1 = "resources/blip.wav";
const std::string Spot2 = "resources/bloop.wav";
const std::string Spot3 = "resources/blurp.wav";
const std::string Spot4 = "resources/boing.wav";
const std::string SpotBland = "resources/womp.wav";
const std::string SOUNDFILE_NEGATIVE_FEEDBACK = "resources/chimedown.wav";
const std::string SOUNDFILE_POSITIVE_FEEDBACK = "resources/chimeup.wav";

std::string toStringInt(int value);
std::string toStringDouble(double value);

String getTextureNameByColor(Color col);

//Save/Load File Directory (for debugging);
std::string getSaveDir();