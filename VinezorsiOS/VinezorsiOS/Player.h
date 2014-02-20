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

struct Score
{
    int right;
    int wrong;
    float accuracy;
    
    void computeAccuracy()
    {
        accuracy = right + wrong > 0 ? static_cast<float>(right) / (right + wrong) : 0.0;
    }
    
    Score() : right(0), wrong(0), accuracy(0.0) {}
};
struct ScoreSet
{
    std::map<int, Score> scores;
    
    int findMin() const
    {
        if (scores.size() <= 0) return 0;
        return scores.begin()->first;
    }
    
    int findMax() const
    {
        if (scores.size() <= 0) return 0;
        return scores.rbegin()->first;
    }
    
    int find(int x) const
    {
        return scores.find(x) != scores.end() ? x : -1;
    }
    
    std::map<int, Score>::const_iterator findMedian() const
    {
        std::map<int, Score>::const_iterator ret = scores.begin();
        std::advance(ret, scores.size() / 2);
        return ret;
    }
    
    std::pair<float, float> calculateLRAccuracies(std::map<int, Score>::const_iterator divider) const
    {
        std::pair<float, float> ret;
        std::pair<int, int> rights(0, 0);
        std::pair<int, int> wrongs(0, 0);
        
        std::map<int, Score>::const_iterator it = scores.begin();
        for (; it != divider; ++it)
        {
            rights.first += it->second.right;
            wrongs.first += it->second.wrong;
        }
        for (; it != scores.end(); ++it)
        {
            rights.second += it->second.right;
            wrongs.second += it->second.wrong;
        }
        if (rights.first + wrongs.first == 0)
            ret.first = 1.0;
        else
            ret.first = static_cast<float>(rights.first) / (rights.first + wrongs.first);
        if (rights.second + wrongs.second == 0)
            ret.second = 0.0;
        else
            ret.second = static_cast<float>(rights.second) / (rights.second + wrongs.second);
        return ret;
    }
    
    
    Score& operator[](int i)
    { return scores[i]; }
    const Score& at(int i) const
    { return scores.at(i); }
    
    // Old Algorithm
    int findBest(int defaultBest, float Zoffset, float lowLimit) const
    {
        float lacksum = 0.0;
        int best = defaultBest;
        for (std::map<int, Score>::const_iterator it = scores.begin(); it != scores.end(); ++it)
        {
            lacksum += (Zoffset - it->second.accuracy);
            if (lacksum <= lowLimit)
                best = it->first;
        }
        return best;
    }
    
    int findNumRight() const
    {
        int total = 0;
        for (std::map<int, Score>::const_iterator it = scores.begin(); it != scores.end(); ++it)
            total += it->second.right;
        return total;
    }
    
    int findNumWrong() const
    {
        int total = 0;
        for (std::map<int, Score>::const_iterator it = scores.begin(); it != scores.end(); ++it)
            total += it->second.wrong;
        return total;
    }
    
    void updateAccuracies()
    {
        for (std::map<int, Score>::iterator it = scores.begin(); it != scores.end(); ++it)
            it->second.computeAccuracy();
    }
    
    float findTotalAccuracy() const
    {
        int nr = findNumRight();
        int nw = findNumWrong();
        if (nw + nr <= 0)
            return 0.0;
        return static_cast<float>(nr) / (nr + nw);
    }
    
    void setMinOptMax(int & nmin, int & noptimal, int & nmax) const
    {
        const float desiredLeft = 1.00;
        const float desiredRight = 0.25;
        std::map<int, Score>::const_iterator best = scores.begin();
        std::pair<float, float> accs(0.0, 1.0);
        std::pair<float, float> err = std::pair<float, float>(pow(desiredLeft - accs.first, 2), pow(desiredRight - accs.second, 2));
        
        std::map<int, Score>::const_iterator divider;
		
        // Find the divider
        divider = scores.begin();
        while (divider != scores.end())
        {
            // Can be more efficient at recomputing scores
            std::pair<float, float> naccs = calculateLRAccuracies(divider);
            std::pair<float, float> nerr = std::pair<float, float>(pow(desiredLeft - naccs.first, 2), pow(desiredRight - naccs.second, 2));
            if (divider == scores.end())
                std::cout << "Line: " << scores.rbegin()->first << "/" << scores.rbegin()->first + 1 << std::endl;
            else
                std::cout << "Line: " << divider->first - 1 << "/" << divider->first << std::endl;
            std::cout << "Acc: " << naccs.first << " " << naccs.second << " " << naccs.first + naccs.second << std::endl;
            std::cout << "Err: " << nerr.first << " " << nerr.second << " " << nerr.first + nerr.second << std::endl;
            if (nerr.first + nerr.second <= err.first + err.second)
            {
                best = divider;
                accs = naccs;
                err = nerr;
            }
            divider++;
        }
        if (best == scores.end())
            nmax = scores.rbegin()->first + 1;
        else
            nmax = best->first;
        std::cout << "Drawn Line: " << nmax - 1 << "/" << nmax << std::endl;
        
        // Find the optimal play speed
        std::map<int, Score>::const_iterator optimal = best;
        std::map<int, Score>::const_iterator previous;
        
        const float improvementThreshold = 0.01;
        divider = best;
        while (divider != scores.begin())
        {
            optimal = divider;
            previous = divider;
            --divider;
            std::pair<float, float> naccs2 = calculateLRAccuracies(previous);
            std::pair<float, float> naccs1 = calculateLRAccuracies(divider);
            if (naccs1.first - naccs2.first < improvementThreshold)
                break;
        }
        noptimal = optimal->first - 1;
        std::cout << "Optimal: " << noptimal << std::endl;
        
        const float pgrowthLimit = 1.25;
        float pgrowth = static_cast<float>(nmax) / noptimal;
        if (pgrowth < pgrowthLimit) //
        {
            pgrowth = pgrowthLimit;
            noptimal = nmax / pgrowth;
        }
        nmin = noptimal / pgrowth;
    }
    
    void print() const
    {
        std::cout << "Scores: \n";
        for (map<int, Score>::const_iterator it = scores.begin(); it != scores.end(); ++it)
        {
            std::cout << it->first << ": " << it->second.right << " " << it->second.wrong << std::endl;
        }
    }
};
struct PlayerLevel {
    // Reps
    int set1Rep;
    int set2Rep;
    int set3Rep;
    
    // Ranks
    int set1; // Color/Sound
    int set2; // Shape/Sound
    int set3; // Sound only
    int navigation;
    
    int minSpeed;
    int averageSpeed;
    int maxSpeed;
    ScoreSet navigationScores;
    ScoreSet speedScores;
    int getMasteredNBack() const
    {
        return std::min(set1, std::min(set2, set3));
    }
    
    PlayerLevel() : set1Rep(0), set2Rep(0), set3Rep(0), set1(2), set2(2), set3(2), navigation(1), minSpeed(5.0), averageSpeed(15.0), maxSpeed(25.0), navigationScores(), speedScores() {}
    
    void calculateNavigation()
    {
        navigationScores.updateAccuracies();
        //navigation = findBest(navigationScores, 0, 0.90, 0.20) * 2; // Phases in navigation test are packed into 2.
        
        int nextNavLevel = navigationScores.findMax();
        if (navigationScores.find(navigation) >= 0) // Did the player reach his/her current nav level?
            nextNavLevel = navigation;
        
        // Starting at current nav level, determine whether we should go up or down
        const float THRESHOLD_PASS = 0.85;
        const float THRESHOLD_DROP = 0.70;
        while (navigationScores.find(nextNavLevel) >= 0 &&
               navigationScores[nextNavLevel].accuracy >= THRESHOLD_PASS)
            ++nextNavLevel;
        while (navigationScores.find(nextNavLevel) >= 0 &&
               navigationScores[nextNavLevel].accuracy < THRESHOLD_DROP)
            --nextNavLevel;
        nextNavLevel = Util::clamp(nextNavLevel, navigationScores.findMin(), navigationScores.findMax());
        navigation = nextNavLevel;
    }
    
    void calculateSpeed()
    {
        int nmin;
        int nmax;
        int noptimal;
        
        speedScores.setMinOptMax(nmin, noptimal, nmax);
        
        std::cout << "New Max: " << nmax << std::endl;
        std::cout << "New Opt: " << noptimal << std::endl;
        std::cout << "New Min: " << nmin << std::endl;
        
        minSpeed = nmin;
        averageSpeed = noptimal;
        maxSpeed = nmax;
    }

    /*
    void calculateSpeed()
    {
        speedScores.updateAccuracies();
        
        int averageSpeedCheck = speedScores.findBest(averageSpeed, 0.90, 0.10);
        float nmaxSpeed = speedScores.findBest(averageSpeed, 0.90, 0.30);
        float nminSpeed = speedScores.findMin();
        
        std::cout << "Previous: " << maxSpeed << " " << minSpeed << " " << averageSpeed << std::endl;
        std::cout << "SpeedChk: " << averageSpeedCheck << " " << speedScores.findTotalAccuracy() << std::endl;
        
        // If our distribution is bad (below 80% accuracy), revert to minSpeed for a larger range        
        if (speedScores.findTotalAccuracy() <= 0.8 && nminSpeed > minSpeed)
            nminSpeed = minSpeed;
            
        float naverageSpeed = (nmaxSpeed + nminSpeed) / 2;
        // Set average speed to be this value if it is greater
        if (averageSpeedCheck < naverageSpeed)
            naverageSpeed = averageSpeedCheck;
        
        //// The bottom determines whether the previous algorithm failed to determine a decent range for player speed
        // Ensure a backbuffer range to slow down
        int fallbackBuffer = 5;
        if (nminSpeed + fallbackBuffer > naverageSpeed)
            nminSpeed = naverageSpeed - fallbackBuffer;
        
        // Ensure a frontbuffer range to speed down
        int pushfrontBuffer = 5;
        if (nmaxSpeed - pushfrontBuffer < naverageSpeed)
            nmaxSpeed = naverageSpeed + pushfrontBuffer;
        
        minSpeed = nminSpeed;
        maxSpeed = nmaxSpeed;
        averageSpeed = naverageSpeed;
        
        std::cout << "After: " << maxSpeed << " " << minSpeed << " " << averageSpeed << std::endl;
        
        // Never go below a certain speed
        if (averageSpeed <= 5) averageSpeed = 5;
        if (minSpeed <= 5) minSpeed = 5;
        if (maxSpeed <= 5) maxSpeed = 5;
    }
    */
    std::string getCurrentStats() const
    {
        return
        "Color/Sound " + Util::toStringInt(set1) + "\n" +
        "Shape/Sound " + Util::toStringInt(set2) + "\n" +
        " Sound Only " + Util::toStringInt(set3) + "\n" +
        " Navigation " + Util::toStringInt(navigation) + "\n" +
        "  Min Speed " + Util::toStringInt(minSpeed) + "\n" +
        " Init Speed " + Util::toStringInt(averageSpeed) + "\n" +
        "  Max Speed " + Util::toStringInt(maxSpeed);
    }
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
    
    struct Result {
        int stageID;
        int timestamp;
        SectionInfo sectionInfo;
        PodInfo podInfo;
        int nback;
        float speed;
        GameMode gameMode;
    };
    std::vector<Result> results;
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
    
    int previousNumCorrectTotal;
    int previousNumMissedTotal;
    int previousNumCollisionsTotal;
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
    void updateGlowExtraction(float elapsed);
    void setGlowGrabParameters();
    void performShockwave();
    void updateShockwave(float elapsed);
    void performBoost();
    void updateBoost(float elapsed);
    void testPodGiveFeedback(Pod* test);
    void determineSpawnCombo();
	void update(float elapsed);
    
    void calculateNavigationScores();
    void calculateSpeedScores();
    std::string getCurrentStats() const;
    int getPreviousNumCorrectTotal() const;
    int getPreviousNumMissesTotal() const;
    int getPreviousNumCollisionsTotal() const;
    bool saveStage(std::string file);
    bool saveProgress(std::string file, int lastStageID);
    bool loadProgress(std::string savePath);
    
    ~Player();
};

#endif /* defined(__Testing__Player__) */
