//
//  General.cpp
//  Strong
//
//  Created by Braeden Atlee on 1/19/16.
//  Copyright © 2016 Braeden Atlee. All rights reserved.
//

#include "General.hpp"
#include "Network.hpp"
#include "Unit.hpp"

General::General(World* world, string name){
    this->world = world;
    this->name = name;
}

General::~General(){
    
}

void General::update(){
    for(Unit* unit : units){
        unit->update();
    }
    while(unitsToRemove.size() > 0){
        for(size_t i=0;i<units.size();i++){
            if(units[i] == unitsToRemove[0]){
                units.erase(units.begin() + i);
            }
        }
        unitsToRemove.erase(unitsToRemove.begin());
    }
    while (unitsToAdd.size() > 0) {
        units.push_back(unitsToAdd[0]);
        unitsToAdd.erase(unitsToAdd.begin());
    }
}

void General::render(){
    for(Unit* unit : units){
        unit->render();
    }
}

void General::writeData(vector<unsigned char>& data){
    Network::addDataShortString(data, name);
    Network::addDataNumber(data, (size_t)units.size());
    for(Unit* unit : units){
        unit->writeData(data);
    }
}

General::General(World* world, unsigned char* data, size_t& position){
    this->world = world;
    Network::readDataShortString(data, position, name);
    size_t size;
    Network::readDataNumber(data, position, size);
    for(size_t i = 0;i<size;i++){
        unitsToAdd.push_back(new Unit(this, data, position));
    }
}

