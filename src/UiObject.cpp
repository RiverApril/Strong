//
//  UiObject.cpp
//  Strong
//
//  Created by Braeden Atlee on 1/8/16.
//  Copyright © 2016 Braeden Atlee. All rights reserved.
//

#include "UiObject.hpp"


UiObject::UiObject(Menu* menu){
    this->menu = menu;
}

UiObject::~UiObject(){

}

bool UiObject::contains(int x, int y){
    return (x >= this->x) && (y >= this->y) && (x <= this->x+this->w) && (y <= this->y+this->h);
}

bool UiObject::mouseWheel(int x, int y){
    return false;
}

bool UiObject::mouseMove(int x, int y){
    return false;
}

bool UiObject::mouseDown(int x, int y, int button){
    return false;
}

bool UiObject::mouseUp(int x, int y, int button){
    return false;
}

bool UiObject::keyPressed(SDL_Keysym key){
    return false;
}

bool UiObject::keyReleased(SDL_Keysym key){
    return false;
}

void UiObject::update(){

}

void UiObject::render(){

}

