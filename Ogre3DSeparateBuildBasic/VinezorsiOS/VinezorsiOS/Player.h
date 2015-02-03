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
#include "Util.h"
#include "Vine.h"
#include "Tunnel.h"
#include "Score.h"

struct PlayerLevel {
    int sessionID;
    
    // Reps
    int set1Rep;
    int set2Rep;
    int set3Rep;
    
    // Ranks
    int set1; // Color/Sound
    int set2; // Shape/Sound
    int set3; // Sound only
    
    // New
    int set1Notify; // Color/Sound
    int set2Notify; // Shape/Sound
    int set3Notify; // Sound only
    
    // From Recall
    float nbackLevel;
    
    int navigation;
    
    int minSpeed;
    int averageSpeed;
    int maxSpeed;
    
    int runSpeed1;
    int runSpeed2;
    int runSpeed3;
    
    ScoreSet navigationScores;
    ScoreSet speedScores;
    
    int getMasteredNBack() const;
    
    int getHighestNBack() const;
    
    int getNavLimit() const;
    
    PlayerLevel();
    
    void calculateNavigation();
    void calculateSpeed();
    std::string getCurrentStats() const;
};

class Player
{
private:
    enum MovementMode { MOVEMENT_STATIC, MOVEMENT_ROTATING };
    
    Tunnel* tunnel;
    Hud* hud;
    
    unsigned seed;
    
    std::string name;
    int hp;
    int numCorrectTotal;
    int numSafeTotal;
    int numMissedTotal;
    int numWrongTotal;
    int numCorrectBonus;
    int numCorrectCombo;
    int numWrongCombo;
    int numAvoidancesTotal;
    int numCollisionsTotal;
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
    int oldRoll;
    Quaternion camRot;
    int camRoll;
    Quaternion desireRot;
    int desireRoll;
    float baseSpeed;
    float bonusSpeed;
    float finalSpeed;
    float minSpeed;
    float maxSpeed;
    
	float vineOffset; // offset to camPos in direction of forward
    TunnelSlice* lookback;
    Pod* selectedTarget;
    float glowSpeed;
    
    SpeedControlMode speedControl;
    
    //static const char ENC_NO_PANEL = '-';
    //static const char ENC_YES_PANEL = '+';
    //static const char ENC_OBSTACLE_NO_HIT = '=';
    //static const char ENC_OBSTACLE_YES_HIT = '#';
    //static const char ENC_PLAYER_STATIC = 'P';
    //static const char ENC_PLAYER_CORRECT = 'O';
    //static const char ENC_PLAYER_WRONG = 'X';
    //static const char ENC_TARGET = 'T';
    //static const char ENC_FAKE = 'F';
    static const char ENC_NO_PANEL = '0';
    static const char ENC_YES_PANEL = '1';
    static const char ENC_TARGET = '2';
    static const char ENC_FAKE = '3';
    static const char ENC_OBSTACLE_NO_HIT = '4';
    static const char ENC_OBSTACLE_YES_HIT = '5';
    static const char ENC_PLAYER_STATIC = '6';
    static const char ENC_PLAYER_CORRECT = '7';
    static const char ENC_PLAYER_WRONG = '8';
    struct Result {
        char segmentEncoding[NUM_DIRECTIONS];
        int eventID;
        int taskType;
        int nback;
        int navigation;
        Direction playerLoc;
        PodInfo podInfo;
        SectionInfo sectionInfo;
        int timestamp;
        float minSpeed;
        float maxSpeed;
        float baseSpeed;
        float finalSpeed;
        int navScore;
    };
    struct Action {
        int eventID;
        ActionCode action;
        int timestamp;
        int baseSpeed;
        int finalSpeed;
    };
    struct Session {
        int sessionNo;
        int eventID;
        int taskType;
        int stageTime;
        int timestampIn;
        int timestampOut;
        int nback;
        int rep;
        int runSpeedIn;
        int runSpeedOut;
        int maxSpeed;
        int navScore;
        float accuracy;
        int TP;
        int FP;
        int TN;
        int FN;
        int obsHit;
        int obsAvoided;
        float schedulerLevel;
    };
    std::list<Result> results;
    std::list<Action> actions;
    std::list<Session> sessions;
    PlayerLevel skillLevel;
    
    float totalElapsed;
    float totalDistanceTraveled;
    float animationTimer;
    float speedTimer;
    float shockwaveTimer;
    float boostTimer;
    float selectTimer;
    float startMusicTimer;
    
    OgreOggSound::OgreOggISound* soundMusic;
    OgreOggSound::OgreOggISound* soundFeedbackGreat;
    OgreOggSound::OgreOggISound* soundFeedbackGood;
    OgreOggSound::OgreOggISound* soundFeedbackBad;
    OgreOggSound::OgreOggISound* soundCollision;
    OgreOggSound::OgreOggISound* soundStartup;
    OgreOggSound::OgreOggISound* soundBoost;
    std::vector<OgreOggSound::OgreOggISound*> soundPods;
    bool triggerStartup;
    
    // Swipe Capabilities (Note: iOS now uses the ViewController on top of ogre, this is more for the OS X version now
    float inputTotalX;
    bool inputMoved;
    float inputLeftBound;
    float inputRightBound;
    
    int numStagesWon;
public:
    
	Player();
	Player(const std::string & name, Vector3 camPos, Quaternion camRot, float camSpeed, float  offset, SpeedControlMode speedControl, unsigned seed, const std::string & filename);
	
    unsigned getSeed() const;
    std::string getName() const;
    int getHP() const;
    int getNumCorrectTotal() const;
    int getNumSafeTotal() const;
    int getNumMissedTotal() const;
    int getNumWrongTotal() const;
    int getNumCorrectBonus() const;
    int getNumCorrectCombo() const;
    int getNumWrongCombo() const;
    int getNumCollisionsTotal() const;
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
	Quaternion getOldRot() const;
    int getOldRoll() const;
	Quaternion getCamRot() const;
	int getCamRoll() const;
	Quaternion getDesireRot() const;
	int getDesireRoll() const;
	float getBaseSpeed() const;
	float getFinalSpeed() const;
	float getTotalSpeed() const;
	Vector3 getVineOffset() const;
    SpeedControlMode getSpeedControl() const;
	float getTotalElapsed() const;
	float getTotalDistanceTraveled() const;
	float getAnimationTimer() const;
    float getAccuracy() const;
    float getProgress() const;
    bool getShowCombo() const;
    PlayerLevel getSkillLevel() const;
    int getNumStagesWon() const;
    
    void setRunningSpeed(int val1, int val2, int val3, int val4, int nav);
    void setSeed(unsigned value);
    void setName(const std::string & name);
    void setHP(int value);
    void setNumCorrectTotal(int value);
    void setNumWrongTotal(int value);
    void setNumCorrectCombo(int value);
    void setNumWrongCombo(int value);
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
	void setOldRot(Quaternion value);
    void setOldRoll(int value);
	void setCamRot(Quaternion value);
    void setCamRoll(int value);
	void setDesireRot(Quaternion value);
    void setDesireRoll(int value);
    void setBaseSpeed(float value);
    void setSkillLevel(PlayerLevel value);
    void saveCam();
    void revertCam();
	Vector3 getCamForward(bool combined = true) const;
	Vector3 getCamUpward(bool combined = true) const;
	Vector3 getCamRight(bool combined = true) const;
    Quaternion getRot() const;
    Quaternion getRoll() const;
    Quaternion getCombinedRotAndRoll() const;
    void playPodSound(int index) const;
    void unpause();
    void pause();
    
    void setSounds(bool mode);
    
    void unlink();
    void link(Tunnel* tunnel, Hud* hud);
    void newTunnel(bool setmusic);
    
	void move(Vector3 delta);
    void changeMovementMode();
    void setShowCombo(bool value);
	void addVine(Vine* vine);
	void checkCollisions();
    
    void resetCursorMoved();
    void setCursorMoved();
    void updateCursorCooldown(float elapsed);
    void checkCursorMove(float dx, float dy);
    bool checkPerformLeftMove(bool force);
    bool checkPerformRightMove(bool force);
    void addAction(ActionCode actType);
    void updateGlowExtraction(float elapsed);
    void setGlowGrabParameters();
    void performShockwave();
    void updateShockwave(float elapsed);
    void performBoost();
    void updateBoost(float elapsed);
    void testPodGiveFeedback(Pod* test);
    void determineSpawnCombo();
	void update(float elapsed);
    
    void incrementNumStagesWon();
    void calculateNavigationScores();
    void calculateSpeedScores();
    std::string getCurrentStats() const;
    bool saveStage(std::string file);
    bool saveActions(std::string file);
    bool saveSession(std::string file);
    bool saveProgress(std::string file, bool updateSessionID);
    bool loadProgress(std::string savePath);
    
    ~Player();
};

#endif /* defined(__Testing__Player__) */
