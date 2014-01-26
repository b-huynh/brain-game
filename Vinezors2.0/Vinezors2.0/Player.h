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

struct PlayerStagePerformance
{
    int stageID;
    int stars;
    
    PlayerStagePerformance() : stageID(0), stars(0) {}
};

class Player
{
private:
    enum MovementMode { MOVEMENT_STATIC, MOVEMENT_ROTATING };
    
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
	float score; // a value of pride
    int points; // money to buy
    int stars;
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
    float finalSpeed;
    
	float vineOffset; // offset to camPos in direction of forward
    TunnelSlice* lookback;
    Pod* earlySelect;
    float glowSpeed;
    
    SpeedControlMode speedControl;
    
    struct Result {
        int stageID;
        int timestamp;
        SectionInfo sectionInfo;
        PodInfo podInfo;
        int nback;
        float speed;
        GameMode gameMode;
        float score;
    };
    std::vector<Result> results;
    std::vector<PlayerStagePerformance> performances;
    
    float totalElapsed;
    float totalDistanceTraveled;
    float animationTimer;
    float speedTimer;
    float glowTimer;
    
    OgreOggSound::OgreOggISound* soundMusic;
    OgreOggSound::OgreOggISound* soundFeedbackGreat;
    OgreOggSound::OgreOggISound* soundFeedbackGood;
    OgreOggSound::OgreOggISound* soundFeedbackBad;
    OgreOggSound::OgreOggISound* soundCollision;
    OgreOggSound::OgreOggISound* soundStartup;
    std::vector<OgreOggSound::OgreOggISound*> soundPods;
    bool triggerStartup;
    
    // iOS Swipe Capabilities
    float inputTotalX;
    bool inputMoved;
    float inputLeftBound;
    float inputRightBound;
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
	float getScore() const;
	int getPoints() const;
    int getStars() const;
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
    float getProgress(Tunnel* tunnel) const;
    bool getShowCombo() const;
    
    void setSeed(unsigned value);
    void setName(const std::string & name);
    void setHP(int value);
    void setNumCorrectTotal(int value);
    void setNumWrongTotal(int value);
    void setNumCorrectCombo(int value);
    void setNumWrongCombo(int value);
	void setScore(float value);
    void setPoints(int value);
    void setStars(int value);
	void setMouseLeft(bool value);
	void setKeyUp(bool value);
	void setKeyDown(bool value);
	void setKeyLeft(bool value);
	void setKeyRight(bool value);
	void setKeySpace(bool value);
	
	void setCamDir(Direction value);
	bool setVineDirRequest(Direction value, Tunnel* tunnel);
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
    void saveCam();
    void revertCam();
	Vector3 getCamForward(bool combined = true) const;
	Vector3 getCamUpward(bool combined = true) const;
	Vector3 getCamRight(bool combined = true) const;
    Quaternion getRot() const;
    Quaternion getRoll() const;
    Quaternion getCombinedRotAndRoll() const;
    void playPodSound(int index) const;
    
    void setSounds(bool mode);
    void newTunnel(Tunnel* tunnel, bool setmusic);
    
	void move(Vector3 delta);
    void changeMovementMode();
    void setShowCombo(bool value);
	void addVine(Vine* vine);
	void checkCollisions(Tunnel* tunnel);
    
    void resetCursorMoved();
    void setCursorMoved();
    void updateCursorCooldown(float elapsed);
    void checkCursorMove(float dx, float dy);
    bool checkPerformLeftMove(bool force);
    bool checkPerformRightMove(bool force);
    void setStars(Tunnel* tunnel);
    void performShockwave(Tunnel* tunnel);
    void testPodGiveFeedback(Tunnel* tunnel, Pod* test);
	void update(Tunnel* tunnel, Hud* hud, float elapsed);
    
    bool saveStage(std::string file);
    bool saveProgress(std::string file, int lastStageID);
    bool loadProgress(std::string savePath);
    
    ~Player();
};

#endif /* defined(__Testing__Player__) */
