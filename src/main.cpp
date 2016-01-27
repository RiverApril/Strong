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
#include "Math.hpp"

int main(int argc, char* argv[]){

    bool runServer = false;
    bool runClient = false;

    if(argc >= 2){
        if(strcmp(argv[1], "s") == 0){
            runServer = true;
        }else if(strcmp(argv[1], "c") == 0){
            runClient = true;
        }else if(strcmp(argv[1], "h") == 0){
            runServer = true;
            runClient = true;
        }
    }

    string address = "";
    string username = "";
    int port = -1;

    if(argc >= 3){
        if(runClient){
            string s = string(argv[2]);
            size_t i = s.find(':');
            address = s.substr(0, i);
            port = stoi(s.substr(i+1));
        }else if(runServer){
            port = stoi(argv[2]);
        }
    }

    if(argc >= 4){
        if(!runClient){
            username = string(argv[3]);
        }
    }

    Network::init();

    Server* server = nullptr;
    Window* window = nullptr;

    if(runServer){
        server = new Server();

        Settings::loadSettings(true);

        if(port != -1){
            Settings::Server::hostPort = port;
        }

        Settings::saveSettings(true);

        server->startServer();

    }

    if(runClient){
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

        Settings::saveSettings(false);

        window = new Window();

    }

    while((!server || server->running) && (!window || window->running)){
        if(server){
            server->update();
        }
        if(window){
            window->update();
        }
    }

    if(runClient){
        Graphics::cleanup();
    }

    Network::cleanup();

    return 0;
}
