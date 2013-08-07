#include "Vinezors.h"

Vinezors::Vinezors(PolycodeView *view)
{
	core = new POLYCODE_CORE(view, 640, 480, false, false, 0, 0, 60);
	CoreServices::getInstance()->getResourceManager()->addArchive("default.pak");
	CoreServices::getInstance()->getResourceManager()->addDirResource("default", false);
	CoreServices::getInstance()->getResourceManager()->addDirResource("resources", false);

    pause = false;
	
	screen = new Screen();  

	scene = new CollisionScene();
    scene->ambientColor = Color(0.5, 0.5, 0.5, 0.2);
    scene->enableLighting(true);
    
	origin = Vector3(0, 0, 0);

	scene->getDefaultCamera()->setPosition(Vector3(origin.x, origin.y, origin.z + TUNNEL_DEPTH / 2));
	scene->getDefaultCamera()->lookAt(origin);
	player = new Player(scene, "The Player", scene->getDefaultCamera()->getPosition(), scene->getDefaultCamera()->getRotationQuat(), CAM_SPEED, VINE_T_OFFSET);
	player->addVine(new Vine(scene, player->getCamPos(), VINE_RADIUS));

	tunnel = new Tunnel(scene, origin + TUNNEL_REFERENCE_FORWARD * (TUNNEL_WIDTH / 2), TUNNEL_WIDTH, TUNNEL_DEPTH, TUNNEL_SEGMENTS_PER_SECTION, TUNNEL_SEGMENTS_PER_POD);
    tunnel->constructTunnel(TUNNEL_SECTIONS, NBACK);
	
    /*
	fog1 = new ScenePrimitive(ScenePrimitive::TYPE_BOX, 100 * TUNNEL_WIDTH, 100 * TUNNEL_WIDTH, 2.5 * TUNNEL_DEPTH);
	fog1->setPosition(origin.x, origin.y, origin.z - 5 * TUNNEL_DEPTH);
	fog1->setColor(0.5, 0.5, 0.5, 1.0);
	fog2 = new ScenePrimitive(ScenePrimitive::TYPE_BOX, 100 * TUNNEL_WIDTH, 100 * TUNNEL_WIDTH, 4.0 * TUNNEL_DEPTH);
	fog2->setPosition(origin.x, origin.y, origin.z - 5 * TUNNEL_DEPTH);
	fog2->setColor(0.8, 0.8, 0.8, 0.8);
	fog3 = new ScenePrimitive(ScenePrimitive::TYPE_BOX, 100 * TUNNEL_WIDTH, 100 * TUNNEL_WIDTH, 5.0 * TUNNEL_DEPTH);
	fog3->setPosition(origin.x, origin.y, origin.z - 5 * TUNNEL_DEPTH);
	fog3->setColor(1.0, 1.0, 1.0, 0.5);
	scene->addChild(fog1);
	scene->addChild(fog2);
	scene->addChild(fog3);
     */

    barHP = new ScreenShape(ScreenShape::SHAPE_RECT, BAR_WIDTH, BAR_HEIGHT);
    barHP->setPosition((Vector2(BAR_XPOS, BAR_YPOS) + Vector2(BAR_XPOS + BAR_WIDTH, BAR_YPOS + BAR_HEIGHT)) / 2);
    barHP->setColor(0.0, 1.0, 0.0, 1.0);
    screen->addChild(barHP);
    
	label = new ScreenLabel("Score: " + toStringInt(player->getScore()), 36);
    label->setPosition(0,400);
	screen->addChild(label);
    
    label2 = new ScreenLabel("Nback: " + toStringInt(tunnel->getNBack()), 36);
    label2->setPosition(450,400);
    screen->addChild(label2);
    
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
                    
                    Vector3 right = player->getCamRight();
                    Vector3 up = player->getCamUpward();
                    Quaternion yawRot;
                    Quaternion pitchRot;
                    yawRot.createFromAxisAngle(up.x, up.y, up.z, dmove.x);
                    pitchRot.createFromAxisAngle(right.x, right.y, right.z, dmove.y);
                    
                    Quaternion curRot = player->getCamRot();
                    curRot = pitchRot * yawRot * curRot;
                    player->setCamRot(curRot);
                    scene->getDefaultCamera()->setRotationQuat(curRot.w, curRot.x, curRot.y, curRot.z);
                    
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
                        if (!pause) {
                            player->setDir(rightOf(player->getDir()));
                            player->setDesireRoll(player->getDesireRoll() + 45);
                        }
						break;
					case KEY_RIGHT:
						player->setKeyRight(true);
                        if (!pause) {
                            player->setDir(leftOf(player->getDir()));
                            player->setDesireRoll(player->getDesireRoll() - 45);
                        }
						break;
                    case KEY_p:
                        pause = !pause;
                        if (!pause) {
                            player->setCamPos(player->getOldPos());
                            player->setCamRot(player->getOldRot());
                            player->setCamRoll(player->getOldRoll());
                            player->setHP(STARTING_HP * 2 / 3);
                        } else {
                            player->setOldPos(player->getCamPos());
                            player->setOldRot(player->getCamRot());
                            player->setOldRoll(player->getCamRoll());
                        }   
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
    if (pod->getType() != POD_NONE)
        podSounds[(int)pod->getPodType()]->getSound()->Play();
}

bool Vinezors::Update()     
{
	Number elapsed = core->getElapsed();
	
    // Determine whether a stage has completed
    if (!tunnel->isDone() && player->getScore() >= 200) {
        tunnel->setDone(true);
        
        for (int i = 0; i < INITIATION_SECTIONS; ++i) {
            SectionInfo info = SectionInfo(NORMAL_BLANK, NO_DIRECTION, 0);
            if (i == INITIATION_SECTIONS - 1) {
                info.tunnelType = CHECKPOINT;
            }
            tunnel->renewSection(info);
        }
    }
    
    // Gameover
    if (player->getHP() <= 0) {
        pause = true;
        player->setOldPos(player->getCamPos());
        player->setOldRot(player->getCamRot());
        player->setOldRoll(player->getCamRoll());
    }
    
    if (!pause) {
        player->update(elapsed, tunnel);
        /*
        Quaternion rot = player->getCamRot();
        fog1->setPosition(player->getCamPos() + player->getCamForward() * (5 * TUNNEL_DEPTH));
        fog2->setPosition(player->getCamPos() + player->getCamForward() * (5 * TUNNEL_DEPTH));
        fog3->setPosition(player->getCamPos() + player->getCamForward() * (5 * TUNNEL_DEPTH));
        fog1->setRotationQuat(rot.w, rot.x, rot.y, rot.z);
        fog2->setRotationQuat(rot.w, rot.x, rot.y, rot.z);
        fog3->setRotationQuat(rot.w, rot.x, rot.y, rot.z);
        */
        
        // Animate Pod Appearance
        TunnelSlice* nextSliceM = tunnel->getNext(POD_APPEARANCE);
        if (nextSliceM)
            nextSliceM->updateGrowth(5 * elapsed);
        nextSliceM = tunnel->getNext(POD_APPEARANCE + 1);
        if (nextSliceM)
            nextSliceM->updateGrowth(5 * elapsed);
        
        if (tunnel->renewIfNecessary(player->getCamPos())) {
        
            // Set new camera slerp goal
            player->setOldPos(player->getCamPos());
            player->setOldRot(player->getCamRot());
            player->setOldRoll(player->getCamRoll());
            TunnelSlice* nextSlice1 = tunnel->getNext(1);
            if (nextSlice1)
            {
                player->setDesireRot(nextSlice1->getQuaternion());
            }
            else {
                Vector3 newOrigin = tunnel->getEnd();
                TunnelSlice* current = tunnel->getCurrent();
                Quaternion rot = current->getQuaternion();
                Vector3 forward = current->getForward();
                int nback = tunnel->getNBack();
                delete tunnel;
                tunnel = new Tunnel(scene, newOrigin + forward * (TUNNEL_WIDTH / 2), TUNNEL_WIDTH, TUNNEL_DEPTH, TUNNEL_SEGMENTS_PER_SECTION, TUNNEL_SEGMENTS_PER_POD);
                tunnel->constructTunnel(TUNNEL_SECTIONS, nback + 1, rot);
                player->setScore(0.0);
            }
            
            // Show Pod Color and Play Sound
            TunnelSlice* nextSliceN = tunnel->getNext(POD_APPEARANCE);
            if (nextSliceN)
            {
                for (int i = 0; i < nextSliceN->getPods().size(); ++i) {
                    nextSliceN->getPods()[i]->showPod();
                    nextSliceN->getPods()[i]->showShell();
                    playPodSound(nextSliceN->getPods()[i]);
                }
            }
        }

        player->checkCollisions(tunnel);    
        /*
         //Play Gold Sound
         int currentScore = player->getScore();
         player->checkCollisions(tunnel);
         if (currentScore < player->getScore()) podSounds[(int)POD_YELLOW]->getSound()->Play();
         
         //Play pod sounds as they come out of the fog
         vector<Pod *> pods = tunnel->findPodCollisions(scene, fog3);
         for (int i = 0; i < pods.size(); ++i)
            playPodSound(pods[i]);
        */
    } else {
        // Navigation Keys
        if (player->getKeyUp())
            player->move(Vector3(player->getCamForward() * CAM_SPEED * elapsed));
        if (player->getKeyDown())
            player->move(Vector3(player->getCamForward() * -CAM_SPEED * elapsed));
        if (player->getKeyLeft())
        	player->move(Vector3(player->getCamRight() * -CAM_SPEED * elapsed));
        if (player->getKeyRight())
        	player->move(Vector3(player->getCamRight() * CAM_SPEED * elapsed));
    }
    if (player->getHP() < 0)
        player->setHP(0.0);
    if (player->getHP() > STARTING_HP)
        player->setHP(STARTING_HP);
    
    Quaternion camRot = player->getCombinedRotAndRoll();
	scene->getDefaultCamera()->setPosition(player->getCamPos());
	scene->getDefaultCamera()->setRotationQuat(camRot.w, camRot.x, camRot.y, camRot.z);

    label->setText("Score: " + toStringInt(player->getScore()));
    label2->setText("Nback: " + toStringInt(tunnel->getNBack()));
    
    barHP->setScale(player->getHP() / STARTING_HP, 1.0);
    if (player->getHP() <= STARTING_HP / 5)
        barHP->setColor(1.0, 0.0, 0.0, 1.0);
    else if (player->getHP() <= STARTING_HP / 2)
        barHP->setColor(1.0, 1.0, 0.0, 1.0);
    else
        barHP->setColor(0.0, 1.0, 0.0, 1.0);
    
	return core->updateAndRender();
}