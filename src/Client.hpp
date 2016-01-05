#ifndef Strong_Client_hpp
#define Strong_Client_hpp

#include "Network.hpp"

class Client{
public:
    
    bool running = true;
    bool clientIsConnected = false;
    
    string address;
    int port;
    
    string username;
    
    TCPsocket* socket = new TCPsocket();
    IPaddress* ip = new IPaddress();
    
    Client();
    
    ~Client(){}
    
    void update();
    void connectToServer();
    void connectToServer(string address, int port, string username);
    
    void serverDisconnected(bool intentional);
    void processPacket(unsigned char code, unsigned char* data);
    
};

#endif
