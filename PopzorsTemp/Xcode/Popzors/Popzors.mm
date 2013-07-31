#include "Popzors.h"

#include <iostream>
#include <cstdlib>
#include <time.h>

Popzors::Popzors(PolycodeView *view)
	: EventHandler() {
	srand(time(0));
    
	int width = 640;
	int height = 480;

	core = new CocoaCore(view, width, height, false, false, 0, 0,60);
	CoreServices::getInstance()->getResourceManager()->addArchive("default.pak");
	CoreServices::getInstance()->getResourceManager()->addDirResource("default", false);
        CoreServices::getInstance()->getResourceManager()->addDirResource("resources", false);
        
	//SCREEN
	screen = new Screen();
        
    label = new ScreenLabel(">.<", 36);
    label->setColor(1.0, 1.0, 0.0, 1.0);
    screen->addChild(label);

	//SCENE
	scene = new CollisionScene();
	cameraPos = Vector3(0,7,7);
	origin = Vector3(0,0,0);
	
    //pattern = new PotPattern(screen, scene);
    //pattern = new PoppyPattern(screen, scene);
    pattern = new HerdPattern(screen, scene);
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
				ClickedResult res = pattern->stage.getClicked(inputEvent);
                pattern->processSelect(res);
			}
			break;
									
			case InputEvent::EVENT_MOUSEUP:
                ClickedResult res = pattern->stage.getClicked(inputEvent);
                pattern->processSelect(res);
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
    
    label->setText( pattern->isFinished() ? "True" : "False" );

	return core->updateAndRender();
}