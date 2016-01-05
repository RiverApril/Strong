#include "Client.hpp"

Client::Client(){
    
}

void Client::update(){
    
    while(clientIsConnected){
        
        size_t s = 1+sizeof(size_t);
        unsigned char data1[s];
        int r = Network::reciveData(socket, data1, s);
        if(r == 0){
            serverDisconnected(true);
        }else if(r == -1){
            serverDisconnected(false);
        }else{
            s = data1[1] & (data1[2] << 1) & (data1[3] << 2) & (data1[4] << 3);
            unsigned char data2[s];
            int r = Network::reciveData(socket, data2, s);
            if(r == 0){
                serverDisconnected(true);
            }else if(r == -1){
                serverDisconnected(false);
            }else{
                processPacket(data1[0], data2);
            }
        }
        
    }
    
}

void Client::connectToServer(){
    while(true){
        printf("Enter IP address:\n");
        cin >> address;
        if(address.size() > 0){
            break;
        }
    }
    while(true){
        printf("Enter port:\n");
        cin >> port;
        if(port < 0 || port > 65535){
            printf("Port must be between 0 and 65535\n");
        }else{
            break;
        }
    }
    while(true){
        printf("Enter Unsername:\n");
        cin >> username;
        if(username.size() > 0){
            break;
        }
    }
}

void Client::connectToServer(string address, int port, string username){
    if(clientIsConnected){
        printf("Client is already connected\n");
    }else{
        clientIsConnected = true;
        printf("Connecting to server: %s:%d\n", address.c_str(), port);
        
        bool success = Network::connectToHost(address, port, socket, ip);
        if(success){
            printf("Connected to server\n");
        }else{
            printf("Failed to connect to server\n");
            clientIsConnected = false;
            return;
        }
    }
}

void Client::serverDisconnected(bool intentional){
	SDLNet_TCP_Close(*socket);
    clientIsConnected = false;
    if(intentional){
        printf("Disconnected from server\n");
    }else{
        printf("Lost connection to server\n");
    }
}

void Client::processPacket(unsigned char code, unsigned char* data){
    printf("Client recived code: %d\n", code);
    unsigned char position = 0;
    switch(code){
        case PACKET_TC_REQUEST_USERNAME:{
            vector<unsigned char> data;
            Network::initPacket(data, PACKET_TS_USERNAME);
            
            Network::addDataString(data, username);
            
            Network::finishPacket(data);
            Network::sendData(socket, data);
            break;
        }
    }
}
