//
//  History.cpp
//  Vinezors2.0
//
//  Created by Calvin Phung on 10/14/13.
//
//

#include "History.h"

extern Util::ConfigGlobal globals;

static int seatID = 0;
static int coverID = 0;

const float PANEL_HEIGHT = 18;
const float PANEL_X = 0.0;
const float PANEL_Y = -10.0;

History::History(Ogre::SceneManager* sceneMgr, int nback)
: sceneMgr(sceneMgr), sidebarMode(SIDEBAR_NONE), nback(nback), seats(), list(), lightNodeSide(NULL), coverNode(NULL), coverDest(),
panelHeight(PANEL_HEIGHT), panelX(PANEL_X), panelY(PANEL_Y), reservedHeight(), coverHeight(), coverOffsetY(), coverInd(), mode(FIRST_TIME)
{
    initSeats(nback);
    
    //Util::createSphere(OgreFramework::getSingletonPtr()->m_pSceneMgrSide, "sphereMesh", 1.0, 16, 16);
    //Util::createCylinder(OgreFramework::getSingletonPtr()->m_pSceneMgrSide, "cylinderMesh", 1.0, 1.0, 16);
    //Util::createDiamond(OgreFramework::getSingletonPtr()->m_pSceneMgrSide, "diamondMesh", 1.0, 1.0);
    //Util::createPlane(OgreFramework::getSingletonPtr()->m_pSceneMgrSide, "planeMesh", 1.0, 1.0);
     Light* lightSide = OgreFramework::getSingletonPtr()->m_pSceneMgrSide->createLight("Light");
     lightSide->setDiffuseColour(1.0, 1.0, 1.0);
     lightSide->setSpecularColour(1.0, 1.0, 1.0);
     lightNodeSide = OgreFramework::getSingletonPtr()->m_pSceneMgrSide->getRootSceneNode()->createChildSceneNode("lightNode");
     lightNodeSide->attachObject(lightSide);
     lightNodeSide->setPosition(OgreFramework::getSingletonPtr()->m_pCameraSide->getPosition());
    setSidebar();
}

void History::initSeats(int nback)
{
    this->nback = nback;
    
    const int SIZE = 6;
    
    reservedHeight = panelHeight / (SIZE);
    
    ++seatID;
    
    float x = panelX;
    float y = panelY + panelHeight;
    for (int i = 0; i < SIZE; ++i)
    {
        float midx = x;
        float midy = y + reservedHeight / 2;
        
        SceneNode* node = sceneMgr->getRootSceneNode()->createChildSceneNode("seatNode" + Util::toStringInt(seatID));
        
        Entity* nodeEntity = node->getCreator()->createEntity("seatEntity" + Util::toStringInt(seatID), "planeMesh");
        
        if (i == nback - 1)
            nodeEntity->setMaterialName("General/PodYellow");
        else
            nodeEntity->setMaterialName("General/PodUnknown");
        node->attachObject(nodeEntity);
        node->setPosition(Vector3(midx, midy, 0));
        node->pitch(Degree(90));
        node->scale(globals.seatLength, globals.seatLength, globals.seatLength);
        
        x += 0;
        y -= reservedHeight;
        
        ++seatID;
        seats.push_back(node);
    }
    
    coverNode = sceneMgr->getRootSceneNode()->createChildSceneNode("coverNode" + Util::toStringInt(coverID));
    
    coverHeight = 2 * panelHeight;
    Entity* coverEntity = coverNode->getCreator()->createEntity("seatEntity" + Util::toStringInt(seatID), "planeMesh");
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
    
    Pod* cpy = new Pod(sceneMgr->getRootSceneNode(), Vector3(loc.x, loc.y + reservedHeight, 0), Vector3(globals.podStemLength, loc.y + reservedHeight, globals.podStemLength), POD_BASIC, podInfo.podSignal, podInfo.podColor, podInfo.podShape, podInfo.podSound, NO_DIRECTION, globals.podStemRadius, globals.podHeadRadius);
    cpy->setMoveSpeed(5.0);
    cpy->uncloakPod();
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
            list[i]->setDest(Vector3(list[i]->getPosition().x, list[i]->getPosition().y - reservedHeight, list[i]->getPosition().z));
    }
}

void History::uncloakPod()
{
    if (list.size() > 0)
        list[0]->uncloakPod();
}

void History::update(float elapsed)
{
    for (int i = 0; i < list.size(); ++i)
        list[i]->update(elapsed);
    
    Vector3 dist = coverDest - coverNode->getPosition();
    Vector3 move;
    
    float distlen2 = dist.squaredLength();
    move = dist * 1.5 * elapsed;
    
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
        coverDest = seats[seatInd - 1]->getPosition() + Vector3(0.0, coverOffsetY - reservedHeight, globals.podHeadRadius + Util::EPSILON);
    }
    else
        coverDest = seats[seatInd]->getPosition() + Vector3(0.0, coverOffsetY, globals.podHeadRadius + Util::EPSILON);
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
            }
            else
            {
                coverInd = nback - 1;
            }
            break;
        }
    }
}

void History::setSidebar()
{
    Camera* m_pCameraMain = OgreFramework::getSingletonPtr()->m_pCameraMain;
    Camera* m_pCameraSide = OgreFramework::getSingletonPtr()->m_pCameraSide;
    
    Viewport* m_pViewportMain = OgreFramework::getSingletonPtr()->m_pViewportMain;
    Viewport* m_pViewportSide = OgreFramework::getSingletonPtr()->m_pViewportSide;
    
    switch (sidebarMode)
    {
        case SIDEBAR_NONE:
            m_pViewportMain->setDimensions(
                                           0.0,
                                           0.0,
                                           float(globals.viewportMainWidth_modeNone) / globals.screenWidth,
                                           float(globals.viewportMainHeight_modeNone) / globals.screenHeight);
            m_pCameraMain->setAspectRatio(Real(m_pViewportMain->getActualWidth()) / Real(m_pViewportMain->getActualHeight()));
            
            m_pCameraSide->setOrthoWindow(0.0, 0.0);
            m_pViewportSide->setDimensions(
                                           float(globals.viewportMainWidth_modeNone) / globals.screenWidth,
                                           0.0,
                                           float(globals.viewportSideWidth_modeNone) / globals.screenWidth,
                                           float(globals.viewportSideHeight_modeNone) / globals.screenHeight);
            m_pCameraSide->setAspectRatio(Real(0.0));
            
            break;
        case SIDEBAR_RIGHT:
            m_pViewportMain->setDimensions(
                                           0.0,
                                           0.0,
                                           float(globals.viewportMainWidth_modeRight) / globals.screenWidth,
                                           float(globals.viewportMainHeight_modeRight) / globals.screenHeight);
            m_pCameraMain->setAspectRatio(Real(m_pViewportMain->getActualWidth()) / Real(m_pViewportMain->getActualHeight()));
            
            m_pCameraSide->setPosition(Vector3(0, 0, 30));
            m_pCameraSide->lookAt(Vector3(0, 0, 0));
            m_pCameraSide->setNearClipDistance(1);
            m_pCameraSide->setOrthoWindow(10.0, 25.0);
            m_pViewportSide->setDimensions(
                                           float(globals.viewportMainWidth_modeRight) / globals.screenWidth,
                                           0.0,
                                           float(globals.viewportSideWidth_modeRight) / globals.screenWidth,
                                           float(globals.viewportSideHeight_modeRight) / globals.screenHeight);
            m_pCameraSide->setAspectRatio(Real(m_pViewportSide->getActualWidth()) / Real(m_pViewportSide->getActualHeight()));
            
            break;
        case SIDEBAR_BOTTOM_LTR:
            m_pViewportMain->setDimensions(
                                           0.0,
                                           0.0,
                                           float(globals.viewportMainWidth_modeBottom) / globals.screenWidth,
                                           float(globals.viewportMainHeight_modeBottom) / globals.screenHeight);
            m_pCameraMain->setAspectRatio(Real(m_pViewportMain->getActualWidth()) / Real(m_pViewportMain->getActualHeight()));
            
            m_pCameraSide->setPosition(Vector3(0, 0, 30));
            m_pCameraSide->lookAt(Vector3(0, 0, 0));
            m_pCameraSide->setNearClipDistance(1);
            m_pCameraSide->roll(Degree(-90));
            m_pCameraSide->setOrthoWindow(5, 2.5);
            m_pViewportSide->setDimensions(
                                           0.0,
                                           float(globals.viewportMainHeight_modeBottom) / globals.screenHeight,
                                           float(globals.viewportSideWidth_modeBottom) / globals.screenWidth,
                                           float(globals.viewportSideHeight_modeBottom) / globals.screenHeight);
            m_pCameraSide->setAspectRatio(Real(m_pViewportSide->getActualWidth()) / Real(m_pViewportSide->getActualHeight()));
            
            break;
        case SIDEBAR_BOTTOM_RTL:
            m_pViewportMain->setDimensions(
                                           0.0,
                                           0.0,
                                           float(globals.viewportMainWidth_modeBottom) / globals.screenWidth,
                                           float(globals.viewportMainHeight_modeBottom) / globals.screenHeight);
            m_pCameraMain->setAspectRatio(Real(m_pViewportMain->getActualWidth()) / Real(m_pViewportMain->getActualHeight()));
            
            m_pCameraSide->setPosition(Vector3(0, 0, 30));
            m_pCameraSide->lookAt(Vector3(0, 0, 0));
            m_pCameraSide->setNearClipDistance(1);
            m_pCameraSide->roll(Degree(90));
            m_pCameraSide->setOrthoWindow(5, 2.5);
            m_pViewportSide->setDimensions(
                                           0.0,
                                           float(globals.viewportMainHeight_modeBottom) / globals.screenHeight,
                                           float(globals.viewportSideWidth_modeBottom) / globals.screenWidth,
                                           float(globals.viewportSideHeight_modeBottom) / globals.screenHeight);
            m_pCameraSide->setAspectRatio(Real(m_pViewportSide->getActualWidth()) / Real(m_pViewportSide->getActualHeight()));
            
            break;
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