//
//  World.hpp
//  Strong
//
//  Created by Braeden Atlee on 1/19/16.
//  Copyright © 2016 Braeden Atlee. All rights reserved.
//

#ifndef World_hpp
#define World_hpp

#include "Global.hpp"
class General;
class Server;
class Client;

class World{
public:

    Server* server = nullptr;
    Client* client = nullptr;

    World(Server* server);
    World(Client* client);

    ~World();

    void update();
    void render();

    void writeData(vector<unsigned char>& data);
    void readData(unsigned char* data, size_t& position);

    void newGeneral(General* general);

    vector<General*> generalsToAdd;
    vector<General*> generals;

};

#include "General.hpp"

#endif /* World_hpp */
