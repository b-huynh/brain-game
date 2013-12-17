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

struct PlayerLevel
{
    int nback;
    int control;
    
    PlayerLevel();
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
	double score; // a value of pride
    int points; // money to buy
	bool mouseLeft;
	bool keyUp;
	bool keyDown;
	bool keyLeft;
	bool keyRight;
	
    std::vector<Vine*> vines;
    
    MovementMode movementMode;
	Direction camDir; // direction offset on tunnel for player camera
	Direction vineDir; // direction offset on tunnel for vine
	Vector2 mousePos;
    Vector3 oldPos;
	Vector3 camPos;
    Quaternion oldRot;
    int oldRoll;
    Quaternion camRot;
    int camRoll;
    Quaternion desireRot;
    int desireRoll;
    double camSpeed;
    
	double vineOffset; // offset to camPos in direction of forward
    TunnelSlice* lookback;
    
    SpeedControlMode speedControl;
    
    PlayerLevel level;
    struct Result {
        int stageID;
        int timestamp;
        SectionInfo sectionInfo;
        PodInfo podInfo;
        int nback;
        double speed;
        GameMode gameMode;
        double score;
        ProgressionMode progressionMode;
    };
    std::vector<Result> results;
    
    double totalElapsed;
    double totalDistanceTraveled;
    
    OgreOggSound::OgreOggISound* soundMusic;
    OgreOggSound::OgreOggISound* soundFeedbackGreat;
    OgreOggSound::OgreOggISound* soundFeedbackGood;
    OgreOggSound::OgreOggISound* soundFeedbackBad;
    OgreOggSound::OgreOggISound* soundStartup;
    std::vector<OgreOggSound::OgreOggISound*> soundPods;
    bool triggerStartup;
    
    // iOS Swipe Capabilities
    double inputTotalX;
    bool inputMoved;
public:
    
	Player();
	Player(const std::string & name, const PlayerLevel & level, Vector3 camPos, Quaternion camRot, double camSpeed, double  offset, SpeedControlMode speedControl, unsigned seed, const std::string & filename);
	
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
	double getScore() const;
	int getPoints() const;
	bool getMouseLeft() const;
	bool getKeyUp() const;
	bool getKeyDown() const;
	bool getKeyLeft() const;
	bool getKeyRight() const;
	Direction getCamDir() const;
	Direction getVineDir() const;
	Vector2 getMousePos() const;
	Vector3 getOldPos() const;
	Vector3 getCamPos() const;
	Quaternion getOldRot() const;
    int getOldRoll() const;
	Quaternion getCamRot() const;
	int getCamRoll() const;
	Quaternion getDesireRot() const;
	int getDesireRoll() const;
	double getCamSpeed() const;
	Vector3 getVineOffset() const;
    SpeedControlMode getSpeedControl() const;
    PlayerLevel getLevel() const;
	double getTotalElapsed() const;
	double getTotalDistanceTraveled() const;
    double getAccuracy() const;
    double getProgress(Tunnel* tunnel) const;
    Evaluation getEvaluation(Tunnel* tunnel) const;
    
    void setSeed(unsigned value);
    void setName(const std::string & name);
    void setHP(int value);
    void setNumCorrectTotal(int value);
    void setNumWrongTotal(int value);
    void setNumCorrectCombo(int value);
    void setNumWrongCombo(int value);
	void setScore(double value);
	void setMouseLeft(bool value);
	void setKeyUp(bool value);
	void setKeyDown(bool value);
	void setKeyLeft(bool value);
	void setKeyRight(bool value);
	
	void setCamDir(Direction value);
	void setVineDir(Direction value);
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
    void setCamSpeed(double value);
    void setLevel(PlayerLevel value);
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
    void newTunnel(Tunnel* tunnel, bool setmusic, bool fixspeed = false, bool resetscore = false);
    
	void move(Vector3 delta);
    void changeMovementMode();
	void addVine(Vine* vine);
	void checkCollisions(Tunnel* tunnel);
    
    void resetCursorMoved();
    void setCursorMoved();
    void updateCursorCooldown(double elapsed);
    void checkCursorMove(double dx, double dy);
    bool checkPerformLeftMove();
    bool checkPerformRightMove();
	void update(Tunnel* tunnel, Hud* hud, double elapsed);
    
    void evaluatePlayerLevel(bool pass);
    bool saveProgress(std::string file);
    bool saveStage(std::string file, int lastStageID);
    void setConfigValues();
    
    ~Player();
};

#endif /* defined(__Testing__Player__) */
