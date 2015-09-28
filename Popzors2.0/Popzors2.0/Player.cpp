#include "Player.h"
#include <fstream>
#include <sstream>
#include <iostream>
#include <unistd.h>

Player::Player()
: seed(), name(""), totalElapsed(0.0), score(0), numCorrect(0), totalProblems(0), level(1), numConsecutiveSuccess(0)
{}

void Player::reset() {
    numCorrect = 0;
    totalProblems = 0;
}

void Player::resetLevel() {
    level = 1;
}

void Player::updateSuccess(double accuracy)
{
    if (accuracy == PLAYER_SUCCESS)
        ++numConsecutiveSuccess;
    else
        numConsecutiveSuccess = 0;
}

void Player::updateLevel(double accuracy)
{
    PlayerResult levelResults;
    levelResults.playerLevel = this->level;
    levelResults.numCorrect = this->numCorrect;
    levelResults.totalProblems = this->totalProblems;
    levelResults.accuracy = accuracy;
    progression.push_back(levelResults);

    if (accuracy == PLAYER_FAILURE) {
        if (level > 1) --level;
    }
    else {
        if (numConsecutiveSuccess >= levelUpCeiling) {
            numConsecutiveSuccess = 0;
            ++level;
        }
    }
}

//Returns false if failed to save to file, true otherwise
bool Player::saveProgress(std::string file)
{
    file = getSaveDir() + file;
    std::ofstream out;
    out.open(file.c_str(), std::ofstream::out | std::ofstream::trunc);
    
    if (out.good()) {
        out << progression.size() << std::endl;
        for (int i = 0; i < progression.size(); ++i) {
            out << "{" << std::endl;
            out << "    ProgressionID : "  << i << std::endl;
            out << "    playerLevel : " << progression[i].playerLevel << std::endl;
            out << "    numCorrect : " << progression[i].numCorrect << std::endl;
            out << "    totalProblems : " << progression[i].totalProblems << std::endl;
            out << "    accuracy : " << progression[i].accuracy << std::endl;
            out << "}" << std::endl;
        }
        out.close();
    }
    else {
        out.close();
        return false;
    }
    
    return true;
}

//TODO: Fix Parsing. THIS FUNCTION DOES NOT WORK YET.
//Returns false if failed to load from file, true otherwise
bool Player::loadProgress(std::string file)
{
    file = getSaveDir() + file;
    std::ifstream in (file.c_str());
    
    if (!in.good()) return false;
    
    int size;
    std::string line;
    while (std::getline(in,line)) {
        std::istringstream ss (line);
        std::string key;
        std::string seperator;
        int intVal;
        int id;
        
        ss >> size;
        progression.resize(size);
        
        ss >> key;
        if (key == "{") {
            PlayerResult res;
            while (key != "}") {
                in >> key;
                in >> seperator;
                in >> intVal;
                
                if (key == "ProgressionID") id = intVal;
                if (key == "playerLevel") res.playerLevel = intVal;
                if (key == "numCorrect") res.numCorrect = intVal;
                if (key == "totalProblems") res.totalProblems = intVal;
                if (key == "accuracy") res.accuracy = intVal;
            }
            progression.push_back(res);
        }
    }
    
    return false;
}