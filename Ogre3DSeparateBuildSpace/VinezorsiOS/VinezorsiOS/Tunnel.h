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
#include "LevelSet.h"

// Stores the list of tunnel segments
class Tunnel
{
public:
    enum SetPodTarget { UNKNOWN, BAD_TARGET, GOOD_TARGET };
    
    Player* player;
    
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
    int powerupSegmentSize;
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
    int renewalPowerupCounter;
    std::list<Pod*> activePods; // Animating pods
    
    // Stage attributes
    int playNo; //distinguish different tunnel playthroughs in log file
    int stageNo; // level number in level set
    StageMode mode;
    char phase;
    float stageTime;
    float totalElapsed;
    float timePenalty;
    int nback;
    int control;
    Direction basis;
    bool sidesUsed[NUM_DIRECTIONS];
    std::vector<std::string> materialNames;
    Evaluation eval;
    Direction guide;
    
    std::vector<std::vector<PodInfo> > signalTypes;
    int catchupPhase; // Used to update distractors at navPhase
    int tunnelSectionsPerNavigationUpgrade;
    int navPhase; // Put in navigation class later...
    int navCheckpoint; // Counter to tell when to upgrade tunnel when changing sections
    std::vector<NavigationLevel> navLevels;
    std::list<int> propagateCounters;
    
    std::vector<CollectionCriteria> collectionCriteria;
    std::vector<PowerupType> powerups;
    bool hasHoldout;
    int holdoutCounter;
    int holdoutPod;
    int holdoutIndex;
    int holdoutFrequency;
    int trackNBackA;
    int trackNBackB;
    int trackNBackC;
    
    bool multiCollectionTask;
    
    bool done;      // Says stage is over, but not the ending animation
    bool cleanup;   // Totally done, ending animation is over
    bool flyOut;
    
    TunnelSlice* gateSlice;
    float tVal;
    bool gateOpen;
    bool activateGreen;

    float tSpeedOpen;
    
    float tSpeed;
    float tAccel;
    
    float gateDelayTimer;
    float gateDelay;
public:
	Tunnel();
    
	Tunnel(Ogre::SceneNode* parentNode, Vector3 start, Quaternion rot, float segmentWidth, float segmentDepth, int segmentMinAngleTurn, int segmentMaxAngleTurn, int playNo, int stageNo, StageMode mode, char phase, int nback, float stageTime, Direction sloc, int sectionSize, int podSegmentSize, int distractorSegmentSize, int powerupSegmentSize, const std::vector<std::vector<PodInfo> > & signalTypes, const std::vector<PowerupType> & powerups);
	
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
    std::vector<TunnelSlice*> getNSlices(int n) const;
	int getSpawnIndex() const;
	int getSpawnCombo() const;
	int getSpawnLimit() const;
	int getNumTargets() const;
	int getSignalsLeft() const;
	int getSectionIndex() const;
	int getPodIndex() const;
    Quaternion getQuaternion() const;
    Quaternion getCombinedQuaternion(TunnelSlice* slice) const;
    Evaluation getEval() const;
    Pod* getNearestPod(int numSlices) const;
    void getSpanFrom(int span, Direction base, const bool sides[NUM_DIRECTIONS], bool ret[NUM_DIRECTIONS]);
    Direction getRandPossibleDirection(SectionInfo segmentInfo, bool assignGuide = true);
    
	float getSegmentWidth() const;
	float getSegmentDepth() const;
    std::vector<SectionInfo> getSectionInfo() const;
    std::vector<PodInfo> getPodInfo() const;
    Quaternion getNewSegmentQuaternion(Direction dir, int degrees) const;
    // Given n-back to test, return the match if any, otherwise return POD_UNKNOWN for no
    bool testForRepeatSignal(PodSignal ps, int numtimes);
    bool testForRepeatMatch(int nvalue, PodSignal ps, int numtime);
    PodSignal generateItem(int nbackA, int nbackB, int nbackC, int trackA, int trackB, int trackC);
    PodSignal getNBackTest(int nvalue, PodSignal test) const;
    PodSignal getNBackTest(int index, int nvalue) const;
    PodSignal getNBackTest(int nvalue) const;
    // Uses getNBackTest and determines n-back to test by the player's toggleBack
    bool getPodIsGood(int index, int toggle) const;
    bool getPodIsGood(int toggle) const;
    int getNBackToggle(int toggle) const;
    
    StageMode getMode() const;
    float getStageTime() const;
    float getTotalElapsed() const;
    float getTimePenalty() const;
    float getTimeLeft() const;
    float getPercentComplete() const;
    int getNBack() const;
    int getControl() const;
    Direction getBasis() const;
    bool hasAvailableSide(Direction side) const;
    int getPlayNo() const;
    int getStageNo() const;
    char getPhase() const;
    void determineMaterial();
    std::string getMaterialName() const;
    std::vector<NavigationLevel> getNavLevels() const;
    int getNumNavLevels() const;
    int getBuildingNavLevel() const;    // The nav level the tunnel is building
    int getCurrentNavLevel() const;     // The nav level the player is still on
    bool extractPowerup(PowerupType type);
    std::vector<CollectionCriteria> getCollectionCriteria() const;
    
    virtual void checkIfDone();
    bool isDone() const;
    void setDone(Evaluation eval);
    void setSpawnCombo(int level);
    void setSpawnLimit(int value);
    void setNumTargets(int value);
    void upgradeControl();
    void addToTimePenalty(float value);
    void setVisible(bool value);
    void setCleaning(bool value);
    bool needsCleaning() const;
    
    bool getFlyOut();
    
    void setNewControl(int control);
    void updateNavigationLevel();
    void setNavigationLevels(int tunnelSectionsPerNavLevel);
    void setNavigationLevels(int startingNavLevel, int navLimit, int tunnelSectionsPerNavLevel);
    void setNavigationLevels(const std::vector<NavigationLevel> & preset, int tunnelSectionsPerNavLevel);
    void setCollectionCriteria(const std::vector<CollectionCriteria> & value);
    bool satisfyCriteria(int nback, int amount);
    bool killCriteria(int amount);
    int loseRandomCriteria();
    bool areCriteriaFilled() const;
    bool isMultiCollectionTask() const;
    bool setAllCriteriaTo(int value);
    int getLowestCriteria() const;
    int getHighestCriteria() const;
    int getFirstCriteria() const; // just get the n-back of the first item, efficient then scanning list
    int getNumRequiredCriteria() const;
    int getNumSatisfiedCriteria() const;
    int getStarPhase() const;
	void removeSegment();
    
    SectionInfo getNextSectionInfo() const;
    SectionInfo getNextSegmentInfo(SectionInfo sectionInfo) const;
    PodInfo getNextPodInfoAt(SectionInfo segmentInfo, SetPodTarget setting);
    PodInfo getNextPodInfo(SectionInfo segmentInfo, SetPodTarget setting);
    PodInfo getNextPodInfo(SectionInfo segmentInfo);
    std::vector<PodInfo> getNextDistractorInfo(SectionInfo segmentInfo, const std::vector<PodInfo> & signals);
    std::vector<PodInfo> getNextPowerupInfo(SectionInfo segmentInfo, const std::vector<PodInfo> & signals);
    void setPods(TunnelSlice* segment, const std::vector<PodInfo> & podInfos = std::vector<PodInfo>());
    void addSegment(SectionInfo sectionInfo);
	void renewSegment(SectionInfo sectionInfo);
    void addSection(SectionInfo newSection);
    void renewSection(SectionInfo newSection);
    
    bool updateIterators(Vector3 checkPos);
	bool renewIfNecessary();
    
    void unlink();
    void link(Player* player);
    void presetTargets(int level);
    void constructTunnel(const std::string & nameTunnelTile, int size);
    
    void update(float elapsed);
    void respondToToggleCheat();
    
    void setHoldout(bool val, int holdoutFrequency);
    
    void gateAnimation(float elapsed);
    
	~Tunnel();
};

#endif /* defined(__Testing__Tunnel__) */
