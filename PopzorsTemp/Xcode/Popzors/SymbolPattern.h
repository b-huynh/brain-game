#pragma once

#include "Pch.h"
#include "Poppy.h"
#include <vector>

class SymbolPattern
{
public:
    SymbolPattern();
    
    void loadPattern(const vector<Poppy> & poppies);
    
private:
    vector<int> poppyOrderList;
    int playerPoppyID;
    bool playerIsRight;
};
