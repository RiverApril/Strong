//
//  MenuGame.cpp
//  Strong
//
//  Created by Braeden Atlee on 1/19/16.
//  Copyright © 2016 Braeden Atlee. All rights reserved.
//

#include "MenuGame.hpp"
#include "World.hpp"
#include "Debug.hpp"
#include "Unit.hpp"

#define general (window->client->general)

MenuGame::MenuGame(Menu* upperMenu) : Menu(upperMenu->window){
    this->upperMenu = upperMenu;
    
}

MenuGame::~MenuGame(){

}

void MenuGame::openMenu(){
    window->client->sendPacket(PACKET_TS_REQUEST_ALL_WORLD_DATA);
    Menu::openMenu();
}

void MenuGame::closeMenu(){
    window->changeMenu(upperMenu, true);
    Menu::closeMenu();
}

bool MenuGame::keyPressed(SDL_Keysym key){
    if(key.sym == SDLK_1){
        debugf("1 pressed, adding unit...");
        Unit* unit = new Unit(general);
        unit->x = 20;
        unit->y = 20;
        unit->tx = 20;
        unit->ty = 20;
        window->client->sendPacket(PACKET_TS_NEW_UNIT, unit);
        return true;
    }
    return Menu::keyPressed(key);
}

bool MenuGame::mouseDown(int x, int y, int button){
    if(button == SDL_BUTTON_LEFT){
        selectInitX = x;
        selectInitY = y;
        selectX = x;
        selectY = y;
    }else if(button == SDL_BUTTON_RIGHT){
        for(Unit* unit : selectedUnits){
            if(unit){
                unit->setTargetPos(x, y);
                window->client->sendPacket(PACKET_TS_UNIT_TARGET_SET, unit);
            }
        }
    }
    return Menu::mouseDown(x, y, button);
}

bool MenuGame::mouseMove(int x, int y){
    if(selectX != -1){
        selectX = x;
        selectY = y;
    }

    return Menu::mouseMove(x, y);
}

bool MenuGame::mouseUp(int x, int y, int button){
    if(button == SDL_BUTTON_LEFT){
        selectedUnits.clear();

        if(selectInitX > selectX){
            int temp = selectX;
            selectX = selectInitX;
            selectInitX = temp;
        }
        if(selectInitY > selectY){
            int temp = selectY;
            selectY = selectInitY;
            selectInitY = temp;
        }

        for(map<UID, Unit*>::iterator i=general->units.begin(); i!=general->units.end(); ++i){
            Unit* unit = i->second;
            if((unit->x >= selectInitX) && (unit->y >= selectInitY) && (unit->x <= selectX) && (unit->y <= selectY)){
                selectedUnits.push_back(unit);
            }
        }

        selectInitX = -1;
        selectInitY = -1;
        selectX = -1;
        selectY = -1;
    }

    debugf("Selected count: %d", (int)selectedUnits.size());

    return Menu::mouseUp(x, y, button);
}

void MenuGame::update(){
    //updates world in Client
    Menu::update();
}

void MenuGame::render(){
    window->client->world->render();

    if(selectX != -1){
    	Graphics::drawRectangleOutline(window, selectInitX, selectInitY, selectX, selectY, selectColor);
    }

    Menu::render();
}
