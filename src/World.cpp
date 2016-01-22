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
#include "Debug.hpp"
#include "Window.hpp"

World::World(Server* server) : server(server){

}

World::World(Client* client) : client(client){

}

World::~World(){

}

void World::update(){
    for(map<UID, General*>::iterator i=generals.begin(); i!=generals.end(); ++i){
        i->second->update();
    }
    while (generalsToAdd.size() > 0) {
        generals.insert(pair<UID, General*>(generalsToAdd[0]->uid, generalsToAdd[0]));
        generalsToAdd.erase(generalsToAdd.begin());
    }
}

void World::render(){

    SDL_SetRenderDrawColor(client->window->sdlRenderer, 40, 170, 30, 255);
    SDL_RenderFillRect(client->window->sdlRenderer, &client->window->windowRect);

    for(map<UID, General*>::iterator i=generals.begin(); i!=generals.end(); ++i){
        i->second->render();
    }
}

void World::newGeneral(General* general){
    if(server){
        general->uid = nextUid++;
        debugf("General UID set");
    }
    generalsToAdd.push_back(general);
    debugf("New General: \"%s\" [%u]", general->name.c_str(), general->uid);
}

void World::writeData(vector<unsigned char>& data){
    Network::addDataNumber(data, (size_t)generals.size());
    for(map<UID, General*>::iterator i=generals.begin(); i!=generals.end(); ++i){
        i->second->writeAllData(data);
    }
}

void World::readData(unsigned char* data, size_t& position){
    size_t size;
    Network::readDataNumber(data, position, size);
    for(size_t i = 0;i<size;i++){
        generalsToAdd.push_back(new General(this, data, position));
    }
}

