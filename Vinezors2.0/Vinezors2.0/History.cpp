//
//  History.cpp
//  Vinezors2.0
//
//  Created by Calvin Phung on 10/14/13.
//
//

#include "History.h"

static int seatID = 0;
static int coverID = 0;

const double PANEL_HEIGHT = 18;
const double PANEL_X = 0.0;
const double PANEL_Y = -10.0;

History::History(Ogre::SceneManager* sceneMgr, int nback)
: sceneMgr(sceneMgr), nback(nback), seats(), list(), coverNode(NULL), coverDest(),
    panelHeight(PANEL_HEIGHT), panelX(PANEL_X), panelY(PANEL_Y), reservedHeight(), coverHeight(), coverOffsetY(), coverInd(), mode(FIRST_TIME)
{
    initSeats(nback);
}

void History::initSeats(int nback)
{
    this->nback = nback;
    
    const int SIZE = 6;
    
    reservedHeight = panelHeight / (SIZE);
    
    ++seatID;
    
    double x = panelX;
    double y = panelY + panelHeight;
    for (int i = 0; i < SIZE; ++i)
    {
        double midx = x;
        double midy = y + reservedHeight / 2;
        
        SceneNode* node = sceneMgr->getRootSceneNode()->createChildSceneNode("seatNode" + Util::toStringInt(seatID));
        
        Entity* nodeEntity = node->getCreator()->createEntity("seatEntity" + Util::toStringInt(seatID), "seatMesh");
        
        if (i == nback - 1)
            nodeEntity->setMaterialName("General/Smiley");
        else
            nodeEntity->setMaterialName("General/PodUnknown");
        node->attachObject(nodeEntity);
        node->setPosition(Vector3(midx, midy, 0));
        node->pitch(Degree(90));
        node->scale(Util::SEAT_LENGTH, Util::SEAT_LENGTH, Util::SEAT_LENGTH);
        
        x += 0;
        y -= reservedHeight;
        
        ++seatID;
        seats.push_back(node);
    }
    
    coverNode = sceneMgr->getRootSceneNode()->createChildSceneNode("coverNode" + Util::toStringInt(coverID));
    
    coverHeight = 2 * panelHeight;
    Entity* coverEntity = coverNode->getCreator()->createEntity("seatEntity" + Util::toStringInt(seatID), "seatMesh");
    coverEntity->setMaterialName("General/BaseWhite");
    coverNode->attachObject(coverEntity);
    coverOffsetY = reservedHeight / 2 - coverHeight / 2;
    coverInd = seats.size();
    setCoverDest(seats.size());
    coverNode->setPosition(coverDest);
    coverNode->scale(coverHeight, coverHeight, coverHeight);
    coverNode->pitch(Degree(90));

    ++coverID;
}

void History::addPod(const PodInfo & podInfo)
{
    Vector3 loc = seats[0]->getPosition();
    
    Pod* cpy = new Pod(sceneMgr, Vector3(loc.x, loc.y + Util::POD_STEM_LENGTH, 0), Vector3(Util::POD_STEM_LENGTH, loc.y, Util::POD_STEM_LENGTH), podInfo.podType, Util::POD_HEAD_RADIUS, Util::POD_STEM_LENGTH);
    cpy->revealPod();
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
        //if (covered[i] && i != 0)
        //    list[i]->hidePod();
        //else
        //list[i]->revealPod();
        
        if (i < seats.size())
            list[i]->setDest(Vector3(seats[i]->getPosition().x, seats[i]->getPosition().y, list[i]->getPosition().z));
        else
            list[i]->setDest(Vector3(list[i]->getPosition().x, list[i]->getPosition().y - Util::POD_STEM_LENGTH, list[i]->getPosition().z));
    }
}

void History::revealPod()
{
    if (list.size() > 0)
        list[0]->revealPod();
}

void History::update(double elapsed)
{
    for (int i = 0; i < list.size(); ++i)
        list[i]->update(elapsed);
    
    Vector3 dist = coverDest - coverNode->getPosition();
    Vector3 move;
    
    double distlen2 = dist.squaredLength();
    move = dist * 2 * elapsed;
    
    if (move.squaredLength() >= distlen2)
        coverNode->setPosition(coverDest);
    else
        coverNode->translate(move);
}

void History::setCoverDest(int seatInd)
{
    if (seatInd >= seats.size())
    {
        seatInd = seats.size();
        coverDest = seats[seatInd - 1]->getPosition() + Vector3(0.0, coverOffsetY - reservedHeight, Util::POD_HEAD_RADIUS + Util::EPSILON);
    }
    else
        coverDest = seats[seatInd]->getPosition() + Vector3(0.0, coverOffsetY, Util::POD_HEAD_RADIUS + Util::EPSILON);
}

void History::determineCoverLoc(bool success)
{
    switch (mode)
    {
        case FIRST_TIME:
        {
            if (success) {
                mode = PLAYING;
                coverInd = nback - 1;
                saveInd = coverInd;
                setCoverDest(coverInd);
            }
            break;
        }
        case PLAYING:
        {
            if (success)
            {
                coverInd = coverInd / 2;
                if (coverInd < saveInd)
                {
                    setCoverDest(coverInd);
                    saveInd = coverInd;
                }
                std::cout << coverInd << " " << saveInd << std::endl;
            }
            else
            {
                coverInd = nback - 1;
                std::cout << coverInd << " " << saveInd << std::endl;
            }
            break;
        }
    }
}

History::~History()
{
    for (int i = 0; i < seats.size(); ++i)
    {
        seats[i]->getCreator()->destroyMovableObject(seats[i]->getAttachedObject(0)); // Assuming only one entity
        seats[i]->removeAndDestroyAllChildren();
        seats[i]->getCreator()->destroySceneNode(seats[i]);
    }
    
    coverNode->getCreator()->destroyMovableObject(coverNode->getAttachedObject(0)); // Assuming only one entity
    coverNode->removeAndDestroyAllChildren();
    coverNode->getCreator()->destroySceneNode(coverNode);
    
    for (int i = 0; i < list.size(); ++i)
    {
        list[i]->removeFromScene();
        delete list[i];
    }
    list.clear();
    seats.clear();
}