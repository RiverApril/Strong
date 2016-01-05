#ifndef Strong_ClientConnection_hpp
#define Strong_ClientConnection_hpp

#include "Network.hpp"

class Server;

class ClientConnection{
public:

    TCPsocket* hostSocket;

    TCPsocket* socket;
    IPaddress* ip = new IPaddress();

    Server* server;

    SDL_Thread* threadRecive;
    bool connected = true;

    string username = "";

    ClientConnection(TCPsocket* hostSocket, TCPsocket* socket, Server* server);

    void update();

};

int updateThread(void* data);

#include "Server.hpp"

#endif
