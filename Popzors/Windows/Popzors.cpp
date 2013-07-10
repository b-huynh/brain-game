#include "Popzors.h"

#include <iostream>
#include <cstdlib>
#include <time.h>

Color floatColor (int r, int g, int b, int a)
{
	return Color( (float)r/255.0, (float)g/255.0, (float)b/255.0, (float)a/255.0 );
}

int fibonacci(int k)
{
	if (k == 0) return 0;
	if (k <= 2) return 1;
	return fibonacci(k-1) + fibonacci(k-2);
}

Color getRandomPotColor()
{
	int choice = rand() % 4 + 1;

	if (choice == 1) return Cpot1;
	if (choice == 2) return Cpot2;
	if (choice == 3) return Cpot3;
	if (choice == 4) return Cpot4;
}

Popzors::Popzors(PolycodeView *view)
	:EventHandler()
{
	int width = 640;
	int height = 480;

	core = new Win32Core(view, width, height, false, false, 0, 0,60);	  
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

	signaled = false;
	signalStart = 3;
	signalLength = 1;

	resultsLength = 1;

	currentPoppyID = 0;
	selected = NULL;
	
	//Make Ground
	ScenePrimitive * ground = new ScenePrimitive(ScenePrimitive::TYPE_PLANE, 8, 8);
	ground->setColor(floatColor(255,231,186,128));
	ground->setPosition(0,-0.5,0);
	scene->addCollisionChild(ground, CollisionSceneEntity::SHAPE_PLANE);

	//Make some poppies
	for (int i = -1; i < 2; ++i) {
		Poppy dummy (Vector3(i,0,rand()%3-1), Color(0,255,255,255), getRandomPotColor(), signalLength);
		dummy.addToCollisionScene(scene);
		poppies.push_back(dummy);
	}

	//Make some pots
	Pot pot1, pot2, pot3, pot4;
	pot1.setPosition(2.5,0.5,2.5);
	pot2.setPosition(2.5,0.5,-2.5);
	pot3.setPosition(-2.5,0.5,2.5);
	pot4.setPosition(-2.5,0.5,-2.5);

	pot1.setColor(Cpot1);
	pot2.setColor(Cpot2);
	pot3.setColor(Cpot3);
	pot4.setColor(Cpot4);

	pot1.addToCollisionScene(scene);
	pot2.addToCollisionScene(scene);
	pot3.addToCollisionScene(scene);
	pot4.addToCollisionScene(scene);
	
	pots.push_back(pot1);
	pots.push_back(pot2);
	pots.push_back(pot3);
	pots.push_back(pot4);
	
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
				ClickedResult res = getClicked(scene->getDefaultCamera()->getPosition(), dir * 1000);
				
				if (res.poppy) {
					//deselect
					if ( (selected != res.poppy) && (selected != NULL) )
						Poppy * old = (Poppy*)selected;
					res.poppy->setColor(255,255,0,255);
					selected = res.poppy;
				}

				if (res.pot) {
					if (selected && selected != res.pot)
						if (selected->getType() == Selectable::TYPE_POPPY) {
							Poppy * old = (Poppy*)selected;
							Vector3 potpos = res.pot->getPosition();
							old->setPosition(potpos.x,potpos.y,potpos.z);
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
			res.poppy = &poppies[i];
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
	srand((NULL));

	Number elapsed = core->getElapsed();
	timeElapsed += elapsed;

	if (timeElapsed > signalStart && !signaled) signaled = true;
	/*
	if (signaled && currentPoppyID < poppies.size())
		if (!poppies[currentPoppyID].blink(elapsed)) {
			poppies[currentPoppyID].unblink();
			currentPoppyID++;
		}
		*/
	if (signaled && currentPoppyID < poppies.size()) {
		if (!poppies[currentPoppyID].blink(elapsed)) 
			currentPoppyID++;
	}
	else {
		for (int i = 0; i < poppies.size(); ++i)
			poppies[i].unblink();
	}
	

	return core->updateAndRender();
}