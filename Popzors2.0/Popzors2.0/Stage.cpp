#include "Stage.h"

#include <iostream>

// The two following collision functions below should not be called separately.
// If a pattern should check for both collisions, a new function should be written
void Stage::handlePoppyCollisions(double elapsed)
{
    Ogre::IntersectionSceneQuery* query = OgreFramework::getSingletonPtr()->m_pSceneMgr->createIntersectionQuery();
    Ogre::IntersectionSceneQueryResult result = query->execute();
    
    Ogre::SceneQueryMovableIntersectionList::iterator it = result.movables2movables.begin();
    for ( ; it != result.movables2movables.end(); ++it)
    {
        
        Selectable* obj1 = any_cast<Selectable*>(it->first->getUserAny());
        Selectable* obj2 = any_cast<Selectable*>(it->second->getUserAny());
        
        if (obj1->getType() == Object::TYPE_POPPY && obj2->getType() == Object::TYPE_POPPY)
        {
            static_cast<Poppy*>(obj1)->handleCollision(elapsed, static_cast<Poppy*>(obj2));
        }
    }
}

void Stage::handlePotCollisions(double elapsed)
{
    Ogre::IntersectionSceneQuery* query = OgreFramework::getSingletonPtr()->m_pSceneMgr->createIntersectionQuery();
    Ogre::IntersectionSceneQueryResult result = query->execute();
    
    Ogre::SceneQueryMovableIntersectionList::iterator it = result.movables2movables.begin();
    for ( ; it != result.movables2movables.end(); ++it)
    {
        
        Selectable* obj1 = any_cast<Selectable*>(it->first->getUserAny());
        Selectable* obj2 = any_cast<Selectable*>(it->second->getUserAny());
        
        if (obj1->getType() == Object::TYPE_POT && obj2->getType() == Object::TYPE_POT)
        {
            static_cast<Pot*>(obj1)->handleCollision(elapsed, static_cast<Pot*>(obj2));
        }
    }
}

void Stage::handleBothCollisions(double elapsed)
{
    Ogre::IntersectionSceneQuery* query = OgreFramework::getSingletonPtr()->m_pSceneMgr->createIntersectionQuery();
    Ogre::IntersectionSceneQueryResult result = query->execute();
    
    Ogre::SceneQueryMovableIntersectionList::iterator it = result.movables2movables.begin();
    for ( ; it != result.movables2movables.end(); ++it)
    {
        Selectable* obj1 = any_cast<Selectable*>(it->first->getUserAny());
        Selectable* obj2 = any_cast<Selectable*>(it->second->getUserAny());
     
        // Special conditions to ignore collisions
        if (obj1->getType() == Object::TYPE_GROUND || obj2->getType() == Object::TYPE_GROUND ||
            (obj1->getType() == Object::TYPE_POPPY && !obj1->isSelectable()) ||
            (obj2->getType() == Object::TYPE_POPPY && !obj2->isSelectable()) ||
            selected == obj1 || selected == obj2)
            continue;
        
        Sphere sph1 = obj1->getBody()->getWorldBoundingSphere();
        Sphere sph2 = obj2->getBody()->getWorldBoundingSphere();
        
        const double RESOLUTION_SPEED = randRangeDouble(9.9, 10.0);
        const double EPSILON = 0.001;
        
        double colDist = sph1.getRadius() + sph2.getRadius() - sph1.getCenter().distance(sph2.getCenter()) + EPSILON;
        if (colDist < 0.0) continue;
        Vector3 colVector = (sph1.getCenter() - sph2.getCenter()).normalisedCopy();
        Vector3 dmove = colVector * RESOLUTION_SPEED * elapsed;
        colVector = colVector * colDist;
        if (dmove.squaredLength() > colVector.squaredLength()) // maximum distance to move should not be exceeded
            dmove = colVector;
        dmove = Vector3(dmove.x, 0, dmove.z); // zero out the y-axis
        
        if (obj1->getType() < obj2->getType()) {
            obj1->move(dmove);
        } else if (obj1->getType() > obj2->getType()) {
            obj2->move(-dmove);
        } else {
            obj1->move(dmove / 2);
            obj2->move(-dmove / 2);
        }
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