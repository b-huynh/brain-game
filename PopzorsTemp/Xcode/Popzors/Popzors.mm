#include "Popzors.h"

#include <iostream>
#include <cstdlib>
#include <time.h>

int fibonacci(int k)
{
	if (k == 0) return 0;
	if (k <= 2) return 1;
	return fibonacci(k-1) + fibonacci(k-2);
}

Popzors::Popzors(PolycodeView *view)
	:EventHandler()
{
	srand(time(0));
    
	int width = 640;
	int height = 480;

	core = new CocoaCore(view, width, height, false, false, 0, 0,60);
	CoreServices::getInstance()->getResourceManager()->addArchive("default.pak");
	CoreServices::getInstance()->getResourceManager()->addDirResource("default", false);
	
	//SCREEN
	screen = new Screen();

	ScreenShape * x1 = new ScreenShape(ScreenShape::SHAPE_RECT, 1000, 100);
	ScreenShape * x2 = new ScreenShape(ScreenShape::SHAPE_RECT, 1000, 100);
	x1->setColor(1.0, 0.0, 0.0, 0.25);
	x2->setColor(1.0, 0.0, 0.0, 0.25);
	x1->setPosition(width/2, height/2);
	x2->setPosition(width/2, height/2);
	x1->setRotation(45);
	x2->setRotation(315);

	ScreenShape * circle = new ScreenShape(ScreenShape::SHAPE_CIRCLE, height, height);
	circle->setColor(0.0, 1.0, 0.0, 0.25);
	circle->setPosition(width/2, height/2);

	//SCENE
	scene = new CollisionScene();
	cameraPos = Vector3(0,7,7);
	origin = Vector3(0,0,0);

	//set state
	timeElapsed = 0.0;

	resultsLength = 1;
    
	selected = NULL;
	
	//Make Ground
    ground = Ground(Vector3(0, -0.5, 0), GROUND_COLOR, GROUND_COLOR, SIGNAL_LENGTH);
	ground.addToCollisionScene(scene);

	//Make some pots
	Pot pot1(Vector3(2.5, 0.0, 2.5), Cpot1, Cpot1, 1, Spot1);
    Pot pot2(Vector3(2.5, 0.0, -2.5), Cpot2, Cpot2, 1, Spot2);
    Pot pot3(Vector3(-2.5, 0.0, 2.5), Cpot3, Cpot3, 1, Spot3);
    Pot pot4(Vector3(-2.5, 0.0, -2.5), Cpot4, Cpot4, 1, Spot4);
    
	pot1.addToCollisionScene(scene);
	pot2.addToCollisionScene(scene);
	pot3.addToCollisionScene(scene);
	pot4.addToCollisionScene(scene);
	
	pots.push_back(pot1);
	pots.push_back(pot2);
	pots.push_back(pot3);
	pots.push_back(pot4);
    
    for (int i = 0; i < pots.size(); ++i)
        pots[i].setId(i);
	
    pattern = PoppyPattern();
    pattern.setPoppyPattern(scene, ground, pots, poppies);
    
	scene->getDefaultCamera()->setPosition(cameraPos);
	scene->getDefaultCamera()->lookAt(origin);

	core->getInput()->addEventListener(this, InputEvent::EVENT_MOUSEMOVE);
	core->getInput()->addEventListener(this, InputEvent::EVENT_MOUSEDOWN);
	core->getInput()->addEventListener(this, InputEvent::EVENT_MOUSEUP);
}

void Popzors::handlePoppyCollisions(Number elapsed)
{
    for (int i = 0; i < poppies.size(); ++i)
        for (int j = i + 1; j < poppies.size(); ++j)
        {
            poppies[i].handleCollision(elapsed, scene, poppies[j]);
        }
}

void Popzors::handleEvent(Event * e)
{
	if (e->getDispatcher() == core->getInput()) {
		InputEvent * inputEvent = (InputEvent *)e;
		switch(e->getEventCode()) {
			case InputEvent::EVENT_MOUSEDOWN:
			{
				//calculate ray start and end
				Vector2 mousePos = inputEvent->mousePosition;
				Vector3 dir = CoreServices::getInstance()->getRenderer()->projectRayFrom2DCoordinate(mousePos.x, mousePos.y);
				ClickedResult res = getClicked(scene->getDefaultCamera()->getPosition(), dir * 1000);
				
				if (res.poppy) {
					//deselect
					if ( (selected != res.poppy) && (selected != NULL) )
						Poppy * old = (Poppy*)selected;
					res.poppy->setColor(SELECT_COLOR);
					selected = res.poppy;
				}

				if (res.pot) {
                    res.pot->playSound();
					if (selected && selected != res.pot && pattern.isReady())
						if (selected->getType() == Selectable::TYPE_POPPY) {
							Poppy * old = (Poppy*)selected;
							Vector3 potpos = res.pot->getPosition();
							old->setPosition(potpos.x + randRangeDouble(-0.5, 0.5), potpos.y,potpos.z + randRangeDouble(-0.5, 0.5));
                            old->setSelectable(false);
                            old->setPotIdRef(res.pot->getId());
                            selected = NULL;
                            
                            // If pattern is done, reset and set a new pattern.
                            pattern.updatePlayerChoice(ground, *old, *res.pot);
						}
				}
			}
			break;
									
			case InputEvent::EVENT_MOUSEUP:
			break;
		}
	}
}

ClickedResult Popzors::getClicked(Vector3 origin, Vector3 dest)
{
	ClickedResult res;
	res.poppy = NULL;
	RayTestResult ray = scene->getFirstEntityInRay(origin, dest);
	for (int i = 0; i < poppies.size(); ++i) {
		if (poppies[i].hasEntity(ray.entity)) {
            if (poppies[i].isSelectable()) // If the poppy is not in a pot
                res.poppy = &poppies[i];
            else
                res.pot = &pots[poppies[i].getPotIdRef()]; // Otherwise, select the pot
		}
	}

	for (int i = 0; i < pots.size(); ++i)
		if (pots[i].hasEntity(ray.entity))
			res.pot = &pots[i];

	return res;
}

Popzors::~Popzors() 
{   
}

bool Popzors::Update() 
{
	Number elapsed = core->getElapsed();
    
    ground.update(elapsed);
    for (int i = 0; i < pots.size(); ++i)
        pots[i].update(elapsed);
    for (int i = 0; i < poppies.size(); ++i)
        poppies[i].update(elapsed);
    
    handlePoppyCollisions(elapsed);
    
	pattern.update(elapsed, ground, pots, poppies);
    if (pattern.isPlayerDone() && !ground.isBlinking())
        pattern.setPoppyPattern(scene, ground, pots, poppies);

	return core->updateAndRender();
}