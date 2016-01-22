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

#define VIEW_SCAN_SPEED 1

MenuGame::MenuGame(Menu* upperMenu) : Menu(upperMenu->window){
    this->upperMenu = upperMenu;
    keyboardState = SDL_GetKeyboardState(nullptr);
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

        int x, y;
        SDL_GetMouseState(&x, &y);

        unit->x = x;
        unit->y = y;
        unit->tx = x;
        unit->ty = y;
        
        window->client->sendPacket(PACKET_TS_NEW_UNIT, unit);
        return true;
    }
    return Menu::keyPressed(key);
}

bool MenuGame::mouseDown(int x, int y, int button){
    if(button == SDL_BUTTON_LEFT){
        if(!(keyboardState[SDL_SCANCODE_LSHIFT] || keyboardState[SDL_SCANCODE_RSHIFT])){
            selectedUnits.clear();
        }
        selectInitX = x;
        selectInitY = y;
        selectX = x;
        selectY = y;
    }else if(button == SDL_BUTTON_RIGHT){
        for(Unit* unit : selectedUnits){
            if(unit){
                unit->setTargetPos(x-viewOffsetX, y-viewOffsetY);
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

        int selectWidth = selectX-selectInitX;
        int selectHeight = selectY-selectInitY;
        int selectCenterX = selectInitX+selectWidth/2;
        int selectCenterY = selectInitY+selectHeight/2;

        for(map<UID, Unit*>::iterator i=general->units.begin(); i!=general->units.end(); ++i){
            Unit* unit = i->second;
            if(Math::intersect(viewOffsetX+unit->x, viewOffsetX+unit->y, unit->width, unit->height, selectCenterX, selectCenterY, selectWidth, selectHeight)){
                selectedUnits.push_back(unit);
                if(selectInitX == selectX && selectInitY == selectY){
                    break;
                }
            }
        }

        selectInitX = -1;
        selectInitY = -1;
        selectX = -1;
        selectY = -1;
    }

    //debugf("Selected count: %d", (int)selectedUnits.size());

    return Menu::mouseUp(x, y, button);
}

void MenuGame::update(){
    //updates world in Client
    if(keyboardState[SDL_SCANCODE_UP]){
        viewOffsetY += VIEW_SCAN_SPEED;
    }
    if(keyboardState[SDL_SCANCODE_DOWN]){
        viewOffsetY -= VIEW_SCAN_SPEED;
    }
    if(keyboardState[SDL_SCANCODE_LEFT]){
        viewOffsetX += VIEW_SCAN_SPEED;
    }
    if(keyboardState[SDL_SCANCODE_RIGHT]){
        viewOffsetX -= VIEW_SCAN_SPEED;
    }
    Menu::update();
}

void MenuGame::render(){
    window->client->world->render();

    if(selectX != -1){
    	Graphics::drawRectangleOutline(window, selectInitX, selectInitY, selectX-selectInitX, selectY-selectInitY, selectColor);
    }

    if(selectedUnits.size() > 0){
        for(Unit* unit : selectedUnits){
            Graphics::drawImageEx(window, viewOffsetX+unit->x-(unit->width/2), viewOffsetY+unit->y-(unit->height/2), unit->width, unit->height, Graphics::imageGame, &arrowClip, unit->angle*DEG_PER_RAD, (unit->width/2), (unit->height/2));
        }
    }

    Menu::render();
}
