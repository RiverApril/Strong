//
//  main.cpp
//  Strong
//
//  Created by Braeden Atlee on 1/7/16.
//  Copyright © 2016 Braeden Atlee. All rights reserved.
//

#include "Global.hpp"
#include "Server.hpp"
#include "Client.hpp"
#include "Window.hpp"
#include "Graphics.hpp"
#include "Settings.hpp"

int main(int argc, char* argv[]){

    bool runServer = false;

    if(argc >= 2){
        if(strcmp(argv[1], "s") == 0){
            runServer = true;
        }else if(strcmp(argv[1], "c") == 0){
            runServer = false;
        }
    }

    string address = "";
    string username = "";
    int port = -1;

    if(argc >= 3){
        if(runServer){
            port = stoi(argv[2]);
        }else{
            string s = string(argv[2]);
            size_t i = s.find(':');
            address = s.substr(0, i);
            port = stoi(s.substr(i+1));
        }
    }

    if(argc >= 4){
        if(!runServer){
            username = string(argv[3]);
        }
    }

    Network::init();

    if(runServer){
        Server* server = new Server();

        Settings::loadSettings(true);

        if(port != -1){
        	Settings::Server::hostPort = port;
        }

        server->setPort(Settings::Server::hostPort == -1);

        Settings::saveSettings(true);

        server->startServer();

        while(server->running){
            server->update();
        }
    }else{
        Graphics::init();

        Settings::loadSettings(false);

        if(address.size() != 0){
            Settings::Client::connectAddress = address;
        }
        if(port != -1){
            Settings::Client::connectPort = port;
        }
        if(username.size() != 0){
            Settings::Client::connectUsername = username;
        }
        
        
        Window* window = new Window();

        //window->client->setValues(Settings::Client::connectAddress.size() == 0, Settings::Client::connectPort == -1, Settings::Client::connectUsername.size() == 0);

        Settings::saveSettings(false);

        //window->client->connectToServer();

        while(window->running){
            window->update();
        }
        Graphics::cleanup();
    }

    Network::cleanup();

    return 0;
}
