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
    std::string message;
    std::string background;
    
    TutorialSlide() : message(""), background("") {}
    TutorialSlide(std::string msg, std::string bg) : message(msg), background(bg) {}
};

class TutorialManager
{
private:
    Overlay* popupOverlay;
    PanelOverlayElement* popupWindowBackground;
    PanelOverlayElement* popupSubWindowBackground;
    
    PanelOverlayElement* popupGoLeftBackground;
    PanelOverlayElement* popupGoRightBackground;
    
    TextAreaOverlayElement* popupText;
    TextAreaOverlayElement* popupSlideNoText;
    
    std::vector<TutorialSlide> slides;
    std::vector<bool> visitedSlide;
    bool enableSlides;
    int slideNo;
    
    
    // Similar to Hud Base Class
    std::vector<HudButton> buttons;
    enum ButtonSet {
        BUTTON_GOLEFT, BUTTON_GORIGHT
    };
    
    float yoffset;
public:
    TutorialManager();
    ~TutorialManager();
    
    const int NUM_TUTORIAL_SLIDES = 10;
    enum TutorialSlidesType {
        TUTORIAL_SLIDES_WELCOME,
        TUTORIAL_SLIDES_ZERO_BACK,
        TUTORIAL_SLIDES_ONE_BACK,
        TUTORIAL_SLIDES_TWO_BACK,
        TUTORIAL_SLIDES_COLOR_SOUND,
        TUTORIAL_SLIDES_SHAPE_SOUND,
        TUTORIAL_SLIDES_SOUND_ONLY,
        TUTORIAL_SLIDES_HOLDOUT,
        TUTORIAL_SLIDES_OBSTACLE,
        TUTORIAL_SLIDES_TIME_WARP
    };
    
    bool isEnabled() const;
    void enable();
    void disable();
    
    std::vector<TutorialSlide> getSlides(TutorialSlidesType type) const;
    void setSlides(const std::vector<TutorialSlide> & slides);
    void setSlides(TutorialSlidesType type);
    bool hasPreviousSlide() const;
    bool hasNextSlide() const;
    void setPreviousSlide();
    void setNextSlide();
    void updateOverlay();
    void update(float elapsed);
    
    // Show or hide popup tutorial slides. If tutorial slides are active,
    // in-game controls and update will be inactive
    void hide();
    void show();
    bool isHidden() const;
    bool isVisible() const;
    std::string queryButtons(Vector2 target) const;
    void processInput(Vector2 target);
    void adjust();
    
    void dealloc();
    void alloc();
    
    friend std::ostream& operator<<(std::ostream& out, const TutorialManager& tutorialMgr);
    friend std::istream& operator>>(std::istream& in, TutorialManager& tutorialMgr);
};

std::ostream& operator<<(std::ostream& out, const TutorialManager& tutorialMgr);
std::istream& operator>>(std::istream& in, TutorialManager& tutorialMgr);


#endif /* defined(__Vinezors2_0__TutorialManager__) */
