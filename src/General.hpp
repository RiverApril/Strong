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

    void writeData(vector<unsigned char>& data);
    General(World* world, unsigned char* data, size_t& position);

    string name = "";

    World* world;

    vector<Unit*> units;
    vector<Unit*> unitsToAdd;
    vector<Unit*> unitsToRemove;
    
};

#endif /* General_hpp */
