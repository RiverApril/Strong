#include "Global.hpp"
#include "Server.hpp"
#include "Client.hpp"
#include "Window.hpp"
#include "Graphics.hpp"

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
            int i = s.find(':');
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

        server->port = port;

        server->setPort(port == -1);

        server->startServer();

        while(server->running){
            server->update();
        }
    }else{
        Graphics::init();
        Window* window = new Window();

        window->client->address = address;
        window->client->port = port;
        window->client->username = username;

        window->client->setValues(address.size() == 0, port == -1, username.size() == 0);

        window->client->connectToServer();

        while(window->running){
            window->update();
        }
        Graphics::cleanup();
    }

    Network::cleanup();

    return 0;
}
