//
//  UiButton.cpp
//  Strong
//
//  Created by Braeden Atlee on 1/18/16.
//  Copyright © 2016 Braeden Atlee. All rights reserved.
//

#include "UiButton.hpp"
#include "Graphics.hpp"


UiButton::UiButton(int x, int y, string text, function<void(UiButton*)> click) : UiButton(x, y, 0, 0, text, click){
    fitToText = true;
}

UiButton::UiButton(int x, int y, int w, int h, string text, function<void(UiButton*)> click) : UiObject(){
    this->x = x;
    this->y = y;
    this->w = w;
    this->h = h;
    this->text = text;
    this->click = click;

    mustUpdateText = true;
}

UiButton::~UiButton(){
    delete clipDefault;
    delete clipHover;
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
    if(contains(x, y)){
        if(button == SDL_BUTTON_LEFT){
            clickedDown = true;
            return true;
        }
    }
    return false;
}

bool UiButton::mouseUp(int x, int y, int button){
    if(contains(x, y)){
        if(button == SDL_BUTTON_LEFT && clickedDown){
            clickedDown = false;
            click(this);
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

    Graphics::drawImage(menu->window, x, y, w, h, image, hover?clipHover:clipDefault);
    Graphics::drawImage(menu->window, x+padding, y+padding, textW, textH, imageText, NULL);

}

