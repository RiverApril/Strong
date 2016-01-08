//
//  Menu.cpp
//  Strong
//
//  Created by Braeden Atlee on 1/8/16.
//  Copyright © 2016 Braeden Atlee. All rights reserved.
//

#include "Menu.hpp"

Menu::Menu(Window* window){
    this->window = window;
}

Menu::~Menu(){

}

void Menu::addUiObject(UiObject* uio){
    uio->uID = nextUID;
    nextUID++;
    uiObjectList.push_back(uio);
}

void Menu::setFocused(UiObject* uio){
    for(UiObject* o : uiObjectList){
        o->focused = false;
    }
    uio->focused = true;
}

bool Menu::mouseMove(int x, int y){
    for(UiObject* o : uiObjectList){
        if(o->enabled){
            if(o->mouseMove(x, y)){
                return true;
            }
        }
    }
    return false;
}

bool Menu::mouseWheel(int x, int y){
    for(UiObject* o : uiObjectList){
        if(o->enabled){
            if(o->mouseWheel(x, y)){
                return true;
            }
        }
    }
    return false;
}

bool Menu::mouseDown(int x, int y, int button){
    for(UiObject* o : uiObjectList){
        if(o->enabled){
            if(o->mouseDown(x, y, button)){
                return true;
            }
        }
    }
    return false;
}

bool Menu::mouseUp(int x, int y, int button){
    for(UiObject* o : uiObjectList){
        if(o->enabled){
            if(o->mouseUp(x, y, button)){
                return true;
            }
        }
    }
    return false;
}

bool Menu::keyPressed(SDL_Keysym key){
    for(UiObject* o : uiObjectList){
        if(o->enabled && o->focused){
            if(o->keyPressed(key)){
                return true;
            }
        }
    }
    return false;
}

bool Menu::keyReleased(SDL_Keysym key){
    for(UiObject* o : uiObjectList){
        if(o->enabled && o->focused){
            if(o->keyReleased(key)){
                return true;
            }
        }
    }
    return false;
}

void Menu::update(){
    for(UiObject* o : uiObjectList){
        if(o->enabled){
            o->update();
        }
    }
}

void Menu::render(Window* window){
    for(UiObject* o : uiObjectList){
        if(o->enabled && o->visable){
            o->render();
        }
    }
}

