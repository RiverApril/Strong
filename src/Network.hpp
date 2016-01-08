//
//  Network.hpp
//  Strong
//
//  Created by Braeden Atlee on 1/7/16.
//  Copyright © 2016 Braeden Atlee. All rights reserved.
//

#ifndef Network_hpp
#define Network_hpp

#include "Global.hpp"
#include <SDL2/SDL_net.h>
#include <SDL2/SDL_thread.h>

//To client from server:
#define PACKET_TC_REQUEST_USERNAME 0

//To server from client:
#define PACKET_TS_USERNAME 0

typedef unsigned short PacketSize_t;

namespace Network{

    void init();
    void cleanup();

    bool initHost(int port, TCPsocket* socket, IPaddress* ip);
    bool acceptGuest(TCPsocket* host, TCPsocket* client);

    bool connectToHost(string address, int port, TCPsocket* socket, IPaddress* ip);

    bool sendData(TCPsocket* socket, vector<unsigned char>& data);

    void initPacket(vector<unsigned char>& data, unsigned char code);
    void finishPacket(vector<unsigned char>& data);

    int reciveData(TCPsocket* socket, unsigned char* data, PacketSize_t dataSize);
    void recivePacket(TCPsocket* socket, function<void(int, unsigned char, unsigned char*)> process);

    template<typename T>
    void addDataNumber(vector<unsigned char>& data, T n);
    void addDataUChar(vector<unsigned char>& data, unsigned char n);
    void addDataShortString(vector<unsigned char>& data, string n);

    template<typename T>
    void readDataNumber(unsigned char* data, size_t& position, T& n);
    void readDataUChar(unsigned char* data, size_t& position, unsigned char& n);
    void readDataShortString(unsigned char* data, size_t& position, string& n);

}

#endif
