//
//  Tunnel.h
//  Testing
//
//  Created by Calvin Phung on 9/10/13.
//
//

#ifndef __Vinezors2_0__Tunnel__
#define __Vinezors2_0__Tunnel__

#include <list>
#include <vector>
#include "Util.h"
#include "TunnelSlice.h"
#include "History.h"

// Added GAME_TEACHING C.P.
enum GameMode { GAME_PROFICIENCY, GAME_TIMED, GAME_NAVIGATION, GAME_TEACHING, GAME_RECESS };

// Stores the list of tunnel segments
class Tunnel
{
public:
    enum SetPodTarget { UNKNOWN, BAD_TARGET, GOOD_TARGET };
    
    Player* player;
    Hud* hud;
    
    SceneNode* parentNode;
    
    SceneNode* mainTunnelNode;
	Vector3 start;
	Vector3 end;
    std::list<TunnelSlice*> segments;
    float tLeftPrevious;
    float tLeftCurrent;
    std::list<TunnelSlice*>::iterator previous;
    std::list<TunnelSlice*>::iterator current;
    float tLeftOffsetPrevious;
    float tLeftOffsetCurrent;
    std::list<TunnelSlice*>::iterator previousOffset;
    std::list<TunnelSlice*>::iterator currentOffset;
    int segmentCounter;
	float segmentWidth;
	float segmentDepth;
	int segmentMinAngleTurn;
	int segmentMaxAngleTurn;
    Quaternion endRot;
    
    std::vector<SectionInfo> sections;
    std::vector<PodInfo> types;
    
    struct TargetInfo
    {
        bool level1;
        bool level2;
        bool level3;
    };
    std::vector<TargetInfo> targets;
    int sectionSize;
    int podSegmentSize;
    int distractorSegmentSize;
    int spawnIndex; // The current index to spawn which pod.
    int spawnCombo;
    int spawnLimit;
    int numTargets;
    
    // These indices used to track current player's spot
    int sectionIndex;
    int podIndex;
    int renewalSectionCounter;
    int renewalPodCounter;
    int renewalDistractorCounter;
    std::list<Pod*> activePods; // Animating pods
    
    // Stage attributes
    int stageNo;
    GameMode mode;
    char phase;
    float totalElapsed;
    float timePenalty;
    int nback;
    int control;
    History* history;
    Direction basis;
    bool sidesUsed[NUM_DIRECTIONS];
    std::vector<std::string> materialNames;
    Evaluation eval;
    Direction guide;
    
    std::vector<std::vector<PodInfo> > signalTypes;
    int catchupPhase; // Used to update distractors at navPhase
    int navPhase; // Put in navigation class later...
    int navCheckpoint; // Counter to tell when to upgrade tunnel when changing sections
    std::vector<NavigationLevel> navLevels;
    std::list<int> propagateCounters;
    
    bool done;      // Says stage is over, but not the ending animation
    bool cleanup;   // Totally done, ending animation is over
public:
	Tunnel();
    
	Tunnel(Ogre::SceneNode* parentNode, Vector3 start, Quaternion rot, float segmentWidth, float segmentDepth, int segmentMinAngleTurn, int segmentMaxAngleTurn, int stageNo, GameMode mode, char phase, int nback, int control, Direction sloc, int sectionSize, int podSegmentSize, int distractorSegmentSize, const std::vector<std::vector<PodInfo> > & signalTypes);
	
    SceneNode* getMainTunnelNode() const;
	Vector3 getStart() const;
	Vector3 getEnd() const;
	Vector3 getCenter() const;
    float getTLeftPrevious() const;
    float getTLeftCurrent() const;
    float getTLeftOffsetPrevious() const;
    float getTLeftOffsetCurrent() const;
    std::list<TunnelSlice*>::const_iterator getPreviousIterator() const;
    std::list<TunnelSlice*>::const_iterator getCurrentIterator() const;
    std::list<TunnelSlice*>::const_iterator getBeginIterator() const;
    std::list<TunnelSlice*>::const_iterator getEndIterator() const;
    std::vector<TunnelSlice*> findSlicesSincePrevious() const;
    std::vector<TunnelSlice*> findSlicesSincePreviousOffset() const;
    void setOffsetIterators(Vector3 pos, float tOffset);
    TunnelSlice* findSliceFrom(std::list<TunnelSlice*>::const_iterator stit, Vector3 pos, float tOffset, float & tLeft) const;
	TunnelSlice* getPrevious() const;
	TunnelSlice* getCurrent() const;
	TunnelSlice* getPreviousOffset() const;
	TunnelSlice* getCurrentOffset() const;
	TunnelSlice* getFront() const;
	TunnelSlice* getBack() const;
	TunnelSlice* getNext(int i) const;
	int getSpawnIndex() const;
	int getSpawnCombo() const;
	int getSpawnLimit() const;
	int getNumTargets() const;
	int getTotalCollections() const;
	int getSignalsLeft() const;
	int getSectionIndex() const;
	int getPodIndex() const;
    Quaternion getQuaternion() const;
    Quaternion getCombinedQuaternion(TunnelSlice* slice) const;
    History* getHistory() const;
    Evaluation getEval() const;
    Pod* getNearestPod(int numSlices) const;
    void getSpanFrom(int span, Direction base, const bool sides[NUM_DIRECTIONS], bool ret[NUM_DIRECTIONS]);
    Direction getRandPossibleDirection(SectionInfo segmentInfo, bool assignGuide = true);
    
	float getSegmentWidth() const;
	float getSegmentDepth() const;
    std::vector<SectionInfo> getSectionInfo() const;
    std::vector<PodInfo> getPodInfo() const;
    Quaternion getNewSegmentQuaternion(Direction dir, int degrees) const;
    PodSignal getNBackTest(int section) const;
    GameMode getMode() const;
    float getTotalElapsed() const;
    float getTimePenalty() const;
    float getTimeLeft() const;
    int getNBack() const;
    int getControl() const;
    Direction getBasis() const;
    bool hasAvailableSide(Direction side) const;
    int getStageNo() const;
    char getPhase() const;
    void determineMaterial();
    std::string getMaterialName() const;
    std::vector<NavigationLevel> getNavLevels() const;
    int getNumNavLevels() const;
    int getBuildingNavLevel() const;    // The nav level the tunnel is building
    int getCurrentNavLevel() const;     // The nav level the player is still on
    
    virtual void checkIfDone();
    bool isDone() const;
    void setDone(Evaluation eval);
    void setSpawnCombo(int level);
    void upgradeControl();
    void addToTimePenalty(float value);
    void setCleaning(bool value);
    bool needsCleaning() const;
    
    void setNewControl(int control);
    void updateNavigationLevel();
    void setNavigationLevels();
    void setNavigationLevels(const std::vector<NavigationLevel> & preset);
	void removeSegment();
    
    SectionInfo getNextSectionInfo() const;
    SectionInfo getNextSegmentInfo(SectionInfo sectionInfo) const;
    PodInfo getNextPodInfoAt(SectionInfo segmentInfo, SetPodTarget setting);
    PodInfo getNextPodInfo(SectionInfo segmentInfo, SetPodTarget setting);
    PodInfo getNextPodInfo(SectionInfo segmentInfo);
    std::vector<PodInfo> getNextDistractorInfo(SectionInfo segmentInfo, PodInfo signal = PodInfo());
    void setPods(TunnelSlice* segment, const std::vector<PodInfo> & podInfos = std::vector<PodInfo>());
    void addSegment(SectionInfo sectionInfo);
	void renewSegment(SectionInfo sectionInfo);
    void addSection(SectionInfo newSection);
    void renewSection(SectionInfo newSection);
    
    bool updateIterators(Vector3 checkPos);
	bool renewIfNecessary();
    
    std::vector<Pod *> findPodCollisions(SceneNode* ent);
    
    void unlink();
    void link(Player* player, Hud* hud);
    void presetTargets(int level);
    void constructTunnel(int size); // Removed preset param C.P.
    
    void update(float elapsed);
    
	~Tunnel();
};

#endif /* defined(__Testing__Tunnel__) */
