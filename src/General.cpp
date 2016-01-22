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
#include "Server.hpp"
#include "Debug.hpp"

General::General(World* world, string name){
    this->world = world;
    this->name = name;
}

General::~General(){
    
}

void General::update(){
    for(map<UID, Unit*>::iterator i=units.begin(); i!=units.end(); ++i){
        i->second->update();
    }
    while(unitsToRemove.size() > 0){
        units.erase(unitsToRemove[0]);
        unitsToRemove.erase(unitsToRemove.begin());
    }

    while (unitsToAdd.size() > 0) {
        units.insert(pair<UID, Unit*>(unitsToAdd[0]->uid, unitsToAdd[0]));
        if(world->server){
            world->server->sendPacketToAll(PACKET_TC_NEW_UNIT, unitsToAdd[0]);
        }
        unitsToAdd.erase(unitsToAdd.begin());
    }

}

void General::newUnit(Unit* unit){
    if(world->server){
        unit->uid = nextUid++;
        debugf("Unit UID set");
    }
    if(unit->uid == 0){
       debugf("WARNING! UID is 0!");
    }
    unitsToAdd.push_back(unit);
    debugf("New Unit [%u]", unit->uid);
}

void General::render(){
    for(map<UID, Unit*>::iterator i=units.begin(); i!=units.end(); ++i){
        i->second->render();
    }
}

void General::writeAllData(vector<unsigned char>& data){
    Network::addDataNumber(data, uid);
    Network::addDataShortString(data, name);
    Network::addDataNumber(data, (size_t)units.size());
    for(map<UID, Unit*>::iterator i=units.begin(); i!=units.end(); ++i){
        i->second->writeAllData(data);
    }
}

void General::readAllData(unsigned char* data, size_t& position){
    Network::readDataNumber(data, position, uid);
    Network::readDataShortString(data, position, name);
    size_t size;
    Network::readDataNumber(data, position, size);
    for(size_t i = 0;i<size;i++){
        unitsToAdd.push_back(new Unit(this, data, position));
    }
}

General::General(World* world, unsigned char* data, size_t& position){
    this->world = world;
    readAllData(data, position);
}

