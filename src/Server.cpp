//
//  Server.cpp
//  Strong
//
//  Created by Braeden Atlee on 1/7/16.
//  Copyright © 2016 Braeden Atlee. All rights reserved.
//

#include "Server.hpp"
#include "Settings.hpp"
#include "Debug.hpp"
#include "Unit.hpp"


Server::Server(){

    world = new World(this);

}

void Server::update(){

    //Look for new client
    TCPsocket* clientSocket = new TCPsocket();
    Network::acceptGuest(socket, clientSocket);
    if(*clientSocket){
        ClientConnection* clientConnection = new ClientConnection(socket, clientSocket, this);
        clientList.push_back(clientConnection);
        debugf("Sending username request...");
        sendPacket(clientConnection, PACKET_TC_REQUEST_CLIENT_INFO);
    }else{
        delete clientSocket;
    }

    //Update client connections
    for(ClientConnection* cc: clientList){
        cc->update();
    }

    //Remove disconnected clients
    while(clientRemoveList.size() > 0){
        for(size_t i=0;i<clientList.size();i++){
            if(clientList[i] == clientRemoveList[0]){
                clientList.erase(clientList.begin() + i);
                break;
            }
        }
        clientRemoveList.erase(clientRemoveList.begin());
    }

    currentTime = SDL_GetTicks();

    if(serverIsStarted){
        while(previousTime + TIME_PER_UPDATE < currentTime){
            previousTime += TIME_PER_UPDATE;
    		world->update();
            tick++;
        }
    }


}

/*void Server::setPort(bool setPort){
    while(setPort){
        debugf("Enter host port:");
        cin >> Settings::Server::hostPort;
        if(Settings::Server::hostPort < 0 || Settings::Server::hostPort > 65535){
            debugf("Port must be between 0 and 65535");
        }else{
            break;
        }
    }
}*/

void Server::startServer(){
    if(serverIsStarted){
        debugf("Server is already started");
    }else{
        serverIsStarted = true;
        debugf("Starting server on port: %d", Settings::Server::hostPort);

        bool success = Network::initHost(Settings::Server::hostPort, socket, ip);
        if(success){
            debugf("Server Started");
        }else{
            debugf("Failed to start server");
            serverIsStarted = false;
            return;
        }
    }
}

void Server::clientDisconnected(ClientConnection* cc, bool intentional){
	SDLNet_TCP_Close(*(cc->socket));
    clientRemoveList.push_back(cc);
    if(intentional){
        debugf("Client disconnected");
    }else{
        debugf("Client lost connection");
    }
}

void Server::processPacket(ClientConnection* from, unsigned char code, unsigned char* data){
    //debugf("Server recived code: %d", code);

    size_t position = 0;
    switch (code) {
        case PACKET_TS_CLIENT_INFO:{
            if(data){
                Network::readDataShortString(data, position, from->username);
                debugf("Username set: %s", from->username.c_str());
                from->general = new General(world, from->username);
                world->newGeneral(from->general);
                sendPacketToAllExcept(from, PACKET_TC_NEW_GENERAL, from->general);
                sendPacket(from, PACKET_TC_YOUR_NEW_GENERAL, from->general);
            }else{
                debugf("data is nullptr");
            }
            break;
        }
        case PACKET_TS_REQUEST_ALL_WORLD_DATA:{
            sendPacket(from, PACKET_TC_ALL_WORLD_DATA);
            break;
        }
        case PACKET_TS_NEW_UNIT:{
            from->general->newUnit(new Unit(from->general, data, position));
            break;
        }
        case PACKET_TS_UNIT_TARGET_SET:{
            UID uid;
            Network::readDataNumber(data, position, uid);
            from->general->units[uid]->readTargetData(data, position);
            sendPacketToAll(PACKET_TC_UNIT_TARGET_SET, from->general->units[uid]);
            break;
        }
        case PACKET_TS_UNIT_TARGET_REACHED:{
            UID uid;
            Network::readDataNumber(data, position, uid);
            from->general->units[uid]->readPosData(data, position);
            sendPacketToAll(PACKET_TC_UNIT_TARGET_REACHED, from->general->units[uid]);
            break;
        }
        default:{
            debugf("Server recived packet it shoudlen't have: %d", code);
            break;
        }
    }
}

void Server::sendPacketToAll(unsigned char code, void* meta){
    sendPacket(nullptr, code, meta);
}

void Server::sendPacketToAllExcept(ClientConnection* but, unsigned char code, void* meta){
    sendPacket(but, code, meta, true);
}

void Server::sendPacket(ClientConnection* to, unsigned char code, void* meta, bool allBut){

    vector<unsigned char> data;
    Network::initPacket(data, code);

    switch(code){
        case PACKET_TC_REQUEST_CLIENT_INFO:{
            break;
        }
        case PACKET_TC_ALL_WORLD_DATA:{
            world->writeData(data);
            break;
        }
        case PACKET_TC_NEW_GENERAL:{
            ((General*)meta)->writeAllData(data);
            break;
        }
        case PACKET_TC_YOUR_NEW_GENERAL:{
            ((General*)meta)->writeAllData(data);
            break;
        }
        case PACKET_TC_NEW_UNIT:{
            Network::addDataNumber(data, ((Unit*)meta)->general->uid);
            ((Unit*)meta)->writeAllData(data);
            break;
        }
        case PACKET_TC_UNIT_TARGET_SET:{
            Network::addDataNumber(data, ((Unit*)meta)->general->uid);
            ((Unit*)meta)->writeTargetData(data);
            break;
        }
        case PACKET_TC_UNIT_TARGET_REACHED:{
            Network::addDataNumber(data, ((Unit*)meta)->general->uid);
            ((Unit*)meta)->writePosData(data);
            break;
        }
        default:{
            debugf("Server sending packet it shoudlen't: %d", code);
            break;
        }
    }

    Network::finishPacket(data);
    if(to && !allBut){
        while(to->sendLock);
        to->sendLock = true;
        Network::sendData(to->socket, data);
        to->sendLock = false;
    }else{
        for(ClientConnection* cc : clientList){
            if(allBut && cc == to){
                continue;
            }
            while(cc->sendLock);
            cc->sendLock = true;
            Network::sendData(cc->socket, data);
            cc->sendLock = false;
        }
    }

}
