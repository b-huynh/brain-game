#include "Pattern.h"

PopzorsPattern::PopzorsPattern(Stage* stage, Player* player)
: stage(stage), player(player), ready(false)
{}

PopzorsPattern::~PopzorsPattern() {
    reset();
}

void PopzorsPattern::reset() // Deals with reseting variables associated to the pattern
{
    player->reset();
    ready = false;
    stage->clear();
}

bool PopzorsPattern::isReady() const
{
    return ready;
}

std::string PopzorsPattern::getName() const
{
    return player->name;
}

void PopzorsPattern::setName(const std::string & value)
{
    player->name = value;
}

double PopzorsPattern::getPlayerCorrectness() const
{
    return static_cast<double>(player->numCorrect) / player->totalProblems;
}

bool PopzorsPattern::save(std::string file)
{
    return false;
}

bool PopzorsPattern::load(std::string file)
{
    return false;
}

bool PopzorsPattern::keyPressed(const OIS::KeyEvent &keyEventRef)
{
    return false;
}
bool PopzorsPattern::keyReleased(const OIS::KeyEvent &keyEventRef)
{
    return false;
}

bool PopzorsPattern::mouseMoved(const OIS::MouseEvent &evt)
{
    return false;
}
bool PopzorsPattern::mousePressed(const OIS::MouseEvent &evt, OIS::MouseButtonID id)
{
    return false;
}
bool PopzorsPattern::mouseReleased(const OIS::MouseEvent &evt, OIS::MouseButtonID id)
{
    return false;
}