#ifndef Strong_Network_hpp
#define Strong_Network_hpp

#include "Global.hpp"
#include <SDL2/SDL_net.h>
#include <SDL2/SDL_thread.h>

//To client from server:
#define PACKET_TC_REQUEST_USERNAME 12

//To server from client:
#define PACKET_TS_USERNAME 34

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


    template<typename T>
    void addDataNumber(vector<unsigned char>& data, T n);
    void addDataUChar(vector<unsigned char>& data, unsigned char n);
    void addDataString(vector<unsigned char>& data, string n);

    template<typename T>
    void readDataNumber(unsigned char* data, size_t& position, T& n);
    void readDataUChar(unsigned char* data, size_t& position, unsigned char& n);
    void readDataString(unsigned char* data, size_t& position, string& n);

}

#endif
