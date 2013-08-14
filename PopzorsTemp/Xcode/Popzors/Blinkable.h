#pragma once

#include "Pch.h"

class Blinkable
{
protected:
    Color currentColor;
	Color baseColor;
	Color blinkColor;
	Number timeBlinkLength;
	Number timeBlinked;
    Number ontime;
    Number offtime;
    bool toggle;
    Number timeToggle;
    
public:
    
    Blinkable(Color baseColor = Color(0,0,0,0), Color blinkColor = Color(0,0,0,0), Number blinktime = 0);
    
    Blinkable(Color baseColor, Color blinkColor, Number blinktime, Number ontime, Number offtime);
    
    void activateBlink();
	virtual void setColor(int r, int g, int b, int a) = 0;
	virtual void setColor(Color color) = 0;
	void setTimeBlinkLength(Number blinktime);
	void setTimeBlinkLength(Number blinktime, Number ontime, Number offtime);
	void setBaseColor(int r, int g, int b, int a);
	void setBaseColor(Color color);
	void setBlinkColor(int r, int g, int b, int a);
	void setBlinkColor(Color color);
    void updateBlinkTime(Number elapsed);
    
    bool isBlinking() const { return timeBlinked > 0; }
    bool isToggling() const { return timeToggle > 0; }
	Color getBaseColor() const { return baseColor; }
	Color getBlinkColor() const { return blinkColor; }
    
    virtual void reset();
    virtual void update(Number elapsed);
    
    ~Blinkable() {}
};
