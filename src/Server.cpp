#include "Server.hpp"


Server::Server(){

}

void Server::update(){

    //Look for new client
    TCPsocket* clientSocket = new TCPsocket();
    Network::acceptGuest(socket, clientSocket);
    if(*clientSocket){
        ClientConnection* clientConnection = new ClientConnection(socket, clientSocket, this);
        clientList.push_back(clientConnection);
        sendPacket(clientConnection, PACKET_TC_REQUEST_USERNAME);
        printf("Sent username request\n");
    }else{
        delete clientSocket;
    }

    //Update client connections
    for(ClientConnection* cc: clientList){
        cc->update();
    }

    //Remove disconnected clients
    while(clientRemoveList.size() > 0){
        for(int i=0;i<clientList.size();i++){
            if(clientList[i] == clientRemoveList[0]){
                clientList.erase(clientList.begin() + i);
                break;
            }
        }
        clientRemoveList.erase(clientRemoveList.begin());
    }


}

void Server::startServer(){
    while(true){
        printf("Enter host port:\n");
        cin >> port;
        if(port < 0 || port > 65535){
            printf("Port must be between 0 and 65535\n");
        }else{
            break;
        }
    }
    startServer(port);
}

void Server::startServer(int port){
    if(serverIsStarted){
        printf("Server is already started\n");
    }else{
        serverIsStarted = true;
        printf("Starting server on port: %d\n", port);

        bool success = Network::initHost(port, socket, ip);
        if(success){
            printf("Server Started\n");
        }else{
            printf("Failed to start server\n");
            serverIsStarted = false;
            return;
        }
    }
}

void Server::clientDisconnected(ClientConnection* cc, bool intentional){
	SDLNet_TCP_Close(*(cc->socket));
    clientRemoveList.push_back(cc);
    if(intentional){
        printf("Client disconnected\n");
    }else{
        printf("Client lost connection\n");
    }
}

void Server::processPacket(ClientConnection* from, unsigned char code, unsigned char* data){
    printf("Server recived code: %d\n", code);
    size_t position = 0;
    switch (code) {
        case PACKET_TS_USERNAME:{
            Network::readDataString(data, position, from->username);
            printf("Username set: %s\n", from->username.c_str());
            break;
        }
    }
}

void Server::sendPacket(ClientConnection* to, unsigned char code){
    vector<unsigned char> data;
    Network::initPacket(data, code);

    switch(code){
        case PACKET_TC_REQUEST_USERNAME:{
            break;
        }
    }

    Network::finishPacket(data);
    Network::sendData(to->socket, data);
}
