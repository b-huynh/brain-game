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
	: EventHandler() {
	srand(time(0));
    
	int width = 640;
	int height = 480;

	core = new CocoaCore(view, width, height, false, false, 0, 0,60);
	CoreServices::getInstance()->getResourceManager()->addArchive("default.pak");
	CoreServices::getInstance()->getResourceManager()->addDirResource("default", false);
        
	//SCREEN
	screen = new Screen();

	//SCENE
	scene = new CollisionScene();
	cameraPos = Vector3(0,7,7);
	origin = Vector3(0,0,0);
	
    pattern = new PotPattern(screen, scene);
    pattern->setPattern();
    
	scene->getDefaultCamera()->setPosition(cameraPos);
	scene->getDefaultCamera()->lookAt(origin);

	core->getInput()->addEventListener(this, InputEvent::EVENT_MOUSEMOVE);
	core->getInput()->addEventListener(this, InputEvent::EVENT_MOUSEDOWN);
	core->getInput()->addEventListener(this, InputEvent::EVENT_MOUSEUP);
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
				ClickedResult res = pattern->stage.getClicked(scene->getDefaultCamera()->getPosition(), dir * 1000);
				
                pattern->processSelect(res);
			}
			break;
									
			case InputEvent::EVENT_MOUSEUP:
			break;
		}
	}
}

Popzors::~Popzors() 
{
    delete pattern;
}

bool Popzors::Update() 
{
	Number elapsed = core->getElapsed();
    
	pattern->update(elapsed);

	return core->updateAndRender();
}