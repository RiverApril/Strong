//
//  World.cpp
//  Strong
//
//  Created by Braeden Atlee on 1/19/16.
//  Copyright © 2016 Braeden Atlee. All rights reserved.
//

#include "World.hpp"
#include "Server.hpp"
#include "Client.hpp"

World::World(Server* server) : server(server){

}

World::World(Client* client) : client(client){

}

World::~World(){

}

void World::update(){
    if(server){
        for(General* general : generals){
            general->update();
        }
        while (generalsToAdd.size() > 0) {
            generals.push_back(generalsToAdd[0]);
            server->sendPacketToAll(PACKET_TC_NEW_GENERAL, generalsToAdd[0]);
            generalsToAdd.erase(generalsToAdd.begin());
        }
    }
}

void World::render(){
    for(General* general : generals){
        general->render();
    }
}

void World::newGeneral(General* general){
    generalsToAdd.push_back(general);
}

void World::writeData(vector<unsigned char>& data){
    Network::addDataNumber(data, (size_t)generals.size());
    for(General* general : generals){
        general->writeData(data);
    }
}

void World::readData(unsigned char* data, size_t& position){
    size_t size;
    Network::readDataNumber(data, position, size);
    for(size_t i = 0;i<size;i++){
        generalsToAdd.push_back(new General(this, data, position));
    }
}

