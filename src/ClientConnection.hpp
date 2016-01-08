//
//  ClientConnection.hpp
//  Strong
//
//  Created by Braeden Atlee on 1/7/16.
//  Copyright © 2016 Braeden Atlee. All rights reserved.
//

#ifndef ClientConnection_hpp
#define ClientConnection_hpp

#include "Network.hpp"

class Server;

class ClientConnection{
public:

    TCPsocket* hostSocket;

    TCPsocket* socket;
    IPaddress* ip = new IPaddress();

    Server* server;

    bool sendLock = false;

    SDL_Thread* threadRecive;
    bool connected = true;

    string username = "";

    ClientConnection(TCPsocket* hostSocket, TCPsocket* socket, Server* server);

    void update();

};

int updateCCThread(void* data);

#include "Server.hpp"

#endif
