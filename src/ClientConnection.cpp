#include "ClientConnection.hpp"


ClientConnection::ClientConnection(TCPsocket* hostSocket, TCPsocket* socket, Server* server){
    this->hostSocket = hostSocket;
    this->socket = socket;
    this->server = server;
    
    if((ip = SDLNet_TCP_GetPeerAddress(*socket))){
        uint32_t n = SDLNet_Read32(&ip->host);
        string address;
        address += to_string((n & 0xFF000000) >> 24);
        address += ".";
        address += to_string((n & 0x00FF0000) >> 16);
        address += ".";
        address += to_string((n & 0x0000FF00) >> 8);
        address += ".";
        address += to_string((n & 0x000000FF));
        printf("Clint connected through address: %s:%d\n", address.c_str(), SDLNet_Read16(&ip->port));
	}else{
		fprintf(stderr, "SDLNet_TCP_GetPeerAddress: %s\n", SDLNet_GetError());
    }
    
    threadRecive = SDL_CreateThread(updateThread, NULL, this);
    
}

void ClientConnection::update(Server server){
}

int updateThread(void* data){
    ClientConnection* self = (ClientConnection*)data;
    while(self->connected){
        size_t s = 1+sizeof(size_t);
        unsigned char data1[s];
        int r = Network::reciveData(self->socket, data1, s);
        if(r == 0){
            self->connected = false;
            self->server->clientDisconnected(self, true);
        }else if(r == -1){
            self->connected = false;
            self->server->clientDisconnected(self, false);
        }else{
            s = data1[1] & (data1[2] << 1) & (data1[3] << 2) & (data1[4] << 3);
            unsigned char data2[s];
            int r = Network::reciveData(self->socket, data2, s);
            if(r == 0){
                self->connected = false;
                self->server->clientDisconnected(self, true);
            }else if(r == -1){
                self->connected = false;
                self->server->clientDisconnected(self, false);
            }else{
                self->server->processPacket(self, data1[0], data2);
            }
        }
    }
    return 0;
    
}
