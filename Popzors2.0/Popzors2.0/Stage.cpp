#include "Stage.h"

#include <iostream>


// Simple inefficient algorithm that pushes poppies away from each other if they collide
void Stage::handlePoppyCollisions(double elapsed)
{
    Ogre::IntersectionSceneQuery* query = OgreFramework::getSingletonPtr()->m_pSceneMgr->createIntersectionQuery();
    Ogre::IntersectionSceneQueryResult result = query->execute();
    
    Ogre::SceneQueryMovableIntersectionList::iterator it = result.movables2movables.begin();
    for ( ; it != result.movables2movables.end(); ++it)
    {
        
        Selectable* obj1 = any_cast<Selectable*>(it->first->getUserAny());
        Selectable* obj2 = any_cast<Selectable*>(it->second->getUserAny());
        
        if (obj1->getType() == Selectable::TYPE_POPPY && obj2->getType() == Selectable::TYPE_POPPY)
        {
            std::cout << "COLLISION" << std::endl;
            static_cast<Poppy*>(obj1)->handleCollision(elapsed, static_cast<Poppy*>(obj2));
        }
         
    }
}

void Stage::handlePotCollisions(double elapsed)
{
    for (int i = 0; i < pots.size(); ++i)
        for (int j = i + 1; j < pots.size(); ++j)
        {
            //pots[i]->handleCollision(elapsed, scene, pots[j]);
        }
}


// Returns input data for clicking
/*
ClickedResult Stage::rayTest(Vector3 origin, Vector3 dest)
{
	ClickedResult res;
	res.poppy = NULL;
    res.pot = NULL;
	RayTestResult ray = scene->getFirstEntityInRay(origin, dest);
	for (int i = 0; i < poppies.size(); ++i) {
		if (poppies[i]->hasEntity(ray.entity)) {
            res.poppy = poppies[i];
		}
	}
    
	for (int i = 0; i < pots.size(); ++i)
		if (pots[i]->hasEntity(ray.entity))
            res.pot = pots[i];
    
    res.ray = ray;
    
	return res;
}

ClickedResult Stage::rayTest(Vector3 origin, Vector3 dest)
{
	ClickedResult res;
	res.poppy = NULL;
    res.pot = NULL;
	RayTestResult ray = scene->getFirstEntityInRay(origin, dest);
	for (int i = 0; i < poppies.size(); ++i) {
		if (poppies[i]->hasEntity(ray.entity)) {
                res.poppy = poppies[i];
		}
	}
    
	for (int i = 0; i < pots.size(); ++i)
		if (pots[i]->hasEntity(ray.entity))
            res.pot = pots[i];
    
    res.ray = ray;
    
	return res;
}

ClickedResult Stage::getClicked(InputEvent * inputEvent)
{
    Vector2 mousePos = inputEvent->mousePosition;
    Vector3 dir = CoreServices::getInstance()->getRenderer()->projectRayFrom2DCoordinate(mousePos.x, mousePos.y);
    
    ClickedResult res = rayTest(scene->getDefaultCamera()->getPosition(), dir * 1000);
    
    res.eventCode = inputEvent->getEventCode();
    
    return res;
}
*/