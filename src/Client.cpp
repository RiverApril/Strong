#include "Client.hpp"

Client::Client(){

}

void Client::update(){

    while(clientIsConnected){

        printf("a\n");

        PacketSize_t s = 1+sizeof(PacketSize_t);
        unsigned char data1[s];
        int r = Network::reciveData(socket, data1, s);
        if(r == 0){
            serverDisconnected(true);
        }else if(r == -1){
            serverDisconnected(false);
        }else{

            printf("b\n");

            s = 0;

            printf("s[%d] = %u\n", 0, data1[0]);
            for(size_t i=0;i<sizeof(PacketSize_t);i++){
                s |= (data1[i+1] << (8*i));
                printf("s[%zu] = %u\n", i+1, data1[i+1]);
            }
            printf("s = %u\n", s);

            if(s > 0){
                unsigned char data2[s];
                int r = Network::reciveData(socket, data2, s);
                if(r == 0){
                    serverDisconnected(true);
                }else if(r == -1){
                    serverDisconnected(false);
                }else{

                    printf("c\n");

                    processPacket(data1[0], data2);
                }
            }else{
                processPacket(data1[0], nullptr);
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
        }else{
            printf("You must enter an Address\n");
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
        }else{
            printf("You must enter a Username\n");
        }
    }

    connectToServer(address, port, username);
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
    size_t position = 0;
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
            Network::addDataString(data, username);
            break;
        }
    }

    Network::finishPacket(data);
    Network::sendData(socket, data);
}
