#include "Vinezors.h"

Vinezors::Vinezors(PolycodeView *view)
{
	core = new POLYCODE_CORE(view, 640, 480, false, false, 0, 0, 60);
	CoreServices::getInstance()->getResourceManager()->addArchive("default.pak");
	CoreServices::getInstance()->getResourceManager()->addDirResource("default", false);
	CoreServices::getInstance()->getResourceManager()->addDirResource("resources", false);
	
	screen = new Screen();

	scene = new CollisionScene();
	origin = Vector3(0, 0, 0);

	player = new Player(scene, "The Player", Vector3(origin.x, origin.y, origin.z + TUNNEL_DEPTH / 2), Vector3(0, 0, -TUNNEL_DEPTH));
	player->addVine(new Vine(scene, player->getCamPos() + player->getVineOffset(), VINE_LENGTH, VINE_RADIUS));

	tunnel = new Tunnel(scene, origin, TUNNEL_WIDTH, TUNNEL_DEPTH);
    tunnel->constructTunnel(200, 1);
	
	fog1 = new ScenePrimitive(ScenePrimitive::TYPE_BOX, TUNNEL_WIDTH, TUNNEL_WIDTH, 2.5 * TUNNEL_DEPTH);
	fog1->setPosition(origin.x, origin.y, origin.z - 5 * TUNNEL_DEPTH);
	fog1->setColor(0.5, 0.5, 0.5, 1.0);
	fog2 = new ScenePrimitive(ScenePrimitive::TYPE_BOX, TUNNEL_WIDTH, TUNNEL_WIDTH, 4.0 * TUNNEL_DEPTH);
	fog2->setPosition(origin.x, origin.y, origin.z - 5 * TUNNEL_DEPTH);
	fog2->setColor(0.8, 0.8, 0.8, 0.8);
	fog3 = new ScenePrimitive(ScenePrimitive::TYPE_BOX, TUNNEL_WIDTH, TUNNEL_WIDTH, 5.0 * TUNNEL_DEPTH);
	fog3->setPosition(origin.x, origin.y, origin.z - 5 * TUNNEL_DEPTH);
	fog3->setColor(1.0, 1.0, 1.0, 0.5);
	scene->addChild(fog1);
	scene->addChild(fog2);
	scene->addChild(fog3);

	scene->getDefaultCamera()->setPosition(player->getCamPos());
	scene->getDefaultCamera()->lookAt(origin);

	label = new ScreenLabel(toStringInt(player->getScore()), 36);
	label->setColor(0.0, 0.0, 0.0, 1.0);
	screen->addChild(label);
    
//    label2 = new ScreenLabel("aghhh", 36);
//    label2->setPosition(50,50,0);
//    screen->addChild(label2);
    
    
    //Sounds
    speaker = new SceneEntity();
    
    string soundFiles [] = {"blip.wav", "gulp.wav", "pluck.wav", "womp.wav", "test.wav"};
    
    for (int i = 0; i < sizeof(soundFiles) / sizeof(soundFiles[0]); ++i) {
        SceneSound * sound = new SceneSound("Resources/" + soundFiles[i], 20, 50);
        speaker->addChild(sound);
        podSounds.push_back(sound);
    }
    
    SceneSoundListener * listener = new SceneSoundListener();
    scene->addEntity(listener);

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
	delete fog1; fog2 = NULL;
	delete fog2; fog2 = NULL;
	delete fog3; fog3 = NULL;
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

void Vinezors::playPodSound(Pod * pod)
{
    if (pod->getPodType() == POD_YELLOW) return;
    podSounds[(int)pod->getPodType()]->getSound()->Play();
}

bool Vinezors::Update() 
{
	Number elapsed = core->getElapsed();
	
	player->update(elapsed, tunnel);
	fog1->setPosition(player->getCamPos() + Vector3(0, 0, -5 * TUNNEL_DEPTH));
	fog2->setPosition(player->getCamPos() + Vector3(0, 0, -5 * TUNNEL_DEPTH));
	fog3->setPosition(player->getCamPos() + Vector3(0, 0, -5 * TUNNEL_DEPTH));
	tunnel->renewIfNecessary(player->getCamPos() + player->getVineOffset());

    int currentScore = player->getScore();
	player->checkCollisions(tunnel);
    if (currentScore < player->getScore()) podSounds[(int)POD_YELLOW]->getSound()->Play();
    vector<Pod *> pods = tunnel->findPodCollisions(scene, fog3);
    for (int i = 0; i < pods.size(); ++i)
        playPodSound(pods[i]);

    label->setText(toStringInt(player->getScore()));
	scene->getDefaultCamera()->setPosition(player->getCamPos());

	return core->updateAndRender();
}