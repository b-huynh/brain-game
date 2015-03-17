//
//  Player.h
//  Testing
//
//  Created by Calvin Phung on 9/13/13.
//
//

#ifndef __Vinezors2_0__Player__
#define __Vinezors2_0__Player__

#include <vector>
#include <fstream>
#include <map>
#include "Util.h"
#include "Vine.h"
#include "Tunnel.h"
#include "Powerup.h"
#include "PlayerLevel.h"
#include "PlayerProgress.h"
#include "LevelSet.h"
#include "TutorialManager.h"
#include "LevelScheduler.h"

class Player
{
private:
    enum MovementMode { MOVEMENT_STATIC, MOVEMENT_ROTATING };
    
    LevelSet* levels;
    Tunnel* tunnel;
    
    unsigned seed;
    int sessionID;
    
    std::string name;
    int hp;
    int numCorrectTotal;
    int numSafeTotal;
    int numMissedTotal;
    int numWrongTotal;
    int numIgnoredTotal;    // Unsuccessful Pickups
    int numPickupsTotal;    // Successful Pickups
    int numCorrectBonus;
    int numCorrectCombo;
    int numWrongCombo;
    int numAvoidancesTotal;
    int numCollisionsTotal;
    float score;
	bool mouseLeft;
	bool keyUp;
	bool keyDown;
	bool keyLeft;
	bool keyRight;
	bool keySpace;
	
    std::vector<Vine*> vines;
    
    MovementMode movementMode;
    bool showCombo;
    
	Direction camDir; // direction offset on tunnel for player camera
	Vector2 mousePos;
    Vector3 oldPos;
	Vector3 camPos;
    Quaternion oldRot;
    float oldRoll;
    Quaternion camRot;
    float camRoll;
    Quaternion desireRot;
    float desireRoll;
    float baseSpeed;
    float bonusSpeed;
    float finalSpeed;
    float initSpeed;
    float minSpeed;
    float maxSpeed;
    
	float vineOffset; // offset to camPos in direction of forward
    TunnelSlice* lookback;
    Pod* selectedTarget;
    float glowSpeed;
    
    int toggleBack; // 0, 1, or 2
    
    // Logging Info:
    // Encoding on each panel where a lower number can be overridden by a higher number
    static const char ENC_NO_PANEL = '0';
    static const char ENC_EMPTY_PANEL = '1';
    static const char ENC_PLAYER = '2';
    static const char ENC_MATCH = '3';
    static const char ENC_NONMATCH = '4';
    static const char ENC_OBSTACLE = '5';
    static const char ENC_TIMEWARP = '6';
    // Results recorded at each segment in a tunnel
    struct Result {
        char segmentEncoding[NUM_DIRECTIONS];
        int eventID;
        int levelID;
        int taskType;
        int nback;
        int toggle;
        int playerRollBase;
        int playerRollOffset;
        int playerRollSpeed;
        float playerFuelTimer;
        float playerFuelBuffer;
        Direction playerLoc;
        PodInfo podInfo;
        SectionInfo sectionInfo;
        int timestamp;
        bool levelEnded;
        float minSpeed;
        float maxSpeed;
        float baseSpeed;
        float finalSpeed;
    };
    // Player actions recorded here (does not track panning)
    struct Action {
        int eventID;
        int levelID;
        ActionCode action;
        int timestamp;
        int baseSpeed;
        int finalSpeed;
    };
    // Stores information about each tunnel level
    struct Session {
        int sessionID;
        int eventID;
        int levelID;
        int taskType;
        bool hasHoldout;
        int timestampIn;
        int timestampOut;
        int nback;
        int runSpeedIn;
        int runSpeedOut;
        int eval;
        float accuracy;
        int TP;
        int FP;
        int TN;
        int FN;
        int ignored;
        int pickups;
        int obsHit;
        int obsAvoided;
        int score;
        int totalMarbles;
        float nbackLevelA;
        float nbackLevelB;
        float nbackLevelC;
        float nbackLevelD;
        float nbackLevelE;
        float scoreCurr;
        float currentHoldout;
        float holdoutOffsetA;
        float holdoutOffsetB;
        float holdoutOffsetD;
        float speedA;
        float speedB;
        float speedC;
        float speedD;
        float speedE;
        float musicVolume;
        float soundVolume;
        bool syncDataToServer;
        float maxVel;
        float minVelFree;
        float minVelStopper;
        float dampingDecayFree;
        float dampingDecayStop;
        float dampingDropFree;
        float dampingDropStop;
        bool inverted;
    };
    
    bool logged;
    std::list<Result> results;
    std::list<Action> actions;
    std::list<Session> sessions;
    PlayerLevel skillLevel;
    
    std::map<std::string,Powerup*> powerups;
    
    float totalElapsed;
    float totalDistanceTraveled;
    float animationTimer;
    float speedTimer;
    float badFuelPickUpTimer;
    float boostTimer;
    float selectTimer;
    bool selectTimerFlag;
    float startMusicTimer;
    bool godMode;
    
    Ogre::ColourValue boostColor;
    
    OgreOggSound::OgreOggISound* soundMusic;
    OgreOggSound::OgreOggISound* soundFeedbackGreat;
    OgreOggSound::OgreOggISound* soundFeedbackGood;
    OgreOggSound::OgreOggISound* soundFeedbackBad;
    OgreOggSound::OgreOggISound* soundFeedbackMiss;
    OgreOggSound::OgreOggISound* soundCollision;
    OgreOggSound::OgreOggISound* soundStartup;
    OgreOggSound::OgreOggISound* soundBoost;
    OgreOggSound::OgreOggISound* soundButtonPress;
    OgreOggSound::OgreOggISound* soundFirework;
    bool triggerStartup;
    
    // Study report at the end of the session
    int numStagesWon;
    
    // Stage
    int levelRequestRow;
    int levelRequestCol;
    // Level Selection
    int menuRowIndex;
    
    // Level Results
    std::vector< std::vector<PlayerProgress> > levelProgress;
    
    // Tutorials
    TutorialManager* tutorialMgr;
    
public:
    // Marble Restart Counter of Study Version
    int marbleChoice = 0;
    int choice0RestartCounter = 0;  //Tutorial Counter
    int choice1RestartCounter = 0;
    int choice2RestartCounter = 0;
    int choice3RestartCounter = 0;
    int numRetries = 3;
    
    bool fuelEnabled = false;   //Study Settings
    bool holdoutEnabled = false; //Study Settings
    int initialVelocity = 10; //Study Settings
    bool manRecessEnabled = false; //Study Settings
    int manRecessLevelLimit = 1; //StudySettings
    int manRecessCount = 0; //StudySettings
    bool newNavEnabled = false; //StudySettings
    float newNavIncrement = .35f; //Study Settings
    
    bool indRecessEnabled = false; //StudySettings
    float indRecessIncrement = .5f; //Study Settings
    double indRecessNBackLevel = 1.0;
    float indRecessNBackDelta = 0.0f;
    
    bool holdoutdelayEnabled = false;
    float holdoutdelayNumber = 2.5f;
    
    bool sessionTimeSettings = false;
    int sessionStartTime = 20;
    int sessionEndTime = 30;
    int numOfSessions = 20;
    
    bool sessionNeedsIncrement = true;
    
    //End Study Settings

    
    bool fadeMusic;
    float xsTimer; // timer for the three X's display
    
    // Settings Parameters
    float musicVolume;
    float soundVolume;
    float holdout;
    float holdoutLB;
    float holdoutUB;
    bool syncDataToServer;
    
    float maxVel;
    float minVelFree;       // not recorded and set by player
    float minVelStopper;
    float dampingDecayFree;
    float dampingDecayStop;
    float dampingDropFree;
    float dampingDropStop;
    bool inverted;
    
    float rollSpeed;
    float offsetRoll;
    float offsetRollDest;
    
	Player();
	Player(const std::string & name, Vector3 camPos, Quaternion camRot, float camSpeed, float offset, unsigned seed, const std::string & filename);
    
    Tunnel* getTunnel() const;
    LevelSet* getLevels() const;
    unsigned getSeed() const;
    std::string getName() const;
    int getHP() const;
    int getNumCorrectTotal() const;
    int getNumSafeTotal() const;
    int getNumMissedTotal() const;
    int getNumWrongTotal() const;
    int getNumPickupsTotal() const;
    int getNumCorrectBonus() const;
    int getNumCorrectCombo() const;
    int getNumWrongCombo() const;
    int getNumCollisionsTotal() const;
    float getScore() const;
	bool getMouseLeft() const;
	bool getKeyUp() const;
	bool getKeyDown() const;
	bool getKeyLeft() const;
	bool getKeyRight() const;
	bool getKeySpace() const;
	Direction getCamDir() const;
	Direction getVineDir() const;
	Direction getVineDest() const;
	Vector2 getMousePos() const;
	Vector3 getOldPos() const;
	Vector3 getCamPos() const;
    Vector3 getPos() const;
	Quaternion getOldRot() const;
    float getOldRoll() const;
	Quaternion getCamRot() const;
	float getCamRoll() const;
	Quaternion getDesireRot() const;
	float getDesireRoll() const;
	float getBaseSpeed() const;
	float getFinalSpeed() const;
	float getTotalSpeed() const;
	float getVineOffset() const;
	float getTotalElapsed() const;
	float getTotalDistanceTraveled() const;
	float getAnimationTimer() const;
    float getAccuracy() const;
    float getProgress() const;
    bool getShowCombo() const;
    PlayerLevel getSkillLevel() const;
    int getSessionID() const;
    std::string getStageStats() const;
    std::string getSessionStats() const;
    int getToggleBack() const;
    bool getGodMode() const;
    int getNumStagesWon() const;
    int getLevelRequestRow() const;
    int getLevelRequestCol() const;
    bool hasLevelProgress(int level) const;
    bool hasLevelProgress(int row, int col) const;
    PlayerProgress getLevelProgress(int level) const;
    PlayerProgress getLevelProgress(int row, int col) const;
    bool isLevelAvailable(int level) const;
    bool isLevelAvailable(int row, int col) const;
    bool isNextLevelAvailable() const;
    int getNextLevel() const;
    int getMenuRowIndex() const;
    int getTotalLevelRating(int row) const;
    float getTotalLevelScore(int row) const;
    float getTotalLevelScore() const;
    Ogre::ColourValue getBoostColor() const;
    bool hasTriggeredStartup() const;
    float getScoring() const;
    TutorialManager* getTutorialMgr() const;
    
    void setRunningSpeed(int val1, int val2, int val3, int val4, int nav);
    void setSpeedParameters(int initSpeed, int minSpeed, int maxSpeed);
    void setSeed(unsigned value);
    void setName(const std::string & name);
    void setHP(int value);
    void setNumCorrectTotal(int value);
    void setNumWrongTotal(int value);
    void setNumCorrectCombo(int value);
    void setNumWrongCombo(int value);
    void setScore(float value);
	void setMouseLeft(bool value);
	void setKeyUp(bool value);
	void setKeyDown(bool value);
	void setKeyLeft(bool value);
	void setKeyRight(bool value);
	void setKeySpace(bool value);
	
	void setCamDir(Direction value);
	bool setVineDirRequest(Direction value, bool force = false);
	void setMousePos(Vector2 value);
	void setOldPos(Vector3 value);
	void setCamPos(Vector3 value);
    void setPos(Vector3 value);
	void setOldRot(Quaternion value);
    void setOldRoll(float value);
	void setCamRot(Quaternion value);
    void setCamRoll(float value);
	void setDesireRot(Quaternion value);
    void setDesireRoll(float value);
    void setBaseSpeed(float value);
    void setSkillLevel(PlayerLevel value);
    void setSessionID(int value);
    void setToggleBack(int value);
    void setGodMode(bool value);
    void setLevelRequestRow(int value);
    void setLevelRequestCol(int value);
    void setLevelRequest(int row, int col);
    void setMenuRowIndex(int value);
    void setAllProgressTo(const PlayerProgress & value);
    void saveCam();
    void revertCam();
	Vector3 getCamForward(bool combined = true) const;
	Vector3 getCamUpward(bool combined = true) const;
	Vector3 getCamRight(bool combined = true) const;
    Quaternion getRot() const;
    Quaternion getRoll() const;
    Quaternion getCombinedRotAndRoll() const;
    void playSound(OgreOggSound::OgreOggISound* sound) const;
    void reactGUI() const;
    void playFireworkSound() const;
    float getStartMusicTimer() const;
    void playMusic() const;
    void stopMusic();
    void updateMusicVolume();
    void unpause();
    void pause();
    
    void setSounds();
    
    void unlink();
    void link(Tunnel* tunnel);
    void initToggleBack();
    void newTunnel(const std::string & nameMusic);
    void startMenu();
    void saveSpeedSettings();
    
	void move(Vector3 delta);
    void changeMovementMode();
    void setShowCombo(bool value);
	void addVine(Vine* vine);
    void removeVines();
	void checkCollisions();
    void updateSpeed(int mean, bool step);
    void decideFinalSpeed(float elapsed);
    
    void addAction(ActionCode actType);
    void updateTractorBeam(float elapsed);
    void performTractorBeam();
    void updateTimeWarp(float elapsed);
    void performTimeWarp();
    void updateBoost(float elapsed);
    void setBoostTimer(float value);
    void performBoost();
    void updateShields(float elapsed);
    bool triggerShields();
    void performShields();
    void updateGlowExtraction(float elapsed);
    void setGlowGrabParameters(Pod* pod);
    void beginBadFuelPickUp();
    void updateBadFuelPickUp(float elapsed);
    void testPodGiveFeedback(Pod* test);
    void determineSpawnCombo();
    void offsetShip(float elapsed);
    void recordInfo();
	void update(float elapsed);
    
    void initPowerUps();
    void setPowerUp(std::string pwr, bool value);
    bool isPowerUpAvailable(std::string pwr);
    bool isPowerUpActive(std::string pwr);
    Powerup* getPowerUpPtr(std::string pwr);
    void performPowerUp(std::string pwr);
    void destroyPowerUps();
    void resetPowerUps();
    
    void incrementNumStagesWon();
    void calculateNavigationScores();
    void calculateSpeedScores();
    std::string getCurrentStats() const;
    
    void saveAllResults(Evaluation eval);
    void logData();
    bool saveStage(std::string file);
    bool saveActions(std::string file);
    bool saveSession(std::string file);
    //bool saveProgress(std::string file, bool updateSessionID);
    bool saveProgress(std::string file);
    bool loadProgress1_5(std::string savePath);
    bool loadProgress(std::string savePath);
    
    std::istream& setSaveValue(std::istream& in, std::string paramName, std::map<std::string, bool> ignoreList);
    
    void initSettings();
    
    bool endFlag;
    
    // =====================
    // Scheduler Stuffs
    // =========================================================
    LevelScheduler* scheduler;
    std::pair<StageRequest, PlayerProgress>* levelRequest;
    std::pair<StageRequest, PlayerProgress> scheduleChoice1;
    std::pair<StageRequest, PlayerProgress> scheduleChoice2;
    std::pair<StageRequest, PlayerProgress> scheduleChoice3;
    LevelPhase lastPlayed;
    
    void feedLevelRequestFromSchedule();
    
    // Returns a multiplier when incrementing or decrementing memory level during assessment
    float getMemoryChallenge(StageRequest level, PlayerProgress assessment) const;
    float modifyNBackDelta(StageRequest level, PlayerProgress assessment, float accuracy, bool exclude);
    float obtainDifficultyWeight(StageRequest level, PlayerProgress assessment, float nBackDelta);
    float obtainSamplingWeight(StageRequest level, PlayerProgress assessment);
    
    // "Grade" the level to see if player should repeat, go back, or advance
    float obtainDeltaNBack(std::pair<StageRequest, PlayerProgress>* levelToGrade, float accuracy) const;
    void assessLevelPerformance(std::pair<StageRequest, PlayerProgress>* levelToGrade);
    
    ~Player();
};

#endif /* defined(__Testing__Player__) */
