#pragma once

#import "PolycodeView.h"
#include "Polycode3DPhysics.h"
#include "Polycode.h"

using namespace Polycode;

#include "Util.h"
#include "Player.h"
#include "Tunnel.h"

using namespace std;

class Vinezors : public EventHandler {
public:
    Vinezors(PolycodeView *view);
    ~Vinezors();
    
	void handleEvent(Event *e);
    bool Update();
    
private:
    Core *core;

    bool pause;
    
	Screen *screen;

    ScreenShape *barHP;
	ScreenLabel *label1;
    ScreenLabel *label2;
    ScreenLabel *label3;

	CollisionScene *scene;
	
	Vector3 origin;

	Player *player;

	Tunnel *tunnel;
	
    /*
	ScenePrimitive *fog1;
	ScenePrimitive *fog2;
	ScenePrimitive *fog3;
    */
    
    SceneEntity * speaker;
    vector<SceneSound *> podSounds;
    
    SceneSound* negativeFeedback;
    SceneSound* positiveFeedback;
    
    void playPodSound(Pod *);
};
