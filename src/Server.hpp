//
//  Server.hpp
//  Strong
//
//  Created by Braeden Atlee on 1/7/16.
//  Copyright © 2016 Braeden Atlee. All rights reserved.
//

#ifndef Server_hpp
#define Server_hpp

#include "Network.hpp"
#include "ClientConnection.hpp"
#include "World.hpp"

class Server{
public:

    bool running = true;
    bool serverIsStarted = false;

    TCPsocket* socket = new TCPsocket();
    IPaddress* ip = new IPaddress();

    vector<ClientConnection*> clientList;
    vector<ClientConnection*> clientRemoveList;

    World* world;

    Uint32 currentTime = 0;
    Uint32 previousTime = 0;

    unsigned long tick = 0;

    Server();

    ~Server(){}

    void update();
    void startServer();
    void setPort(bool setPort);

    void clientDisconnected(ClientConnection* cc, bool intentional);

    void processPacket(ClientConnection* from, unsigned char code, unsigned char* data);
    void sendPacket(ClientConnection* to, unsigned char code, void* meta = nullptr);
    void sendPacketToAll(unsigned char code, void* meta = nullptr);

};

#endif
