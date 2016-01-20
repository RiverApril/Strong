//
//  MenuGame.cpp
//  Strong
//
//  Created by Braeden Atlee on 1/19/16.
//  Copyright © 2016 Braeden Atlee. All rights reserved.
//

#include "MenuGame.hpp"
#include "World.hpp"

MenuGame::MenuGame(Menu* upperMenu) : Menu(upperMenu->window){
    this->upperMenu = upperMenu;
    
}

MenuGame::~MenuGame(){

}

void MenuGame::openMenu(){
    window->client->sendPacket(PACKET_TS_REQUEST_ALL_WORLD_DATA);
}

void MenuGame::closeMenu(){
    window->changeMenu(upperMenu, true);
}

void MenuGame::update(){
    //updates world in Client
}

void MenuGame::render(){
    window->client->world->render();
}
