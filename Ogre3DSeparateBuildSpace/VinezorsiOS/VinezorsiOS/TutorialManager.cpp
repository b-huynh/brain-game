//
//  TutorialManager.cpp
//  Vinezors2.0
//
//  Created by Calvin Phung on 5/15/14.
//
//

#include "TutorialManager.h"
#include "Player.h"

extern Util::ConfigGlobal globals;

#include <iostream>
#include <sstream>

// Inserts newlines by extracting each word and counting the character limit
// All previous whitespaces are removed
std::string insertNL(std::string message)
{
#if defined(OGRE_IS_IOS)
    const int NUM_CHAR_PER_NEWLINE = 20;
#else
    const int NUM_CHAR_PER_NEWLINE = 25;
#endif
    
    std::stringstream ss(message);
    
    int linelen = 0;
    std::string ret = "";
    std::string input;
    while (ss >> input)
    {
#if defined(OGRE_IS_IOS)
        int sublen = input.length() + 2; // i-pad makes space character two-wide
#else
        int sublen = input.length() + 1; // Add one for the space character
#endif
        if (linelen + sublen <= 25)
        {
            if (linelen > 0)
                ret += " ";
            ret += input;
            linelen += sublen;
        }
        else
        {
            ret += "\n";
            ret += input;
            linelen = input.length();
        }
        
        while (ss.peek() == '\n') // try to find hardcoded newlines and put them in
        {
            ret += ss.get(); // extract newline
            linelen = 0;
        }
    }
    return ret;
}

// Returns a random snarky comment for the end of the level
std::string getEndFeedbackText(int accuracy, Evaluation eval)
{
    std::vector<std::string> comments;
    
    // At this point, we can tell them how they did with Apollo
    if (accuracy < 65)
    {
        comments.push_back("It seems like you're not\nready for this run cadet.");
        comments.push_back("Good effort, but you're\nstill not ready cadet.");
        comments.push_back("Looks like you need\nmore practice.");
        comments.push_back("No good cadet,\nyou'll need to do better.");
    }
    else if (accuracy < 75 || eval != PASS)
    {
        comments.push_back("You almost made\nit through!");
        comments.push_back("Blast! You were so close.");
        comments.push_back("Keep trying cadet.\nYou almost made it!");
        comments.push_back("Graaah, almost made it!\nNext time for sure!");
        comments.push_back("I'm starting to\nnot like that door.");
    }
    else if (accuracy < 85)
    {
        comments.push_back("Good cadet,\nbut you can do better.");
        comments.push_back("Try again,\nthis time faster.");
<<<<<<< HEAD
        comments.push_back("You made it cadet.\nYou could do better though.");
=======
        comments.push_back("You made it cadet. You\ncould do better though.");
>>>>>>> 2d6a258041a97a8de0cb96b4aceb6c9560c0c3cc
        comments.push_back("Is that's your best cadet?\nBecause I don't think so.");
        //comments.push_back("You cleared it cadet! A bit\nlackluster on the finish though."); // too long
        comments.push_back("You cleared it cadet! A bit\nlackluster on the finish.");
    }
    else if (accuracy < 100)
    {
        comments.push_back("Couldn't have done it\nbetter myself.");
        comments.push_back("Not bad, not bad at all.");
        comments.push_back("Well done ace!");
        comments.push_back("You've done well cadet,\nyou've done well");
        comments.push_back("Great job cadet!");
    }
    else
    {
        comments.push_back("Your work here is done.");
        comments.push_back("The tunnel is cleared.\nLet's move out.");
        comments.push_back("Perfect. Simply perfect.");
<<<<<<< HEAD
        comments.push_back("Well done ace!");
=======
        comments.push_back("You're a cadet no longer.\nLet's go pilot!");
>>>>>>> 2d6a258041a97a8de0cb96b4aceb6c9560c0c3cc
    }
    
    if (comments.size() <= 0)
        return "";
    int r = std::rand() % comments.size();
    return comments[r];
}

TutorialManager::TutorialManager()
: popupOverlay(NULL), popupWindowBackground(NULL), popupSubWindowBackground(NULL), queue(), slides(), visitedSlide(), enableSlides(true), slideNo(0), yoffset(0.0), startTimer(0.0f), additionalText(""), specialStage(false), specialSession(false)
{
    visitedSlide = std::vector<bool>(NUM_TUTORIAL_SLIDES, false);
    alloc();
}

TutorialManager::~TutorialManager()
{
    dealloc();
}

bool TutorialManager::isEnabled() const
{
    return enableSlides;
}

void TutorialManager::enable()
{
    enableSlides = true;
    
    // For now, re-enabling the manager resets the slides visited
    for (int i = 0; i < visitedSlide.size(); ++i)
        visitedSlide[i] = false;
}

void TutorialManager::disable()
{
    enableSlides = false;
}

// Returns the set of slides depending on the lesson type
std::vector<TutorialSlide> TutorialManager::getSlides(TutorialSlidesType type) const
{
    std::vector<TutorialSlide> ret;
#ifdef DEMO_BUILD
    return ret;
#else
    switch (type)
    {
        case TUTORIAL_SLIDES_TEXTBOX_NAVIGATION:
            ret.push_back(TutorialSlide("", "General/TutorialTextbox1-1", ""));
            ret.push_back(TutorialSlide("", "General/TutorialTextbox1-2", ""));
            ret.push_back(TutorialSlide("", "General/TutorialTextbox1-3", ""));
            break;
        case TUTORIAL_SLIDES_TEXTBOX_1BACK:
            ret.push_back(TutorialSlide("", "General/TutorialTextbox2-1", ""));
            ret.push_back(TutorialSlide("", "General/TutorialTextbox2-2", ""));
            ret.push_back(TutorialSlide("", "General/TutorialTextbox2-3a", ""));
            ret.push_back(TutorialSlide("", "General/TutorialTextbox2-3b", ""));
            ret.push_back(TutorialSlide("", "General/TutorialTextbox2-4a", ""));
            ret.push_back(TutorialSlide("", "General/TutorialTextbox2-4b", ""));
            ret.push_back(TutorialSlide("", "General/TutorialTextbox2-5", ""));
            ret.push_back(TutorialSlide("", "General/TutorialTextbox2-6", ""));
            break;
        case TUTORIAL_SLIDES_TEXTBOX_2BACK:
            ret.push_back(TutorialSlide("", "General/TutorialTextbox3-1", ""));
            ret.push_back(TutorialSlide("", "General/TutorialTextbox3-2a", ""));
            ret.push_back(TutorialSlide("", "General/TutorialTextbox3-2b", ""));
            break;
        case TUTORIAL_SLIDES_TEXTBOX_HOLDOUT:
            ret.push_back(TutorialSlide("", "General/TutorialTextboxHoldout1", ""));
            ret.push_back(TutorialSlide("", "General/TutorialTextboxHoldout2", ""));
            break;
        case TUTORIAL_SLIDES_TEXTBOX_SEEING_MATCH:
            ret.push_back(TutorialSlide("", "General/TutorialTextboxSeeingMatch", ""));
            break;
        case TUTORIAL_SLIDES_TEXTBOX_MISSING_MATCH:
            ret.push_back(TutorialSlide("", "General/TutorialTextboxMissingMatch", ""));
            break;
        case TUTORIAL_SLIDES_TEXTBOX_INCORRECT_MATCH:
            ret.push_back(TutorialSlide("", "General/TutorialTextboxIncorrectMatch", ""));
            break;
        case TUTORIAL_SLIDES_TEXTBOX_OBSTACLE:
            ret.push_back(TutorialSlide("", "General/TutorialTextboxObstacle", ""));
            break;
        case TUTORIAL_SLIDES_TEXTBOX_SOUND_ONLY:
            ret.push_back(TutorialSlide("", "General/TutorialTextboxSoundOnly", ""));
            break;
        case TUTORIAL_SLIDES_TEXTBOX_FUEL:
            ret.push_back(TutorialSlide("", "General/TutorialTextboxFuelWarning", ""));
            break;
        case TUTORIAL_END_OF_STAGE:
            //ret.push_back(TutorialSlide("   \nResults", "General/TutorialBackdropLined", ""));
            ret.push_back(TutorialSlide("", "General/TutorialBackdropLined", ""));
            break;
        case TUTORIAL_END_OF_SESSION:
            ret.push_back(TutorialSlide("\n\n  Good work today!\n\n Make sure you take\n     a break.\n\n   You earned it!", "General/TutorialBackdrop", ""));
            break;
        default:
            break;
    }
    return ret;
#endif
}

// Load set of slides in a queue with a timer that when expired, will load the slides up
void TutorialManager::prepareSlides(TutorialSlidesType type, float startTimer)
{
    specialSession = false;
    specialStage = false;
    if (type == TUTORIAL_END_OF_SESSION)
        specialSession = true;
    else if (type == TUTORIAL_END_OF_STAGE)
    {
        specialStage = true;
        specialMode = 0;
        specialTimer = 0.0f;
        textAnimationTimer = 0.0f;
        dinged = false;
        fireworkNode = NULL;
        fireworkEffects.clear();
        fireworkTimer = 0.0f;
    }
    if ((isEnabled() && !visitedSlide[type]) || isSpecial())
    {
        prepareSlides(getSlides(type), startTimer);
        visitedSlide[type] = true;
    }
}

void TutorialManager::prepareSlides(const std::vector<TutorialSlide> & slides, float startTimer)
{
    this->startTimer = startTimer;
    if (this->startTimer <= 0.0f)
    {
        // If timer is not set, then immediately set the slides for show
        setSlides(slides);
    }
    else
    {
        // Set the slides up in queue for timer
        for (int i = 0; i < slides.size(); ++i)
            this->queue.push_back(slides[i]);
    }
}

// Appends the set of slides if it has not been seen yet
void TutorialManager::setSlides(TutorialSlidesType type)
{
    specialSession = false;
    specialStage = false;
    if (type == TUTORIAL_END_OF_SESSION)
        specialSession = true;
    else if (type == TUTORIAL_END_OF_STAGE)
    {
        specialStage = true;
        specialMode = 0;
        specialTimer = 0.0f;
        textAnimationTimer = 0.0f;
        dinged = false;
        fireworkNode = NULL;
        fireworkEffects.clear();
        fireworkTimer = 0.0f;
    }
    if ((isEnabled() && !visitedSlide[type]) || isSpecial())
    {
        setSlides(getSlides(type));
        visitedSlide[type] = true;
    }
}
// Appends the set of slides, and if the slides do get set, set this overlay to be visible
void TutorialManager::setSlides(const std::vector<TutorialSlide> & slides)
{
    for (int i = 0; i < slides.size(); ++i)
        this->slides.push_back(slides[i]);
    slideNo = 0;
    if (slides.size() > 0)
    {
        show();
        yoffset = -0.50;
        adjust();
        updateOverlay();
    }
}

bool TutorialManager::hasPreviousSlide() const
{
    int newSlideNo = slideNo - 1;
    return newSlideNo >= 0 && newSlideNo < slides.size();
}

bool TutorialManager::hasNextSlide() const
{
    int newSlideNo = slideNo + 1;
    return newSlideNo >= 0 && newSlideNo < slides.size();
}

bool TutorialManager::setPreviousSlide()
{
    slideNo--;
    if (slideNo < 0)
    {
        slideNo = 0;
        return false;
    }
    updateOverlay();
    return true;
}

void TutorialManager::setNextSlide()
{
    slideNo++;
    if (slideNo >= slides.size()) slideNo = slides.size() - 1;
    updateOverlay();
}

void TutorialManager::updateOverlay()
{
    std::string text = slides[slideNo].message;
    std::string subwindowbg = slides[slideNo].background;
    std::string screenbg = slides[slideNo].screen;
    popupText1->setCaption(text);
    popupText2->setCaption(additionalText);
    popupText3->setCaption(additionalValue);
    popupSubWindowBackground->setMaterialName(subwindowbg);
    if (isSpecial())
    {
        popupGoRightBackground->setMaterialName("General/ExitButton");
        popupExitBackground->setMaterialName("");
    }
    else
    {
        popupGoRightBackground->setMaterialName("General/ButtonGoDown");
        popupExitBackground->setMaterialName("General/ExitButton");
    }
}

void TutorialManager::update(float elapsed, Player* player)
{
    if (specialStage)
    {
        popupText2->setPosition(0.06, 0.065);
        popupText2->setCharHeight(0.022 * FONT_SZ_MULT);
        popupText3->setPosition(0.375, 0.065);
        popupText3->setCharHeight(0.022 * FONT_SZ_MULT);
        
        eval = player->getTunnel()->getEval();
        StageMode mode = player->getTunnel()->getMode();
        
        // Get actual values of player performance
        int numWrong = player->getNumWrongTotal();
        int numMissed = player->getNumMissedTotal();
        int numPickups = player->getNumPickupsTotal();
        int numCorrect = player->getNumCorrectTotal();
        int tleft = player->getTunnel()->getStageTime() - player->getTunnel()->getTotalElapsed() - player->getTunnel()->getTimePenalty();
        if (tleft < 0) tleft = 0;
        int tdisp = 0;
        if (specialMode > 4 && eval == PASS)
            tdisp = tleft;
        
        // Go through animation loop, where specialMode tracks which value the special timer
        // is currently animating through.
        //
        // It goes in order from top to bottom.
        if (specialMode == 0)
        {
            // Animate number of bad zaps
            specialTimer += 30 * elapsed;
            if (specialTimer > numWrong)
            {
                specialMode++;
                specialTimer = 0;
            }
            else
                numWrong = specialTimer;
            numMissed = 0;
            numPickups = 0;
            numCorrect = 0;
        }
        else if (specialMode == 1)
        {
            // Animate number of missed targets
            specialTimer += 30 * elapsed;
            if (specialTimer > numMissed)
            {
                specialMode++;
                specialTimer = 0;
            }
            else
                numMissed = specialTimer;
            numPickups = 0;
            numCorrect = 0;
        }
        else if (specialMode == 2)
        {
            // Animate number of picked up non-targets
            specialTimer += 60 * elapsed;
            if (specialTimer > numPickups)
            {
                specialMode++;
                specialTimer = 0;
            }
            else
                numPickups = specialTimer;
            numCorrect = 0;
        }
        else if (specialMode == 3)
        {
            // Animate the number of zapped targets
            if (mode == STAGE_MODE_RECESS)
                specialTimer += 60 * elapsed;
            else
                specialTimer += 15 * elapsed;
            if (specialTimer > numCorrect)
            {
                specialMode++;
                
                // At this point, we can tell them how they did with Apollo
                popupText4->setCaption(getEndFeedbackText(accuracy, eval));
                
                // Skip animating the stage timer if they didn't pass the level.
                // (The stage timer adds points to the player's score)
                if (eval != PASS)
                    specialMode++;
                specialTimer = 0;
            }
            else
                numCorrect = specialTimer;
        }
        else if (specialMode == 4)
        {
            // Animate the stage timer counting the time left on clock down to zero.
            // This only applies to people who win and get bonus points for extra time.
            specialTimer += 60 * elapsed;
            if (specialTimer > tleft)
            {
                specialMode++;
                specialTimer = 0;
            }
            else
                tdisp = specialTimer;
        }
        
        // Compute accuracy and other important criterias to display
        int numCriteria = player->getTunnel()->getNumRequiredCriteria();
        int possiblePickups = player->getNumSafeTotal() + player->getNumWrongTotal();
        accuracy = 0;
        if (mode == STAGE_MODE_RECESS)
        {
            if (player->getNumCorrectTotal() + numWrong + numMissed > 0)
                accuracy = static_cast<float>(numCorrect) / (player->getNumCorrectTotal() + numWrong + numMissed) * 100;
        }
        else
        {
            if (numCriteria + numWrong + numMissed > 0)
                accuracy = static_cast<float>(numCorrect) / (numCriteria + numWrong + numMissed) * 100;
        }
        
        // Compute score using constants.
        const int SCORE_PER_TICK = 100;
        const int NONZAP_PICKUP = 50;
        int score = numCorrect * player->getScoring() + numPickups * NONZAP_PICKUP + SCORE_PER_TICK * tdisp;
        
        Ogre::TextAreaOverlayElement* label2 = (Ogre::TextAreaOverlayElement*)OgreFramework::getSingletonPtr()->m_pOverlayMgr->getOverlayElement("StageTextAreaLabel2");
        label2->setCaption(Util::toStringInt(tleft - tdisp));
        
        Ogre::TextAreaOverlayElement* label7 = (Ogre::TextAreaOverlayElement*)OgreFramework::getSingletonPtr()->m_pOverlayMgr->getOverlayElement("StageTextAreaLabel7");
        label7->setCaption("");
        
        if (specialMode >= 4)
        {
            // Play fireworks and feedback sounds based on their performance
            bool dingSound = true;
            bool genFireworks = false;
            bool pulsateTextSize = false;

            if (accuracy >= 85)
            {
                genFireworks = true;
                pulsateTextSize = true;
            }
            
            // Play the ding sound or fail sound
            if (dingSound && !dinged)
            {
                OgreOggISound* sound = NULL;
                if (accuracy < 75)
                    sound = OgreFramework::getSingletonPtr()->m_pSoundMgr->getSound("SoundBadFeedback");
                else
                    sound = OgreFramework::getSingletonPtr()->m_pSoundMgr->getSound("SoundDing");
                player->playSound(sound);
                dinged = true;
            }
            
            // Generate the fireworks in succession
            if (genFireworks)
            {
                if (!fireworkNode)
                    fireworkNode = OgreFramework::getSingletonPtr()->m_pSceneMgrMain->getRootSceneNode()->createChildSceneNode("FireworkNode");
                else
                {
                    fireworkTimer += elapsed;
                    if (fireworkTimer >= 1.0f && fireworkEffects.size() < 3)
                    {
                        Ogre::ParticleSystem* fireworkEffect = fireworkNode->getCreator()->createParticleSystem("FireworkEffect" + Util::toStringInt(fireworkEffects.size()), "StageEnding/Fireworks");
                        Vector3 dir = player->getCamForward();
                        if (fireworkEffects.size() == 0)
                        {
                            dir = dir - player->getCamRight() * 0.30;
                            dir = dir + player->getCamUpward() * 0.10;
                        }
                        else if (fireworkEffects.size() == 1)
                        {
                            dir = dir + player->getCamUpward() * 0.25;
                        }
                        else
                        {
                            dir = dir + player->getCamRight() * 0.30;
                            dir = dir + player->getCamUpward() * 0.10;
                        }
                        fireworkNode->setPosition(player->getCamPos() + dir);
                        fireworkNode->attachObject(fireworkEffect);
                        fireworkEffects.push_back(fireworkEffect);
                        fireworkTimer = 0.0f;
                        player->playFireworkSound();
                    }
                }
            }
        }

        // Set the text computed from the animator previously
        std::string completed = eval == PASS ? "yes" : "no";
        
        setAdditionalText("Mistakes\nMissed\nPickups\nZapped\n\nAccuracy\nCompleted\nScore");
        if (player->getTunnel()->getMode() == STAGE_MODE_RECESS)
        {
            setAdditionalValue("-\n-\n" +
                               Util::toStringInt(numCorrect) + " / " + Util::toStringInt(player->getNumCorrectTotal() + player->getNumMissedTotal()) + "\n" +
                               "-\n" +
                               "\n" +
                               Util::toStringInt(accuracy) + "%\n" +
                               completed + "\n" +
                               Util::toStringInt(score));
        }
        else
        {
            setAdditionalValue(Util::toStringInt(numWrong) + "\n" +
                               Util::toStringInt(numMissed) + "\n" +
                               Util::toStringInt(numPickups) + " / " + Util::toStringInt(possiblePickups) + "\n" +
                               Util::toStringInt(numCorrect) + " / " + Util::toStringInt(numCriteria) + "\n" +
                               "\n" +
                               Util::toStringInt(accuracy) + "%\n" +
                               completed + "\n" +
                               Util::toStringInt(score));
        }
        // Set after to get actual accuracy in case player closes window and still sees correct post-feedback
        accuracy = player->getAccuracy() * 100;
    }
    else if (specialSession)
    {
        popupText2->setPosition(0.06, 0.16);
        popupText2->setCharHeight(0.025 * FONT_SZ_MULT);
        popupText3->setPosition(0.375, 0.16);
        popupText3->setCharHeight(0.025 * FONT_SZ_MULT);
    }
    // If a start timer has been set, update it
    // and check to see if it is ready to show.
    if (startTimer > 0.0f)
    {
        startTimer -= elapsed;
        if (startTimer <= 0.0f)
        {
            startTimer = 0.0f;
            setSlides(queue);
            queue.clear();
        }
    }
    
    if (isVisible())
    {
        if (yoffset < 0.0)
        {
            float yspeed = (-yoffset) * 5.0;
            if (yspeed < 0.25) yspeed = 0.25;
            yoffset += (yspeed * elapsed);
            if (yoffset > 0.0)
                yoffset = 0.0;
            adjust();
        }
    }
}

bool TutorialManager::hasVisitedSlide(TutorialSlidesType type) const
{
    return visitedSlide[type];
}

bool TutorialManager::isSpecial() const
{
    return specialSession || specialStage;
}

void TutorialManager::hide()
{
    popupOverlay->hide();
}

void TutorialManager::show()
{
    popupOverlay->show();
}

bool TutorialManager::isHidden() const
{
    return !popupOverlay->isVisible();
}

bool TutorialManager::isVisible() const
{
    return popupOverlay->isVisible();
}

std::string TutorialManager::queryButtons(Vector2 target) const
{
    Vector2 comp = globals.convertToPercentScreen(target);
    for (int i = 0; i < buttons.size(); ++i)
    {
        if ((buttons[i].overlay && buttons[i].overlay->isVisible()) && buttons[i].isInside(comp) && buttons[i].active)
        {
            return buttons[i].name;
        }
    }
    return "";
}

bool TutorialManager::processInput(Vector2 target)
{
    std::string query = queryButtons(target);
    if (query == "goleft")
    {
        if (!isSpecial())
            setPreviousSlide();
    }
    else if (query == "goright")
    {
        if (isSpecial())
        {
            if (specialStage)
            {
                Ogre::TextAreaOverlayElement* label7 = (Ogre::TextAreaOverlayElement*)OgreFramework::getSingletonPtr()->m_pOverlayMgr->getOverlayElement("StageTextAreaLabel7");
                label7->setColour(ColourValue::ColourValue(1.0, 1.0, 0.0, 1.0));
                label7->setCharHeight(0.025 * FONT_SZ_MULT);
<<<<<<< HEAD
                label7->setCaption("Try again or continue?");
=======
                label7->setCaption("\nTry again or continue?");
>>>>>>> 2d6a258041a97a8de0cb96b4aceb6c9560c0c3cc

                if (fireworkNode)
                {
                    for (int i = 0; i < fireworkEffects.size(); ++i)
                        fireworkNode->getCreator()->destroyParticleSystem(fireworkEffects[i]);
                    fireworkNode->removeAndDestroyAllChildren();
                    fireworkNode->getCreator()->destroySceneNode(fireworkNode);
                    fireworkNode = NULL;
                }
            }
            //if (specialSession) exit(0);
            specialSession = false;
            specialStage = false;
            setAdditionalValue("");
            setAdditionalText("");
            popupText4->setCaption("");
            slides.clear();
            slideNo = 0;
            hide();
        }
        else
        {
            if (hasNextSlide())
                setNextSlide();
            else
            {
                slides.clear();
                slideNo = 0;
                hide();
            }
        }
    }
    else if (query == "exit")
    {
        if (!isSpecial()) {
            slides.clear();
            slideNo = 0;
            hide();
        }
    }
    return query != "";
}

void TutorialManager::adjust()
{
    popupWindowBackground->setMetricsMode(GMM_RELATIVE);
    popupWindowBackground->setPosition(0.250, yoffset + 0.25);
    popupWindowBackground->setDimensions(0.50, 0.50);
    
    popupSubWindowBackground->setMetricsMode(GMM_RELATIVE);
    popupSubWindowBackground->setPosition(0.250, yoffset + 0.25);
    popupSubWindowBackground->setDimensions(0.50, 0.50);
    
    float bw = 0.075;
    float bh = bw * globals.screenWidth / globals.screenHeight;
    //buttons[BUTTON_GOLEFT].setButton("goleft", popupOverlay, GMM_RELATIVE, Vector2(0.175, 0.425), Vector2(bw, bh), popupGoLeftBackground, NULL);
    buttons[BUTTON_GORIGHT].setButton("goright", popupOverlay, GMM_RELATIVE, Vector2(0.400, 0.425), Vector2(bw, bh), popupGoRightBackground, NULL);
    buttons[BUTTON_EXIT].setButton("exit", popupOverlay, GMM_RELATIVE, Vector2(0.300, 0.425), Vector2(bw, bh), popupExitBackground, NULL);
    
    popupText1->setMetricsMode(GMM_RELATIVE);
    popupText1->setAlignment(TextAreaOverlayElement::Left);
    popupText1->setPosition(0.050, 0.05);
    popupText1->setCharHeight(0.025 * FONT_SZ_MULT);
    popupText1->setColour(ColourValue::ColourValue(1.0, 1.0, 1.0));
    popupText1->setFontName("MainSmall");
    
    popupText2->setMetricsMode(GMM_RELATIVE);
    popupText2->setAlignment(TextAreaOverlayElement::Left);
    popupText2->setColour(ColourValue::ColourValue(1.0, 1.0, 1.0));
    popupText2->setFontName("MainSmall");
    
    popupText3->setMetricsMode(GMM_RELATIVE);
    popupText3->setAlignment(TextAreaOverlayElement::Right);
    popupText3->setColour(ColourValue::ColourValue(1.0, 1.0, 0.0));
    popupText3->setFontName("MainSmall");
    
    popupText4->setMetricsMode(GMM_RELATIVE);
    popupText4->setAlignment(TextAreaOverlayElement::Center);
    popupText4->setPosition(0.225, 0.385);
    popupText4->setCharHeight(0.022 * FONT_SZ_MULT);
    popupText4->setColour(ColourValue::ColourValue(1.0, 1.0, 0.0));
    popupText4->setFontName("MainSmall");
}

void TutorialManager::setAdditionalText(std::string text)
{
    additionalText = text;
    updateOverlay();
}

void TutorialManager::setAdditionalValue(std::string text)
{
    additionalValue = text;
    updateOverlay();
}

void TutorialManager::dealloc()
{
    if (popupOverlay)
        OgreFramework::getSingletonPtr()->m_pOverlayMgr->destroy(popupOverlay);
    if (popupSubWindowBackground)
        OgreFramework::getSingletonPtr()->m_pOverlayMgr->destroyOverlayElement(popupSubWindowBackground);
    if (popupWindowBackground)
        OgreFramework::getSingletonPtr()->m_pOverlayMgr->destroyOverlayElement(popupWindowBackground);
    if (popupGoLeftBackground)
        OgreFramework::getSingletonPtr()->m_pOverlayMgr->destroyOverlayElement(popupGoLeftBackground);
    if (popupGoRightBackground)
        OgreFramework::getSingletonPtr()->m_pOverlayMgr->destroyOverlayElement(popupGoRightBackground);
    if (popupText1)
        OgreFramework::getSingletonPtr()->m_pOverlayMgr->destroyOverlayElement(popupText1);
    if (popupText2)
        OgreFramework::getSingletonPtr()->m_pOverlayMgr->destroyOverlayElement(popupText2);
    if (popupText3)
        OgreFramework::getSingletonPtr()->m_pOverlayMgr->destroyOverlayElement(popupText3);
    if (popupText4)
        OgreFramework::getSingletonPtr()->m_pOverlayMgr->destroyOverlayElement(popupText4);
    popupSubWindowBackground = NULL;
    popupWindowBackground = NULL;
    popupGoLeftBackground = NULL;
    popupGoRightBackground = NULL;
    popupText1 = NULL;
    popupText2 = NULL;
    popupText3 = NULL;
    popupText4 = NULL;
}

void TutorialManager::alloc()
{
    popupOverlay = OgreFramework::getSingletonPtr()->m_pOverlayMgr->create("TutorialPopupOverlay");
    popupWindowBackground = static_cast<PanelOverlayElement*>(OgreFramework::getSingletonPtr()->m_pOverlayMgr->createOverlayElement("Panel", "TutorialPopupWindowBackground"));
    popupSubWindowBackground = static_cast<PanelOverlayElement*>(OgreFramework::getSingletonPtr()->m_pOverlayMgr->createOverlayElement("Panel", "TutorialPopupSubWindowBackground"));
    
    popupGoLeftBackground = static_cast<PanelOverlayElement*>(OgreFramework::getSingletonPtr()->m_pOverlayMgr->createOverlayElement("Panel", "TutorialPopupGoLeftBackground"));
    popupGoRightBackground = static_cast<PanelOverlayElement*>(OgreFramework::getSingletonPtr()->m_pOverlayMgr->createOverlayElement("Panel", "TutorialPopupGoRightBackground"));
    popupExitBackground = static_cast<PanelOverlayElement*>(OgreFramework::getSingletonPtr()->m_pOverlayMgr->createOverlayElement("Panel", "TutorialPopupExitBackground"));

    // Allocate Resources
    popupText1 = static_cast<TextAreaOverlayElement*>(OgreFramework::getSingletonPtr()->m_pOverlayMgr->createOverlayElement("TextArea", "TutorialPopupText1"));
    popupText2 = static_cast<TextAreaOverlayElement*>(OgreFramework::getSingletonPtr()->m_pOverlayMgr->createOverlayElement("TextArea", "TutorialPopupText2"));
    popupText3 = static_cast<TextAreaOverlayElement*>(OgreFramework::getSingletonPtr()->m_pOverlayMgr->createOverlayElement("TextArea", "TutorialPopupText3"));
    popupText4 = static_cast<TextAreaOverlayElement*>(OgreFramework::getSingletonPtr()->m_pOverlayMgr->createOverlayElement("TextArea", "TutorialPopupText4"));
    popupOverlay->add2D(popupSubWindowBackground);
    popupOverlay->add2D(popupWindowBackground);
    popupSubWindowBackground->addChild(popupGoLeftBackground);
    popupSubWindowBackground->addChild(popupGoRightBackground);
    popupSubWindowBackground->addChild(popupExitBackground);
    popupSubWindowBackground->addChild(popupText1);
    popupSubWindowBackground->addChild(popupText2);
    popupSubWindowBackground->addChild(popupText3);
    popupSubWindowBackground->addChild(popupText4);
    
    buttons = std::vector<HudButton>(3);
    
    // Set this overlay as the frontmost item on the screen
    adjust();
    popupOverlay->setZOrder(300);
}

std::ostream& operator<<(std::ostream& out, const TutorialManager& tutorialMgr)
{
    std::cout << "Saving Tutorial Settings\n";
    
    out << tutorialMgr.enableSlides << " " << tutorialMgr.visitedSlide.size() << " ";
    for (int i = 0; i < tutorialMgr.visitedSlide.size(); ++i)
        out << tutorialMgr.visitedSlide[i] << " ";
     
    return out;
}

std::istream& operator>>(std::istream& in, TutorialManager& tutorialMgr)
{
    std::cout << "Loading Tutorial Settings\n";
    int input;
    in >> input;
    tutorialMgr.enableSlides = input;
    in >> input;
    tutorialMgr.visitedSlide = std::vector<bool>(input);
    for (int i = 0; i < tutorialMgr.visitedSlide.size(); ++i)
    {
        in >> input;
        tutorialMgr.visitedSlide[i] = input;
    }
    tutorialMgr.visitedSlide[TutorialManager::TUTORIAL_END_OF_SESSION] = false;
    
    return in;
}
