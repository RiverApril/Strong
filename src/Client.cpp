#include "Client.hpp"

Client::Client(){
    threadRecive = SDL_CreateThread(updateCThread, NULL, this);
}

int updateCThread(void* data){
    Client* self = (Client*)data;

    printf("update thread\n");

    while(self->connected){

        printf("about to try to recive packet\n");

        Network::recivePacket(self->socket, [self](int r, unsigned char code, unsigned char* data){
            if(r == 0){
                self->serverDisconnected(true);
            }else if(r == -1){
                self->serverDisconnected(false);
            }else{
                self->processPacket(code, data);
            }
        });

    }

    return 0;
}

void Client::update(){

}

void Client::setValues(bool setIp, bool setPort, bool setUsername){
    while(setIp){
        printf("Enter IP address:\n");
        cin >> address;
        if(address.size() > 0){
            setIp = false;
        }else{
            printf("You must enter an Address\n");
        }
    }
    while(setPort){
        printf("Enter port:\n");
        cin >> port;
        if(port < 0 || port > 65535){
            printf("Port must be between 0 and 65535\n");
        }else{
            setPort = false;
        }
    }
    while(setUsername){
        printf("Enter Unsername:\n");
        cin >> username;
        if(username.size() > 0){
            setUsername = false;
        }else{
            printf("You must enter a Username\n");
        }
    }
}

void Client::connectToServer(){

    if(connected){
        printf("Client is already connected\n");
    }else{
        connected = true;
        printf("Connecting to server: %s:%d as \"%s\"\n", address.c_str(), port, username.c_str());

        bool success = Network::connectToHost(address, port, socket, ip);
        if(success){
            printf("Connected to server\n");
        }else{
            printf("Failed to connect to server\n");
            connected = false;
            return;
        }
    }
}

void Client::serverDisconnected(bool intentional){
	SDLNet_TCP_Close(*socket);
    connected = false;
    if(intentional){
        printf("Disconnected from server\n");
    }else{
        printf("Lost connection to server\n");
    }
}

void Client::processPacket(unsigned char code, unsigned char* data){
    printf("Client recived code: %d\n", code);
    //size_t position = 0;
    switch(code){
        case PACKET_TC_REQUEST_USERNAME:{
            sendPacket(PACKET_TS_USERNAME);
            break;
        }
    }
}

void Client::sendPacket(unsigned char code){
    vector<unsigned char> data;
    Network::initPacket(data, code);

    switch(code){
        case PACKET_TS_USERNAME:{
            Network::addDataShortString(data, username);
            break;
        }
    }

    Network::finishPacket(data);
    Network::sendData(socket, data);
}
