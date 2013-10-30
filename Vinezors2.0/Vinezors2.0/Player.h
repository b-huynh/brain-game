//
//  Player.h
//  Testing
//
//  Created by Calvin Phung on 9/13/13.
//
//

#ifndef __Vinezors2_0__Player__
#define __Vinezors2_0__Player__

#pragma once

#include <vector>
#include <fstream>
#include "Util.h"
#include "Vine.h"
#include "Tunnel.h"

struct PlayerLevel
{
    int nback;
    int control;
    
    PlayerLevel() : nback(Util::NBACK), control(1) {}
};

class Player
{
private:
    enum MovementMode { MOVEMENT_STATIC, MOVEMENT_ROTATING };
    
    unsigned seed;
    
    std::string name;
    int hp;
    int numCorrect;
    int numWrong;
	double score;
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
    int camSpeed;
	double vineOffset; // offset to camPos in direction of forward
    
    PlayerLevel level;
    struct Result {
        int timestamp;
        SectionInfo sectionInfo;
        PodInfo podInfo;
        int nback;
    };
    std::vector<Result> results;
    
    double totalElapsed;
    TunnelSlice* vineSlice; // Used for vine movement
    double vineT;
    
    OgreOggISound* soundMusic;
    OgreOggISound* soundFeedbackGood;
public:
    
	Player();
	Player(const std::string & name, const PlayerLevel & level, Vector3 camPos, Quaternion camRot, int camSpeed, double  offset, unsigned seed, const std::string & filename);
	
    unsigned getSeed() const;
    std::string getName() const;
    int getHP() const;
    int getNumCorrect() const;
    int getNumWrong() const;
	double getScore() const;
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
	int getCamSpeed() const;
	Vector3 getVineOffset() const;
    PlayerLevel getLevel() const;
	double getTotalElapsed() const;
    
    void setSeed(unsigned value);
    void setName(const std::string & name);
    void setHP(int value);
    void setNumCorrect(int value);
    void setNumWrong(int value);
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
    void setCamSpeed(int value);
	Vector3 getCamForward(bool combined = true) const;
	Vector3 getCamUpward(bool combined = true) const;
	Vector3 getCamRight(bool combined = true) const;
    Quaternion getRot() const;
    Quaternion getRoll() const;
    Quaternion getCombinedRotAndRoll() const;
    
    void newTunnel(Tunnel* tunnel);
    
	void move(Vector3 delta);
    void changeMovementMode();
	void addVine(Vine* vine);
	void checkCollisions(Tunnel* tunnel);
    
	void update(double elapsed, Tunnel* tunnel);
    
    void evaluatePlayerLevel(bool pass);
    bool saveProgress(std::string file);
    
    ~Player();
};

#endif /* defined(__Testing__Player__) */
