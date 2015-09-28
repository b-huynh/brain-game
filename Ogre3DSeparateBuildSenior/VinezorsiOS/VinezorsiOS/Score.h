//
//  Score.h
//  VinezorsiOS
//
//  Created by Calvin Phung on 3/10/14.
//
//

#ifndef __VinezorsiOS__Score__
#define __VinezorsiOS__Score__

#include <map>

struct Score
{
    int right;
    int wrong;
    float accuracy;
    
    void computeAccuracy();
    
    Score();
};
struct ScoreSet
{
    std::map<int, Score> scores;
    
    int findMin() const;
    int findMax() const;
    int find(int x) const;
    std::map<int, Score>::const_iterator findMedian() const;
    
    std::pair<float, float> calculateLRAccuracies(std::map<int, Score>::const_iterator divider) const;
    
    Score& operator[](int i);
    const Score& at(int i) const;
    
    // Old Algorithm
    int findBest(int defaultBest, float Zoffset, float lowLimit) const;
    
    int findNumRight() const;
    int findNumWrong() const;
    void updateAccuracies();
    float findTotalAccuracy() const;
    void setMinOptMax(int & nmin, int & noptimal, int & nmax) const;
    void print() const;
};

#endif /* defined(__VinezorsiOS__Score__) */
