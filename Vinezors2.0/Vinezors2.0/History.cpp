//
//  History.cpp
//  Vinezors2.0
//
//  Created by Calvin Phung on 10/14/13.
//
//

#include "History.h"

static int seatID = 0;

const double PANEL_HEIGHT = 20;
const double PANEL_X = 0;
const double PANEL_Y = -7.5;

History::History(Ogre::SceneManager* sceneMgr, int nback)
: sceneMgr(sceneMgr), nback(nback), covered(), seats(), list()
{
    initSeats(nback);
}

void History::initSeats(int nback)
{
    this->nback = nback;
    
    const double RESERVED_HEIGHT = PANEL_HEIGHT / (nback + 1);
    
    double x = PANEL_X;
    double y = PANEL_Y;
    covered = std::vector<bool>(nback + 1);
    seats = std::vector<SceneNode*>(nback + 1);
    
    for (int i = 0; i < covered.size(); ++i) {
        if (i <= covered.size() / 2 + 1)
            covered[i] = true;
        else
            covered[i] = false;
    }
    covered[covered.size() - 1] = true;
    
    for (int i = 0; i < seats.size(); ++i)
    {
        double midx = x;
        double midy = y + RESERVED_HEIGHT / 2;
        
        SceneNode* node = sceneMgr->getRootSceneNode()->createChildSceneNode("seatNode" + Util::toStringInt(seatID));
        
        Entity* nodeEntity = node->getCreator()->createEntity("seatEntity" + Util::toStringInt(seatID), "seatMesh");
        
        if (i == 0 || i == seats.size() - 1)
            nodeEntity->setMaterialName("General/Smiley");
        else
            nodeEntity->setMaterialName("General/PodUnknown");
        node->attachObject(nodeEntity);
        node->setPosition(Vector3(midx, midy, 0));
        node->pitch(Degree(90));
        
        x += 0;
        y += RESERVED_HEIGHT;
        
        ++seatID;
        seats[i] = node;
    }
    
}

void History::addPod(const PodInfo & podInfo)
{
    double yLoc = seats[0]->getPosition().y;
    
    const double RESERVED_HEIGHT = PANEL_HEIGHT / (nback + 1);
    
    Pod* cpy = new Pod(sceneMgr, Vector3(Util::POD_STEM_LENGTH, yLoc, 0), Vector3(Util::POD_STEM_LENGTH, yLoc, Util::POD_STEM_LENGTH), podInfo.podType, Util::POD_HEAD_RADIUS, Util::POD_STEM_LENGTH);
    list.insert(list.begin(), cpy);
    if (list.size() > seats.size() + 1)
    {
        Pod* rmv = list.back();
        rmv->removeFromScene();
        delete rmv;
        list.pop_back();
    }
    
    for (int i = 0; i < list.size(); ++i)
    {
        if (covered[i] && i != 0)
            list[i]->hidePod();
        else
            list[i]->revealPod();
        if (i < seats.size())
            list[i]->setDest(Vector3(seats[i]->getPosition().x, seats[i]->getPosition().y, list[i]->getPosition().z));
        else
            list[i]->setDest(Vector3(list[i]->getPosition().x, list[i]->getPosition().y + RESERVED_HEIGHT, list[i]->getPosition().z));
    }
}

void History::update(double elapsed)
{
    for (int i = 0; i < list.size(); ++i)
        list[i]->update(elapsed);
}

History::~History()
{
    for (int i = 0; i < seats.size(); ++i)
    {
        seats[i]->getCreator()->destroyMovableObject(seats[i]->getAttachedObject(0)); // Assuming only one entity
        seats[i]->removeAndDestroyAllChildren();
        seats[i]->getCreator()->destroySceneNode(seats[i]);
    }
    for (int i = 0; i < list.size(); ++i)
    {
        list[i]->removeFromScene();
        delete list[i];
    }
    list.clear();
    seats.clear();
}