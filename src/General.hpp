//
//  General.hpp
//  Strong
//
//  Created by Braeden Atlee on 1/19/16.
//  Copyright © 2016 Braeden Atlee. All rights reserved.
//

#ifndef General_hpp
#define General_hpp

#include "World.hpp"

class Unit;

class General{
public:

    General(World* world, string name);

    ~General();

    void update();
    void render();

    UID nextUid = 1;

    UID uid = 0;

    void writeAllData(vector<unsigned char>& data);
    void readAllData(unsigned char* data, size_t& position);
    General(World* world, unsigned char* data, size_t& position);

    void newUnit(Unit* unit);

    string name = "";

    World* world;

    map<UID, Unit*> units;
    vector<Unit*> unitsToAdd;
    vector<Unit*> unitsToRemove;
    
};

#endif /* General_hpp */
