//
//  TutorialManager.cpp
//  Vinezors2.0
//
//  Created by Calvin Phung on 5/15/14.
//
//

#include "TutorialManager.h"

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

TutorialManager::TutorialManager()
: popupOverlay(NULL), popupWindowBackground(NULL), popupSubWindowBackground(NULL), queue(), slides(), visitedSlide(), enableSlides(true), slideNo(0), yoffset(0.0), startTimer(0.0f), additionalText(""), special(false)
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
            ret.push_back(TutorialSlide("", "General/TutorialTextbox2-2a", ""));
            ret.push_back(TutorialSlide("", "General/TutorialTextbox2-2b", ""));
            ret.push_back(TutorialSlide("", "General/TutorialTextbox2-3", ""));
            ret.push_back(TutorialSlide("", "General/TutorialTextbox2-4", ""));
            ret.push_back(TutorialSlide("", "General/TutorialTextbox2-5", ""));
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
        case TUTORIAL_END_OF_SESSION:
            ret.push_back(TutorialSlide("\n\nThat's it for Today.\n    Please check in.", "General/TutorialBackdrop", ""));
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
    if (type == TUTORIAL_END_OF_SESSION)
        special = true;
    if ((isEnabled() && !visitedSlide[type]) || (type == TUTORIAL_END_OF_SESSION))
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
    if (isEnabled() && !visitedSlide[type])
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
    popupSubWindowBackground->setMaterialName(subwindowbg);
    //popupSlideNoText->setCaption(Util::toStringInt(slideNo + 1) + " / " + Util::toStringInt(slides.size()));
    //if (hasPreviousSlide())
    //    popupGoLeftBackground->setMaterialName("General/ButtonGoUp");
    //else
    //    popupGoLeftBackground->setMaterialName("General/ButtonGoUpGray");
    if (special)
    {
        popupGoRightBackground->setMaterialName("General/ExitButton2");
        popupExitBackground->setMaterialName("");
    }
    else
    {
        popupGoRightBackground->setMaterialName("General/ButtonGoDown");
        popupExitBackground->setMaterialName("General/ExitButton2");
    }
}

void TutorialManager::update(float elapsed)
{
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
        if (!special)
            setPreviousSlide();
    }
    else if (query == "goright")
    {
        if (special)
        {
            special = false;
            exit(0);
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
        if (!special) {
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
    popupWindowBackground->setPosition(0.250, yoffset + 0.20);
    popupWindowBackground->setDimensions(0.50, 0.50);
    //popupWindowBackground->setMaterialName("General/ScreenBackground1");
    
    popupSubWindowBackground->setMetricsMode(GMM_RELATIVE);
    popupSubWindowBackground->setPosition(0.250, yoffset + 0.20);
    popupSubWindowBackground->setDimensions(0.50, 0.50);
    
    float bw = 0.075;
    float bh = bw * globals.screenWidth / globals.screenHeight;
    //buttons[BUTTON_GOLEFT].setButton("goleft", popupOverlay, GMM_RELATIVE, Vector2(0.175, 0.425), Vector2(bw, bh), popupGoLeftBackground, NULL);
    buttons[BUTTON_GORIGHT].setButton("goright", popupOverlay, GMM_RELATIVE, Vector2(0.375, 0.425), Vector2(bw, bh), popupGoRightBackground, NULL);
    buttons[BUTTON_EXIT].setButton("exit", popupOverlay, GMM_RELATIVE, Vector2(0.275, 0.425), Vector2(bw, bh), popupExitBackground, NULL);
    
    popupText1->setMetricsMode(GMM_RELATIVE);
    popupText1->setAlignment(TextAreaOverlayElement::Left);
    popupText1->setPosition(0.050, -0.015);
    popupText1->setCharHeight(0.025 * FONT_SZ_MULT);
    popupText1->setColour(ColourValue::ColourValue(1.0, 1.0, 1.0));
    popupText1->setFontName("MainBig");
    
    popupText2->setMetricsMode(GMM_RELATIVE);
    popupText2->setAlignment(TextAreaOverlayElement::Left);
    popupText2->setPosition(0.06, 0.20);
    popupText2->setCharHeight(0.025 * FONT_SZ_MULT);
    popupText2->setColour(ColourValue::ColourValue(1.0, 1.0, 0.0));
    popupText2->setFontName("MainSmall");
    
    popupSlideNoText->setMetricsMode(GMM_RELATIVE);
    popupSlideNoText->setAlignment(TextAreaOverlayElement::Left);
    popupSlideNoText->setPosition(0.050, 0.405);
    popupSlideNoText->setCharHeight(0.025 * FONT_SZ_MULT);
    popupSlideNoText->setColour(ColourValue::ColourValue(1.0, 1.0, 0.0));
    popupSlideNoText->setFontName("MainSmall");
}

void TutorialManager::setAdditionalText(std::string text)
{
    additionalText = text;
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
    if (popupSlideNoText)
        OgreFramework::getSingletonPtr()->m_pOverlayMgr->destroyOverlayElement(popupSlideNoText);
    popupSubWindowBackground = NULL;
    popupWindowBackground = NULL;
    popupGoLeftBackground = NULL;
    popupGoRightBackground = NULL;
    popupText1 = NULL;
    popupText2 = NULL;
    popupSlideNoText = NULL;
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
    popupSlideNoText = static_cast<TextAreaOverlayElement*>(OgreFramework::getSingletonPtr()->m_pOverlayMgr->createOverlayElement("TextArea", "TutorialSlideNoText"));
    popupOverlay->add2D(popupSubWindowBackground);
    popupOverlay->add2D(popupWindowBackground);
    //popupWindowBackground->addChild(popupSubWindowBackground);
    popupSubWindowBackground->addChild(popupGoLeftBackground);
    popupSubWindowBackground->addChild(popupGoRightBackground);
    popupSubWindowBackground->addChild(popupExitBackground);
    popupSubWindowBackground->addChild(popupText1);
    popupSubWindowBackground->addChild(popupText2);
    popupSubWindowBackground->addChild(popupSlideNoText);
    
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
    return in;
}
