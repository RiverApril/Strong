//
//  UiButton.cpp
//  Strong
//
//  Created by Braeden Atlee on 1/18/16.
//  Copyright © 2016 Braeden Atlee. All rights reserved.
//

#include "UiButton.hpp"
#include "Graphics.hpp"


UiButton::UiButton(int x, int y, string text, function<void(UiButton*)> onClick) : UiButton(x, y, 100, 20, text, onClick){
    fitToText = true;
}

UiButton::UiButton(int x, int y, int w, int h, string text, function<void(UiButton*)> onClick) : UiObject(){
    this->x = x;
    this->y = y;
    this->w = w;
    this->h = h;
    this->text = text;
    this->onClick = onClick;

    mustUpdateText = true;
}

UiButton::~UiButton(){
    delete imageText;
}

void UiButton::changeText(string newText, bool force){
    if(force || text.compare(newText) != 0){
        text = newText;
        mustUpdateText = true;
    }
}



bool UiButton::mouseMove(int x, int y){
    if(contains(x, y)){
        hover = true;
    }else{
        hover = false;
    }
    return false; //no need to capture event
}

bool UiButton::mouseDown(int x, int y, int button){
    if(button == SDL_BUTTON_LEFT){
        if(contains(x, y)){
            clickedDown = true;
            return true;
        }
    }
    return false;
}

bool UiButton::mouseUp(int x, int y, int button){
    if(button == SDL_BUTTON_LEFT && clickedDown){
        if(contains(x, y)){
            clickedDown = false;
            onClick(this);
            return true;
        }
    }
    return false;
}

void UiButton::render(){

    if(mustUpdateText){
        mustUpdateText = false;
        Graphics::makeImageFromText(menu->window, text, imageText, Graphics::fontSmall, textColor);
        SDL_QueryTexture(imageText->sdlTexture, NULL, NULL, &textW, &textH);
        if(fitToText){
            w = textW + padding*2;
            h = textH + padding*2;
        }
    }

    Graphics::drawStretchableBox(menu->window, x, y, w, h, image, enabled?(hover?&clipHover:&clipDefault):&clipDisabled);
    Graphics::drawImage(menu->window, x+padding, y+padding, textW, textH, imageText, NULL);

}

