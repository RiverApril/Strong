//
//  Client.hpp
//  Strong
//
//  Created by Braeden Atlee on 1/7/16.
//  Copyright © 2016 Braeden Atlee. All rights reserved.
//

#ifndef Client_hpp
#define Client_hpp

#include "Network.hpp"

class World;
class Window;
class General;

class Client{
public:

    bool running = true;
    bool connected = false;

    SDL_Thread* threadRecive;

    TCPsocket* socket = new TCPsocket();
    IPaddress* ip = new IPaddress();

    World* world;
    Window* window;
    General* general;

    Client(Window* window);

    ~Client();

    void update();
    void setValues(bool setIp, bool setPort, bool setUsername);
    void connectToServer();

    void serverDisconnected(bool intentional);
    void processPacket(unsigned char code, unsigned char* data);
    void sendPacket(unsigned char code, void* meta = nullptr);

};

int updateCThread(void* data);

#endif
