#include "Popzors.h"

Popzors::Popzors(PolycodeView *view)
{
	core = new Win32Core(view, 640,480,false, false, 0, 0,60);	  
	CoreServices::getInstance()->getResourceManager()->addArchive("default.pak");
	CoreServices::getInstance()->getResourceManager()->addDirResource("default", false);
	
	screen = new Screen();

	scene = new Scene();
}

Popzors::~Popzors() 
{   
}

bool Popzors::Update() 
{
	Number elapsed = core->getElapsed();

	return core->Update();
}