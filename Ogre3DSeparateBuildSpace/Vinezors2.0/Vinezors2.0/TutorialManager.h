//
//  TutorialManager.h
//  Vinezors2.0
//
//  Created by Calvin Phung on 5/15/14.
//
//

#ifndef __Vinezors2_0__TutorialManager__
#define __Vinezors2_0__TutorialManager__

#include "Util.h"
#include "HudElements.h"

#include <fstream>

struct TutorialSlide
{
    std::string message;    // Text for popup-window
    std::string background; // Material for tutorial popup-window
    std::string screen;     // Material for entire screen if one, otherwise ""
    
    TutorialSlide() : message(""), background("") {}
    TutorialSlide(std::string msg, std::string bg, std::string screen) : message(msg), background(bg), screen(screen) {}
};

class TutorialManager
{
private:
    Overlay* popupOverlay;
    PanelOverlayElement* popupWindowBackground;
    PanelOverlayElement* popupSubWindowBackground;
    
    PanelOverlayElement* popupGoLeftBackground;
    PanelOverlayElement* popupGoRightBackground;
    PanelOverlayElement* popupExitBackground;
    
    TextAreaOverlayElement* popupText;
    TextAreaOverlayElement* popupSlideNoText;
    
    std::vector<TutorialSlide> queue;
    std::vector<TutorialSlide> slides;
    std::vector<bool> visitedSlide;
    
    bool enableSlides;
    int slideNo;
    
    float startTimer;
    
    // Similar to Hud Base Class
    std::vector<HudButton> buttons;
    enum ButtonSet {
        BUTTON_GOLEFT, BUTTON_GORIGHT, BUTTON_EXIT
    };
    
    float yoffset;
public:
    TutorialManager();
    ~TutorialManager();
    
    const int NUM_TUTORIAL_SLIDES = 14;
    enum TutorialSlidesType {
        TUTORIAL_SLIDES_TEXTBOX_NAVIGATION,
        TUTORIAL_SLIDES_TEXTBOX_1BACK,
        TUTORIAL_SLIDES_TEXTBOX_2BACK,
        TUTORIAL_SLIDES_TEXTBOX_HOLDOUT,
        TUTORIAL_SLIDES_TEXTBOX_SEEING_MATCH,
        TUTORIAL_SLIDES_TEXTBOX_MISSING_MATCH,
        TUTORIAL_SLIDES_TEXTBOX_INCORRECT_MATCH,
        TUTORIAL_SLIDES_TEXTBOX_OBSTACLE,
        TUTORIAL_SLIDES_TEXTBOX_SOUND_ONLY,
        TUTORIAL_SLIDES_TEXTBOX_3_2A,
        TUTORIAL_SLIDES_TEXTBOX_3_2B,
        TUTORIAL_SLIDES_TEXTBOX_HOLDOUT1,
        TUTORIAL_SLIDES_TEXTBOX_HOLDOUT2,
    };
    
    bool isEnabled() const;
    void enable();
    void disable();
    
    std::vector<TutorialSlide> getSlides(TutorialSlidesType type) const;
    void prepareSlides(const std::vector<TutorialSlide> & slides, float startTimer = 0.0f);
    void prepareSlides(TutorialSlidesType type, float startTimer = 0.0f);
    void setSlides(const std::vector<TutorialSlide> & slides);
    void setSlides(TutorialSlidesType type);
    bool hasPreviousSlide() const;
    bool hasNextSlide() const;
    bool setPreviousSlide();
    void setNextSlide();
    void updateOverlay();
    void updateOverride(float elapsed);
    void update(float elapsed);
    bool hasVisitedSlide(TutorialSlidesType type) const;
    
    // Show or hide popup tutorial slides. If tutorial slides are active,
    // in-game controls and update will be inactive
    void hide();
    void show();
    bool isHidden() const;
    bool isVisible() const;
    std::string queryButtons(Vector2 target) const;
    bool processInput(Vector2 target);
    void adjust();
    
    void dealloc();
    void alloc();
    
    friend std::ostream& operator<<(std::ostream& out, const TutorialManager& tutorialMgr);
    friend std::istream& operator>>(std::istream& in, TutorialManager& tutorialMgr);
};

std::ostream& operator<<(std::ostream& out, const TutorialManager& tutorialMgr);
std::istream& operator>>(std::istream& in, TutorialManager& tutorialMgr);


#endif /* defined(__Vinezors2_0__TutorialManager__) */
