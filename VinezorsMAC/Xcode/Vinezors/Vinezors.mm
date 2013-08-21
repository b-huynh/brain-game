#include "Vinezors.h"

Vinezors::Vinezors(PolycodeView *view)
{
    srand(time(0));
    
	core = new POLYCODE_CORE(view, 640, 480, true, false, 0, 0, 60);
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
	
	fog1 = new ScenePrimitive(ScenePrimitive::TYPE_BOX, 100 * TUNNEL_WIDTH, 100 * TUNNEL_WIDTH, 10 * TUNNEL_DEPTH);
	fog1->setPosition(origin.x, origin.y, origin.z - FOG_OFFSET * TUNNEL_DEPTH);
	fog1->setColor(0.5, 0.5, 0.5, 1.0);
    /*
	fog2 = new ScenePrimitive(ScenePrimitive::TYPE_BOX, 100 * TUNNEL_WIDTH, 100 * TUNNEL_WIDTH, 17.5 * TUNNEL_DEPTH);
	fog2->setPosition(origin.x, origin.y, origin.z - 25 * TUNNEL_DEPTH);
	fog2->setColor(0.8, 0.8, 0.8, 0.8);
	fog3 = new ScenePrimitive(ScenePrimitive::TYPE_BOX, 100 * TUNNEL_WIDTH, 100 * TUNNEL_WIDTH, 20.0 * TUNNEL_DEPTH);
	fog3->setPosition(origin.x, origin.y, origin.z - 25 * TUNNEL_DEPTH);
	fog3->setColor(1.0, 1.0, 1.0, 0.5);
     */
	scene->addChild(fog1);
	//scene->addChild(fog2);
	//scene->addChild(fog3);
    
    barHP = new ScreenShape(ScreenShape::SHAPE_RECT, BAR_WIDTH, BAR_HEIGHT);
    barHP->setPosition((Vector2(BAR_XPOS, BAR_YPOS) + Vector2(BAR_XPOS + BAR_WIDTH, BAR_YPOS + BAR_HEIGHT)) / 2);
    barHP->setColor(0.0, 1.0, 0.0, 1.0);
    screen->addChild(barHP);
    
    label = new ScreenLabel("", 36);
    label->setColor(1.0, 1.0, 0.0, 1.0);
    screen->addChild(label);
    
    label1 = new ScreenLabel("Time: " + toStringDouble(player->getTotalElapsed()), 36);
    label1->setPosition(0,425);
    screen->addChild(label1);
    
	label2 = new ScreenLabel("Score: " + toStringInt(player->getScore()), 36);
    label2->setPosition(0,375);
	screen->addChild(label2);
    
    label3 = new ScreenLabel("N-Back: " + toStringInt(tunnel->getNBack()), 36);
    label3->setPosition(425,425);
    screen->addChild(label3);
    
    //Sounds
    speaker = new SceneEntity();
    
    string soundFiles [] = {"blip.wav", "gulp.wav", "pluck.wav", "test.wav", "womp.wav"};
    
    for (int i = 0; i < sizeof(soundFiles) / sizeof(soundFiles[0]); ++i) {
        SceneSound * sound = new SceneSound("Resources/" + soundFiles[i], 20, 50);
        speaker->addChild(sound);
        podSounds.push_back(sound);
    }
    negativeFeedback = new SceneSound("Resources/chimedown.wav", 20, 50);
    speaker->addChild(negativeFeedback);
    positiveFeedback = new SceneSound("Resources/chimeup.wav", 20, 50);
    speaker->addChild(positiveFeedback);
    
    SceneSoundListener * listener = new SceneSoundListener();
    scene->addEntity(listener);
    
	core->getInput()->addEventListener(this, InputEvent::EVENT_KEYDOWN);
	core->getInput()->addEventListener(this, InputEvent::EVENT_KEYUP);
	//core->getInput()->addEventListener(this, InputEvent::EVENT_MOUSEMOVE);
	//core->getInput()->addEventListener(this, InputEvent::EVENT_MOUSEDOWN);
	//core->getInput()->addEventListener(this, InputEvent::EVENT_MOUSEUP);
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
                    curRot = player->getCombinedRotAndRoll();
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
                    case KEY_s:
                        pause = !pause;
                        if (!pause) {
                            player->setCamPos(player->getOldPos());
                            player->setCamRot(player->getOldRot());
                            player->setCamRoll(player->getOldRoll());
                            fog1->enabled = true;
                        } else {
                            player->saveProgress("vinezors.save");
                            label->setText("Saved");
                            player->setOldPos(player->getCamPos());
                            player->setOldRot(player->getCamRot());
                            player->setOldRoll(player->getCamRoll());
                            fog1->enabled = false;
                        }   
                        break;
                    case KEY_ESCAPE:
                        exit(0);
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
                    case KEY_s:
                        label->setText("");
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
    if (!tunnel->isDone() &&
        (player->getHP() >= HP_POSITIVE_LIMIT ||
         player->getHP() <= HP_NEGATIVE_LIMIT)) {
        tunnel->setDone(true);
        
        for (int i = 0; i < INITIATION_SECTIONS; ++i) {
            SectionInfo info = SectionInfo(NORMAL_BLANK, NO_DIRECTION, 0);
            if (i == INITIATION_SECTIONS - 1) {
                info.tunnelType = CHECKPOINT;
            }
            tunnel->renewSection(info);
        }
    }
    
    if (!pause) {
        int currentHp = player->getHP();
        player->update(elapsed, tunnel);
        player->checkCollisions(tunnel);
        //Play Feedback Sound
        if (currentHp < player->getHP()) {
            positiveFeedback->getSound()->Play();
        }
        else if (currentHp > player->getHP()) {
            negativeFeedback->getSound()->Play();
        }
        
        // Animate Pod Growing outwards or Growing inwards
        const double GROWTH_SPEED = 5;
        TunnelSlice* nextSliceM = tunnel->getNext(POD_APPEARANCE);
        if (nextSliceM) {
            if (!tunnel->isDone())
                nextSliceM->updateGrowth(GROWTH_SPEED * elapsed);
            else
                nextSliceM->updateGrowth(-GROWTH_SPEED * elapsed);
        }
        nextSliceM = tunnel->getNext(POD_APPEARANCE + 1);
        if (nextSliceM) {
            if (!tunnel->isDone())
                nextSliceM->updateGrowth(GROWTH_SPEED * elapsed);
            else
                nextSliceM->updateGrowth(-GROWTH_SPEED * elapsed);
        }
        
        // If player's current position is out of its current segment...
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
                // Generate a new tunnel because we are at the end
                Vector3 newOrigin = tunnel->getEnd();
                TunnelSlice* current = tunnel->getCurrent();
                Quaternion rot = current->getQuaternion();
                Vector3 forward = current->getForward();
                int nback = tunnel->getNBack();
                delete tunnel;
                
                int newNback = nback;
                if (player->getHP() > 0)
                    newNback++;
                else if (player->getHP() < 0)
                    newNback--;
                if (newNback < 0)
                    newNback = 0;
                
                tunnel = new Tunnel(scene, newOrigin + forward * (TUNNEL_WIDTH / 2), TUNNEL_WIDTH, TUNNEL_DEPTH, TUNNEL_SEGMENTS_PER_SECTION, TUNNEL_SEGMENTS_PER_POD);
                tunnel->constructTunnel(TUNNEL_SECTIONS, newNback, rot);
                player->setHP(STARTING_HP);
            }
            
            // Show Pod Color and Play Sound
            TunnelSlice* nextSliceN = tunnel->getNext(POD_APPEARANCE);
            if (nextSliceN && !tunnel->isDone())
            {
                for (int i = 0; i < nextSliceN->getPods().size(); ++i) {
                    nextSliceN->getPods()[i]->showPod();
                    nextSliceN->getPods()[i]->showShell();
                    playPodSound(nextSliceN->getPods()[i]);
                }
            }
        }
    } else {
        /*
        // Navigation Debug Keys
        if (player->getKeyUp())
            player->move(Vector3(player->getCamForward() * CAM_SPEED * elapsed));
        if (player->getKeyDown())
            player->move(Vector3(player->getCamForward() * -CAM_SPEED * elapsed));
        if (player->getKeyLeft())
        	player->move(Vector3(player->getCamRight() * -CAM_SPEED * elapsed));
        if (player->getKeyRight())
        	player->move(Vector3(player->getCamRight() * CAM_SPEED * elapsed));
         */
    }
    
    Quaternion camRot = player->getCombinedRotAndRoll();
	scene->getDefaultCamera()->setPosition(player->getCamPos());
	scene->getDefaultCamera()->setRotationQuat(camRot.w, camRot.x, camRot.y, camRot.z);
    
    fog1->setPosition(player->getCamPos() + player->getCamForward() * (FOG_OFFSET * TUNNEL_DEPTH));
    fog1->setRotationQuat(camRot.w, camRot.x, camRot.y, camRot.z);
    
    label1->setText("Time: " + toStringDouble(player->getTotalElapsed()));
    label2->setText("Score: " + toStringInt(player->getScore()));
    label3->setText("N-Back: " + toStringInt(tunnel->getNBack()));
    
    if (player->getHP() >= 0) {
        barHP->setScale(player->getHP() / (double)(HP_POSITIVE_LIMIT), 1.0);
        
        barHP->setColor(0.0, 1.0, 0.0, 1.0);
        if (player->getHP() >= HP_POSITIVE_LIMIT)
            barHP->setColor(0.0, 0.0, 1.0, 1.0);
            
    } else {
        barHP->setScale(player->getHP() / (double)(-HP_NEGATIVE_LIMIT), 1.0);
        
        barHP->setColor(1.0, 1.0, 0.0, 1.0);
        if (player->getHP() <= HP_NEGATIVE_LIMIT)
            barHP->setColor(1.0, 0.0, 0.0, 1.0);
    }
     
	return core->updateAndRender();
}