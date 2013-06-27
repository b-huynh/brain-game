#include "Vinezors.h"

TunnelSlice::TunnelSlice()
	: topLeftWall(NULL), topWall(NULL), topRightWall(NULL), rightWall(NULL), bottomRightWall(NULL), bottomWall(NULL), bottomLeftWall(NULL), leftWall(NULL), 
	topLeftWallBorder(NULL), topWallBorder(NULL), topRightWallBorder(NULL), rightWallBorder(NULL), bottomRightWallBorder(NULL), bottomWallBorder(NULL), bottomLeftWallBorder(NULL), leftWallBorder(NULL),
	light(NULL)
{}

TunnelSlice::TunnelSlice(CollisionScene *scene, Vector3 center, Number width, Number depth, Number intensity)
{
	Number wallLength = width / (2 * cos(PI / 4) + 1);

	topLeftWall = new ScenePrimitive(ScenePrimitive::TYPE_PLANE, wallLength, depth);
	topLeftWall->setPosition(center.x - (width + wallLength) / 4, center.y + (width + wallLength) / 4, center.z);
	topLeftWall->setRoll(225);
	topLeftWall->setMaterialByName("WallMaterial");
	
	topWall = new ScenePrimitive(ScenePrimitive::TYPE_PLANE, wallLength, depth);
	topWall->setPosition(center.x, center.y + width / 2, center.z);
	topWall->setRoll(180);
	topWall->setMaterialByName("WallMaterial");
	
	topRightWall = new ScenePrimitive(ScenePrimitive::TYPE_PLANE, wallLength, depth);
	topRightWall->setPosition(center.x + (width + wallLength) / 4, center.y + (width + wallLength) / 4, center.z);
	topRightWall->setRoll(135);
	topRightWall->setMaterialByName("WallMaterial");

	rightWall = new ScenePrimitive(ScenePrimitive::TYPE_PLANE, wallLength, depth);
	rightWall->setPosition(center.x + width / 2, center.y, center.z);
	rightWall->setRoll(90);
	rightWall->setMaterialByName("WallMaterial");
	
	bottomRightWall = new ScenePrimitive(ScenePrimitive::TYPE_PLANE, wallLength, depth);
	bottomRightWall->setPosition(center.x + (width + wallLength) / 4, center.y - (width + wallLength) / 4, center.z);
	bottomRightWall->setRoll(45);
	bottomRightWall->setMaterialByName("WallMaterial");

	bottomWall = new ScenePrimitive(ScenePrimitive::TYPE_PLANE, wallLength, depth);
	bottomWall->setPosition(center.x, center.y - width / 2, center.z);
	bottomWall->setRoll(0);
	bottomWall->setMaterialByName("WallMaterial");
	
	bottomLeftWall = new ScenePrimitive(ScenePrimitive::TYPE_PLANE, wallLength, depth);
	bottomLeftWall->setPosition(center.x - (width + wallLength) / 4, center.y - (width + wallLength) / 4, center.z);
	bottomLeftWall->setRoll(-45);
	bottomLeftWall->setMaterialByName("WallMaterial");

	leftWall = new ScenePrimitive(ScenePrimitive::TYPE_PLANE, wallLength, depth);
	leftWall->setPosition(center.x - width / 2, center.y, center.z);
	leftWall->setRoll(-90);
	leftWall->setMaterialByName("WallMaterial");
	
	light = new SceneLight(SceneLight::AREA_LIGHT, scene, intensity);
	light->setPosition(center);

	addToCollisionScene(scene);
}

void TunnelSlice::addToCollisionScene(CollisionScene *scene) const
{
	scene->addCollisionChild(topLeftWall, CollisionSceneEntity::SHAPE_PLANE);
	scene->addCollisionChild(topWall, CollisionSceneEntity::SHAPE_PLANE);
	scene->addCollisionChild(topRightWall, CollisionSceneEntity::SHAPE_PLANE);
	scene->addCollisionChild(rightWall, CollisionSceneEntity::SHAPE_PLANE);
	scene->addCollisionChild(bottomRightWall, CollisionSceneEntity::SHAPE_PLANE);
	scene->addCollisionChild(bottomWall, CollisionSceneEntity::SHAPE_PLANE);
	scene->addCollisionChild(bottomLeftWall, CollisionSceneEntity::SHAPE_PLANE);
	scene->addCollisionChild(leftWall, CollisionSceneEntity::SHAPE_PLANE);

	scene->addLight(light);
}

Vinezors::Vinezors(PolycodeView *view)
{
	core = new Win32Core(view, 640,480,false, false, 0, 0,60);	  
	CoreServices::getInstance()->getResourceManager()->addArchive("default.pak");
	CoreServices::getInstance()->getResourceManager()->addDirResource("default", false);
	CoreServices::getInstance()->getResourceManager()->addDirResource("resources", false);
	
	screen = new Screen();
	scene = new CollisionScene();
	origin = Vector3(0, 0, 0);
	
	tunnels.push_back(TunnelSlice(scene, origin, TUNNEL_WIDTH, TUNNEL_DEPTH, LIGHT_INTENSITY));
	tunnels.push_back(TunnelSlice(scene, origin + Vector3(0, 0, -TUNNEL_DEPTH), TUNNEL_WIDTH, TUNNEL_DEPTH, LIGHT_INTENSITY));
	tunnels.push_back(TunnelSlice(scene, origin + Vector3(0, 0, -2 * TUNNEL_DEPTH), TUNNEL_WIDTH, TUNNEL_DEPTH, LIGHT_INTENSITY));

	camPos = Vector3(origin.x, origin.y, origin.z + TUNNEL_DEPTH / 2);
	scene->getDefaultCamera()->setPosition(camPos);
	scene->getDefaultCamera()->lookAt(origin);
}

Vinezors::~Vinezors() 
{   
}

bool Vinezors::Update() 
{
	Number elapsed = core->getElapsed();

	/*
	if (scene->getNumEntities() > 1) {
	std::stringstream ss;
	ss << scene->getEntity(1)->getPosition().x << " " 
	   << scene->getEntity(1)->getPosition().y << " "
	   << scene->getEntity(1)->getPosition().z << " "
	   << scene->getEntity(1)->getPitch();
	label->setText(ss.str());
	}
	*/
	return core->Update();
}