#include "Vinezors.h"

Vinezors::Vinezors(PolycodeView *view)
{
	core = new Win32Core(view, 640, 480, false, false, 0, 0, 60);	  
	CoreServices::getInstance()->getResourceManager()->addArchive("default.pak");
	CoreServices::getInstance()->getResourceManager()->addDirResource("default", false);
	CoreServices::getInstance()->getResourceManager()->addDirResource("resources", false);
	
	screen = new Screen();
	label = new ScreenLabel("Hello!", 14);
	screen->addEntity(label);

	scene = new CollisionScene();
	origin = Vector3(0, 0, 0);

	player = new Player("The Player", Vector3(origin.x, origin.y, origin.z + TUNNEL_DEPTH / 2));
	player->addVine(new Vine(scene, player->getCamPos() + Vector3(0, 0, -TUNNEL_WIDTH), VINE_LENGTH, VINE_RADIUS));

	tunnel = new Tunnel(scene, origin, TUNNEL_WIDTH, TUNNEL_DEPTH);
	for (int i = 0; i < 50; ++i)	
		tunnel->addSegment();

	scene->getDefaultCamera()->setPosition(player->getCamPos());
	scene->getDefaultCamera()->lookAt(origin);

	core->getInput()->addEventListener(this, InputEvent::EVENT_KEYDOWN);
	core->getInput()->addEventListener(this, InputEvent::EVENT_KEYUP);
	core->getInput()->addEventListener(this, InputEvent::EVENT_MOUSEMOVE);
	core->getInput()->addEventListener(this, InputEvent::EVENT_MOUSEDOWN);
	core->getInput()->addEventListener(this, InputEvent::EVENT_MOUSEUP);
}

Vinezors::~Vinezors() 
{
	delete player; player = NULL;
	delete tunnel; tunnel = NULL;
}

void Vinezors::handleEvent(Event *e)
{
	if(e->getDispatcher() == core->getInput()) 
	{
		InputEvent *inputEvent = (InputEvent*)e;
		
		switch(e->getEventCode()) 
		{
			case InputEvent::EVENT_MOUSEMOVE:
				if (player->getMouseLeft())
				{
					Vector2 dmove = inputEvent->getMousePosition() - player->getMousePos();	
					scene->getDefaultCamera()->setYaw(scene->getDefaultCamera()->getYaw() - dmove.x);
					scene->getDefaultCamera()->setPitch(scene->getDefaultCamera()->getPitch() - dmove.y);

					player->setMousePos(inputEvent->getMousePosition());
				}
				break;
			case InputEvent::EVENT_MOUSEDOWN:
				if (inputEvent->getMouseButton() == 0)
				{
					player->setMouseLeft(true);
					player->setMousePos(inputEvent->getMousePosition());
				}
				break;			
			case InputEvent::EVENT_MOUSEUP:
				if (inputEvent->getMouseButton() == 0)
				{
					player->setMouseLeft(false);
					player->setMousePos(inputEvent->getMousePosition());
				}
				break;		
			case InputEvent::EVENT_KEYDOWN:
				switch (inputEvent->keyCode()) 
				{
					case KEY_UP:
						player->setKeyUp(true);
						break;
					case KEY_DOWN:
						player->setKeyDown(true);			
						break;
					case KEY_LEFT:
						player->setKeyLeft(true);
						player->setDir(leftOf(player->getDir()));
						break;
					case KEY_RIGHT:
						player->setKeyRight(true);			
						player->setDir(rightOf(player->getDir()));
						break;		
				}
				break;
			case InputEvent::EVENT_KEYUP:
				switch (inputEvent->key) 
				{
					case KEY_UP:
						player->setKeyUp(false);
						break;
					case KEY_DOWN:
						player->setKeyDown(false);			
						break;					
					case KEY_LEFT:
						player->setKeyLeft(false);
						break;
					case KEY_RIGHT:
						player->setKeyRight(false);			
						break;				
				}
				break;			
		}
		
	}
}

bool Vinezors::Update() 
{
	Number elapsed = core->getElapsed();

	player->update(elapsed);

	scene->getDefaultCamera()->setPosition(player->getCamPos());

	return core->Update();
}