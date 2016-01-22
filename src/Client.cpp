//
//  Client.cpp
//  Strong
//
//  Created by Braeden Atlee on 1/7/16.
//  Copyright © 2016 Braeden Atlee. All rights reserved.
//

#include "Client.hpp"
#include "Settings.hpp"
#include "World.hpp"
#include "Window.hpp"
#include "Debug.hpp"
#include "Unit.hpp"

Client::Client(Window* window){
    this->window = window;
    world = new World(this);
}

Client::~Client(){
    if(connected){
        serverDisconnected(true);
    }
}

int updateCThread(void* data){
    Client* self = (Client*)data;

    while(self->connected){
        Network::recivePacket(self->socket, self);
    }

    return 0;
}

void Client::preProcessPacket(int r, unsigned char code, unsigned char* data){
    if(r == 0){
        serverDisconnected(true);
    }else if(r == -1){
        serverDisconnected(false);
    }else{
        processPacket(code, data);
    }
}

void Client::update(){
    world->update();
}

/*void Client::setValues(bool setIp, bool setPort, bool setUsername){
    while(setIp){
        debugf("Enter IP address:");
        cin >> Settings::Client::connectAddress;
        if(Settings::Client::connectAddress.size() > 0){
            setIp = false;
        }else{
            debugf("You must enter an Address");
        }
    }
    while(setPort){
        debugf("Enter port:");
        cin >> Settings::Client::connectPort;
        if(Settings::Client::connectPort < 0 || Settings::Client::connectPort > 65535){
            debugf("Port must be between 0 and 65535");
        }else{
            setPort = false;
        }
    }
    while(setUsername){
        debugf("Enter Unsername:");
        cin >> Settings::Client::connectUsername;
        if(Settings::Client::connectUsername.size() > 0){
            setUsername = false;
        }else{
            debugf("You must enter a Username");
        }
    }
}*/

void Client::connectToServer(){

    if(connected){
        debugf("Client is already connected");
    }else{
        connected = true;
        debugf("Connecting to server: %s:%d as \"%s\"", Settings::Client::connectAddress.c_str(), Settings::Client::connectPort, Settings::Client::connectUsername.c_str());

        bool success = Network::connectToHost(Settings::Client::connectAddress, Settings::Client::connectPort, socket, ip);
        if(success){
            debugf("Connected to server");
            threadRecive = SDL_CreateThread(updateCThread, NULL, this);
        }else{
            debugf("Failed to connect to server");
            connected = false;
            return;
        }
    }
}

void Client::serverDisconnected(bool intentional){
	SDLNet_TCP_Close(*socket);
    connected = false;
    if(intentional){
        debugf("Disconnected from server");
    }else{
        debugf("Lost connection to server");
    }
}

void Client::processPacket(unsigned char code, unsigned char* data){
    //debugf("Client recived code: %d", code);
    size_t position = 0;
    switch(code){
        case PACKET_TC_REQUEST_CLIENT_INFO:{
            sendPacket(PACKET_TS_CLIENT_INFO);
            break;
        }
        case PACKET_TC_ALL_WORLD_DATA:{
            world->readData(data, position);
            break;
        }
        case PACKET_TC_NEW_GENERAL:{
            General* g = new General(world, data, position);
            world->newGeneral(g);
            break;
        }
        case PACKET_TC_YOUR_NEW_GENERAL:{
            general = new General(world, data, position);
            world->newGeneral(general);
            break;
        }
        case PACKET_TC_NEW_UNIT:{
            UID uid;
            Network::readDataNumber(data, position, uid);
            General* general = world->generals[uid];
            general->newUnit(new Unit(general, data, position));
            break;
        }
        case PACKET_TC_UNIT_TARGET_SET:{
            UID Guid, Uuid;
            Network::readDataNumber(data, position, Guid);
            Network::readDataNumber(data, position, Uuid);
            world->generals[Guid]->units[Uuid]->readTargetData(data, position);
            break;
        }
        case PACKET_TC_UNIT_TARGET_REACHED:{
            UID Guid, Uuid;
            Network::readDataNumber(data, position, Guid);
            Network::readDataNumber(data, position, Uuid);
            world->generals[Guid]->units[Uuid]->readPosData(data, position);
            break;
        }
        default:{
            debugf("Client recived packet it shoudlen't have: %d", code);
            break;
        }
    }
}

void Client::sendPacket(unsigned char code, void* meta){
    vector<unsigned char> data;
    Network::initPacket(data, code);

    switch(code){
        case PACKET_TS_CLIENT_INFO:{
            Network::addDataShortString(data, Settings::Client::connectUsername);
            break;
        }
        case PACKET_TS_REQUEST_ALL_WORLD_DATA:{
            break;
        }
        case PACKET_TS_NEW_UNIT:{
            ((Unit*)meta)->writeAllData(data);
            break;
        }
        case PACKET_TS_UNIT_TARGET_SET:{
            ((Unit*)meta)->writeTargetData(data);
            break;
        }
        case PACKET_TS_UNIT_TARGET_REACHED:{
            ((Unit*)meta)->writePosData(data);
            break;
        }
        default:{
            debugf("Client sending packet it shoudlen't: %d", code);
            break;
        }
    }

    Network::finishPacket(data);
    Network::sendData(socket, data);
}
