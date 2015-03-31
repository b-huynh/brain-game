#ifndef __Pch__
#define __Pch__

#include "OgreFramework.h"

Ogre::ColourValue intToFloatColor (int r, int g, int b, int a);

// Special Constants
const Ogre::ColourValue BASE_COLOR (1.0,1.0,1.0,1.0);
const Ogre::ColourValue BLAND_COLOR (0.0,192,192,255);
const Ogre::ColourValue SELECT_COLOR = intToFloatColor(255,255,0,255);

const Ogre::ColourValue Cpot1 = intToFloatColor(192,0,0,255);
const Ogre::ColourValue Cpot2 = intToFloatColor(0,192,0,255);
const Ogre::ColourValue Cpot3 = intToFloatColor(0,0,192,255);
const Ogre::ColourValue Cpot4 = intToFloatColor(192,192,0,255);

const Ogre::ColourValue GROUND_COLOR = intToFloatColor(225, 225, 225, 100);
const Ogre::ColourValue FEEDBACK_COLOR_GOOD = intToFloatColor(64,255,64,100);
const Ogre::ColourValue FEEDBACK_COLOR_BAD = intToFloatColor(255,64,64,100);

const double SIGNAL_START = 2.0;
const double SIGNAL_LENGTH = 0.5;

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

const double POPPY_RADIUS = 0.3;
const double POT_RADIUS = 1.5;
const double POT_HEIGHT = 0.5;

int randRangeInt(int min, int max);
double randRangeDouble(double min, double max);
Ogre::ColourValue getRandomPotColor();
std::string getSoundAccordingToColor(Ogre::ColourValue col);
int getColorId(Ogre::ColourValue color);

const std::string Spot1 = "resources/blip.wav";
const std::string Spot2 = "resources/bloop.wav";
const std::string Spot3 = "resources/blurp.wav";
const std::string Spot4 = "resources/boing.wav";
const std::string SpotBland = "resources/womp.wav";
const std::string SOUNDFILE_NEGATIVE_FEEDBACK = "resources/chimedown.wav";
const std::string SOUNDFILE_POSITIVE_FEEDBACK = "resources/chimeup.wav";

std::string toStringInt(int value);
std::string toStringDouble(double value);

String getTextureNameByColor(Ogre::ColourValue col);

//Save/Load File Directory (for debugging);
std::string getSaveDir();

#endif