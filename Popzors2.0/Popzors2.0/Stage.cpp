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
            dmove.normalise();
            obj1->setVelocity(dmove * obj1->getVelocity().length());
        } else if (obj1->getType() > obj2->getType()) {
            obj2->move(-dmove);
            dmove.normalise();
            obj2->setVelocity(dmove * -obj2->getVelocity().length());
        } else {
            obj1->move(dmove / 2);
            obj2->move(-dmove / 2);
            if (obj1->getType() == Object::TYPE_POPPY)
            {
                dmove.normalise();
                obj1->setVelocity(dmove * obj1->getVelocity().length() / 2);
                obj2->setVelocity(dmove * -obj2->getVelocity().length() / 2);
            }
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

void Stage::update(double elapsed)
{
    ground->update(elapsed);
    for (int i = 0; i < poppies.size(); ++i)
        poppies[i]->update(elapsed);
    for (int i = 0; i < pots.size(); ++i)
        pots[i]->update(elapsed);
}

void Stage::setupHUD()
{
    // The code snippet below is used to output text
    // create a font resource
    ResourcePtr resourceText = OgreFramework::getSingletonPtr()->m_pFontMgr->create("Arial",ResourceGroupManager::DEFAULT_RESOURCE_GROUP_NAME);
    resourceText->setParameter("type","truetype");
    resourceText->setParameter("source","C64_User_Mono_v1.0-STYLE.ttf");
    resourceText->setParameter("size","16");
    resourceText->setParameter("resolution","96");
    resourceText->load();
    
    // Create a panel
    OverlayContainer* panel = static_cast<OverlayContainer*>(OgreFramework::getSingletonPtr()->m_pOverlayMgr->createOverlayElement("Panel", "HudInterface"));
    panel->setMetricsMode(GMM_PIXELS);
    panel->setPosition(10, 10);
    panel->setDimensions(10, 10);
    
    BorderPanelOverlayElement* healthArea = static_cast<BorderPanelOverlayElement*>(
                                                                                    OgreFramework::getSingletonPtr()->m_pOverlayMgr->createOverlayElement("BorderPanel", "HealthAreaBorder"));
    healthArea->setMetricsMode(GMM_RELATIVE);
    healthArea->setPosition(Util::HP_BAR_XREF - 0.01, Util::HP_BAR_YREF - 0.01);
    healthArea->setDimensions(Util::HP_BAR_WIDTH + 0.02, Util::HP_BAR_HEIGHT + 0.02);
    healthArea->setMaterialName("BaseWhite");
    
    barHP = static_cast<PanelOverlayElement*>(
                                              OgreFramework::getSingletonPtr()->m_pOverlayMgr->createOverlayElement("Panel", "HealthBar"));
    barHP->setMetricsMode(GMM_RELATIVE);
    barHP->setPosition(Util::HP_BAR_XREF, Util::HP_BAR_YREF);
    barHP->setDimensions(0.0, 0.0);
    barHP->setMaterialName("General/BaseRed");
    
    // Create text area
    label1= static_cast<TextAreaOverlayElement*>(
                                                 OgreFramework::getSingletonPtr()->m_pOverlayMgr->createOverlayElement("TextArea", "TextAreaLabel1"));
    label1->setMetricsMode(GMM_PIXELS);
    label1->setPosition(Util::LABEL1_POSX, Util::LABEL1_POSY);
    label1->setCharHeight(26);
    label1->setFontName("Arial");
    label1->setColour(ColourValue::White);
    
    // Create text area
    label2= static_cast<TextAreaOverlayElement*>(
                                                 OgreFramework::getSingletonPtr()->m_pOverlayMgr->createOverlayElement("TextArea", "TextAreaLabel2"));
    label2->setMetricsMode(GMM_PIXELS);
    label2->setPosition(Util::LABEL2_POSX, Util::LABEL2_POSY);
    label2->setCharHeight(26);
    label2->setColour(ColourValue::White);
    label2->setFontName("Arial");
    
    // Create an overlay, and add the panel
    Overlay* overlay = OgreFramework::getSingletonPtr()->m_pOverlayMgr->create("OverlayHelloWorld");
    overlay->add2D(panel);
    panel->addChild(label1);
    panel->addChild(label2);
    panel->addChild(healthArea);
    panel->addChild(barHP);
    overlay->show();
}