//
//  Server.cpp
//  Strong
//
//  Created by Braeden Atlee on 1/7/16.
//  Copyright © 2016 Braeden Atlee. All rights reserved.
//

#include "Server.hpp"
#include "Settings.hpp"


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
        printf("Sending username request...\n");
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

void Server::setPort(bool setPort){
    while(setPort){
        printf("Enter host port:\n");
        cin >> Settings::Server::hostPort;
        if(Settings::Server::hostPort < 0 || Settings::Server::hostPort > 65535){
            printf("Port must be between 0 and 65535\n");
        }else{
            break;
        }
    }
}

void Server::startServer(){
    if(serverIsStarted){
        printf("Server is already started\n");
    }else{
        serverIsStarted = true;
        printf("Starting server on port: %d\n", Settings::Server::hostPort);

        bool success = Network::initHost(Settings::Server::hostPort, socket, ip);
        if(success){
            printf("Server Started\n");
        }else{
            printf("Failed to start server\n");
            serverIsStarted = false;
            return;
        }
    }
}

void Server::clientDisconnected(ClientConnection* cc, bool intentional){
	SDLNet_TCP_Close(*(cc->socket));
    clientRemoveList.push_back(cc);
    if(intentional){
        printf("Client disconnected\n");
    }else{
        printf("Client lost connection\n");
    }
}

void Server::processPacket(ClientConnection* from, unsigned char code, unsigned char* data){
    printf("Server recived code: %d\n", code);

    size_t position = 0;
    switch (code) {
        case PACKET_TS_CLIENT_INFO:{
            if(data){
                Network::readDataShortString(data, position, from->username);
                printf("Username set: %s\n", from->username.c_str());
                from->general = new General(world, from->username);
                world->newGeneral(from->general);
            }else{
                printf("data is nullptr\n");
            }
            break;
        }
        case PACKET_TS_REQUEST_ALL_WORLD_DATA:{
            sendPacket(from, PACKET_TC_ALL_WORLD_DATA);
            break;
        }
    }
}

void Server::sendPacketToAll(unsigned char code, void* meta){
    sendPacket(nullptr, code, meta);
}

void Server::sendPacket(ClientConnection* to, unsigned char code, void* meta){

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
            ((General*)meta)->writeData(data);
            break;
        }
    }

    Network::finishPacket(data);
    if(to){
        while(to->sendLock);
        to->sendLock = true;
        Network::sendData(to->socket, data);
        to->sendLock = false;
    }else{
        for(ClientConnection* cc : clientList){
            while(cc->sendLock);
            cc->sendLock = true;
            Network::sendData(cc->socket, data);
            cc->sendLock = false;
        }
    }

}
