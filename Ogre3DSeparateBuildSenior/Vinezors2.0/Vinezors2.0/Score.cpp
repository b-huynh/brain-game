//
//  Score.cpp
//  VinezorsiOS
//
//  Created by Calvin Phung on 3/10/14.
//
//

#include "Score.h"

#include <iostream>
#include <cmath>

Score::Score()
    : right(0), wrong(0), accuracy(0.0)
{}

void Score::computeAccuracy()
{
    accuracy = right + wrong > 0 ? static_cast<float>(right) / (right + wrong) : 0.0;
}

int ScoreSet::findMin() const
{
    if (scores.size() <= 0) return 0;
    return scores.begin()->first;
}

int ScoreSet::findMax() const
{
    if (scores.size() <= 0) return 0;
    return scores.rbegin()->first;
}

int ScoreSet::find(int x) const
{
    return scores.find(x) != scores.end() ? x : -1;
}

std::map<int, Score>::const_iterator ScoreSet::findMedian() const
{
    std::map<int, Score>::const_iterator ret = scores.begin();
    std::advance(ret, scores.size() / 2);
    return ret;
}

std::pair<float, float> ScoreSet::calculateLRAccuracies(std::map<int, Score>::const_iterator divider) const
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
        ret.first = 1.00;
    else
        ret.first = static_cast<float>(rights.first) / (rights.first + wrongs.first);
    if (rights.second + wrongs.second == 0)
        ret.second = 0.25;
    else
        ret.second = static_cast<float>(rights.second) / (rights.second + wrongs.second);
    return ret;
}

Score& ScoreSet::operator[](int i)
{ return scores[i]; }
const Score& ScoreSet::at(int i) const
{ return scores.at(i); }

// Old Algorithm
int ScoreSet::findBest(int defaultBest, float Zoffset, float lowLimit) const
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

int ScoreSet::findNumRight() const
{
    int total = 0;
    for (std::map<int, Score>::const_iterator it = scores.begin(); it != scores.end(); ++it)
        total += it->second.right;
    return total;
}

int ScoreSet::findNumWrong() const
{
    int total = 0;
    for (std::map<int, Score>::const_iterator it = scores.begin(); it != scores.end(); ++it)
        total += it->second.wrong;
    return total;
}

void ScoreSet::updateAccuracies()
{
    for (std::map<int, Score>::iterator it = scores.begin(); it != scores.end(); ++it)
        it->second.computeAccuracy();
}

float ScoreSet::findTotalAccuracy() const
{
    int nr = findNumRight();
    int nw = findNumWrong();
    if (nw + nr <= 0)
        return 0.0;
    return static_cast<float>(nr) / (nr + nw);
}

void ScoreSet::setMinOptMax(int & nmin, int & noptimal, int & nmax) const
{
    if (scores.size() <= 0)
        return;
    
    const float desiredLeft = 1.00;
    const float desiredRight = 0.25;
    std::map<int, Score>::const_iterator best = scores.begin();
    std::pair<float, float> accs(0.0, 1.0);
    std::pair<float, float> err = std::pair<float, float>(pow(desiredLeft - accs.first, 2), pow(desiredRight - accs.second, 2));
    
    std::map<int, Score>::const_iterator divider;
    
    // Find the divider
    divider = scores.begin();
    while (1)
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
        if (divider != scores.end())
            divider++;
        else
            break;
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
        previous = divider;
        --divider;
        optimal = divider;
        std::pair<float, float> naccs2 = calculateLRAccuracies(previous);
        std::pair<float, float> naccs1 = calculateLRAccuracies(divider);
        if (naccs1.first - naccs2.first < improvementThreshold)
            break;
    }
    noptimal = optimal->first;
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

void ScoreSet::print() const
{
    std::cout << "Scores: \n";
    for (std::map<int, Score>::const_iterator it = scores.begin(); it != scores.end(); ++it)
    {
        std::cout << it->first << ": " << it->second.right << " " << it->second.wrong << std::endl;
    }
}