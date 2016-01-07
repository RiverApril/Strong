#ifndef Strong_Client_hpp
#define Strong_Client_hpp

#include "Network.hpp"

class Client{
public:

    bool running = true;
    bool connected = false;

    string address;
    int port;

    string username;

    SDL_Thread* threadRecive;

    TCPsocket* socket = new TCPsocket();
    IPaddress* ip = new IPaddress();

    Client();

    ~Client(){}

    void update();
    void setValues(bool setIp, bool setPort, bool setUsername);
    void connectToServer();

    void serverDisconnected(bool intentional);
    void processPacket(unsigned char code, unsigned char* data);
    void sendPacket(unsigned char code);

};

int updateCThread(void* data);

#endif
