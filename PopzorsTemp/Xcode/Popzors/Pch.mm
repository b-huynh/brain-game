#include "Pch.h"

#include <string>
#include <sstream>
#include <cstdlib>
#include <sys/stat.h>


int randRangeInt(int min, int max)
{
    return min + rand () % (max - min + 1);
}

double randRangeDouble(double min, double max)
{
    return min + static_cast<double>(rand()) / RAND_MAX * (max - min);
}

Color getRandomPotColor()
{
	int choice = std::rand() % 4 + 1;
    
	if (choice == 1) return Cpot1;
	if (choice == 2) return Cpot2;
	if (choice == 3) return Cpot3;
	if (choice == 4) return Cpot4;
    return Color();
}

int getColorId(Color color)
{
    if (color == Cpot1) return 1;
    if (color == Cpot2) return 2;
    if (color == Cpot3) return 3;
    if (color == Cpot4) return 4;
    return -999;
}

std::string getSoundAccordingToColor(Color col)
{
    if (col == Cpot1) return Spot1;
    if (col == Cpot2) return Spot2;
    if (col == Cpot3) return Spot3;
    if (col == Cpot4) return Spot4;
    if (col == BLAND_COLOR) return SpotBland;
    return "";
}

std::string toStringInt(int value)
{
    std::stringstream ss;
	ss << value;
	return ss.str();
}

std::string toStringDouble(double value)
{
    std::stringstream ss;
	ss << value;
	return ss.str();
}

String getTextureNameByColor(Color col)
{
    if (col == Cpot1) return "PodPinkMat";
    if (col == Cpot2) return "PodBlueMat";
    if (col == Cpot3) return "PodGreenMat";
    if (col == Cpot4) return "PodYellowMat";
    if (col == BLAND_COLOR) return "PoppyBlandMat";
    if (col == BASE_COLOR) return "PoppyBaseMat";
    return "PoppyBaseMat";
}

std::string getSaveDir()
{
    char * dir = getenv("HOME");
    std::string result = "";
    if (dir)
        result = std::string(dir) + "/braingame/";
    else
        return "";
    
    mkdir(result.c_str(), 0777);
    
    return result;
}