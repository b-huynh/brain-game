#include "Popzors.h"

#include <iostream>
#include <cstdlib>
#include <time.h>

Popzors::Popzors(PolycodeView *view)
	: EventHandler() {
    seed = time(0);
	srand(seed);
    
	core = new CocoaCore(view, SCREEN_WIDTH, SCREEN_HEIGHT, true, false, 0, 0,60);
	CoreServices::getInstance()->getResourceManager()->addArchive("default.pak");
	CoreServices::getInstance()->getResourceManager()->addDirResource("default", false);
    CoreServices::getInstance()->getResourceManager()->addDirResource("resources", false);
        
    SCREEN_WIDTH = core->getXRes();
    SCREEN_HEIGHT = core->getYRes();
    BAR_XPOS = 20;
    BAR_YPOS = 20;
    BAR_WIDTH = SCREEN_WIDTH - BAR_XPOS * 2;
    BAR_HEIGHT = SCREEN_HEIGHT / 20;
    LABEL1_POSX = 0;
    LABEL1_POSY = SCREEN_HEIGHT - 55;
    LABEL2_POSX = 0;
    LABEL2_POSY = SCREEN_HEIGHT - 105;
    LABEL3_POSX = SCREEN_WIDTH - 255;
    LABEL3_POSY = SCREEN_HEIGHT - 55;
    
    pause = true;
        
	//SCREEN
	screen = new Screen();
        
    label = new ScreenLabel("\tPaused", 36);
    label->setColor(1.0, 1.0, 0.0, 1.0);
    label->setPosition(0, BAR_HEIGHT + 20);
    screen->addChild(label);

	//SCENE
	scene = new CollisionScene();
	cameraPos = Vector3(0,7,7);
	origin = Vector3(0,0,0);
	
    pattern = new BaselinePattern(screen, scene, seed, core->getXRes(), core->getYRes());
    pattern->setPattern();
    
	scene->getDefaultCamera()->setPosition(cameraPos);
	scene->getDefaultCamera()->lookAt(origin);

	core->getInput()->addEventListener(this, InputEvent::EVENT_MOUSEMOVE);
	core->getInput()->addEventListener(this, InputEvent::EVENT_MOUSEDOWN);
	core->getInput()->addEventListener(this, InputEvent::EVENT_MOUSEUP);
    core->getInput()->addEventListener(this, InputEvent::EVENT_KEYDOWN);
    core->getInput()->addEventListener(this, InputEvent::EVENT_KEYUP);
}

void Popzors::handleEvent(Event * e)
{
	if (e->getDispatcher() == core->getInput()) {
		InputEvent * inputEvent = (InputEvent *)e;
		switch(e->getEventCode()) {
            case InputEvent::EVENT_KEYDOWN:
            {
                switch(inputEvent->keyCode()) {
                        /*
                    case KEY_s:
                        if (!pattern->save("popzors" + toStringInt(seed) + ".csv"))
                            label->setText("Failed to save");
                        else
                            label->setText("Saved");
                        break;
                    case KEY_l:
                        if (!pattern->load("popzors.save"))
                            label->setText("Load feature is not yet available");
                        else
                            label->setText("Loaded popzors.save");
                        break;
                    case KEY_1:
                        pattern->reset();
                        delete pattern;
                        pattern = new BaselinePattern(screen,scene);
                        pattern->setPattern();
                        break;
                    case KEY_2:
                        pattern->reset();
                        delete pattern;
                        pattern = new PoppyPattern(screen, scene);
                        pattern->setPattern();
                        break;
                    case KEY_3:
                        pattern->reset();
                        delete pattern;
                        pattern = new PotPattern(screen, scene);
                        pattern->setPattern();
                        break;
                    case KEY_4:
                        pattern->reset();
                        delete pattern;
                        pattern = new HerdPattern(screen, scene);
                        pattern->setPattern();
                        break;
                         */
                    case KEY_SPACE:
                        label->setText("");
                        pause = false;
                        break;
                    case KEY_ESCAPE:
                        pattern->save("popzors" + toStringInt(seed) + ".csv");
                        label->setText("Saved");
                        exit(0);
                    case KEY_q:
                        pattern->save("popzors" + toStringInt(seed) + ".csv");
                        label->setText("Saved");
                        exit(0);
                }
            }
            break;
                /*
            case InputEvent::EVENT_KEYUP:
            {
                switch(inputEvent->key) {
                    case Polycode::KEY_s:
                        label->setText("");
                        break;
                    case Polycode::KEY_l:
                        label->setText("");
                        break;
                }
            }
                */
			case InputEvent::EVENT_MOUSEDOWN:
			{
                if (!pause) {
                    ClickedResult res = pattern->stage.getClicked(inputEvent);
                    pattern->processSelect(res);
                }
			}
			break;
									
			case InputEvent::EVENT_MOUSEUP:
            {
                if (!pause) {
                    ClickedResult res = pattern->stage.getClicked(inputEvent);
                    pattern->processSelect(res);
                }
            }
			break;
                
            case InputEvent::EVENT_MOUSEMOVE:
            {
                if (!pause) {
                    ClickedResult res = pattern->stage.getClicked(inputEvent);
                    pattern->processSelect(res);
                }
            }
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
    
    if (!pause)
        pattern->update(elapsed);
    
	return core->updateAndRender();
}