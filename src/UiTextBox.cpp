//
//  UiTextBox.cpp
//  Strong
//
//  Created by Braeden Atlee on 1/19/16.
//  Copyright © 2016 Braeden Atlee. All rights reserved.
//

#include "UiTextBox.hpp"
#include "Graphics.hpp"


UiTextBox::UiTextBox(int x, int y, string text) : UiTextBox(x, y, 100, 20, text){
    fitToText = true;
}

UiTextBox::UiTextBox(int x, int y, int w, int h, string text) : UiObject(){
    this->x = x;
    this->y = y;

    widthMin = w;
    heightMin = h;

    this->text = text;

    mustUpdateText = true;
}

UiTextBox::~UiTextBox(){
    delete clipDefault;
    delete clipFocused;
    delete imageText;
}

void UiTextBox::changeText(string newText, bool force){
    if(force || text.compare(newText) != 0){
        text = newText;
        mustUpdateText = true;
    }
}



bool UiTextBox::mouseMove(int x, int y){
    if(contains(x, y)){
        hover = true;
    }else{
        hover = false;
    }
    return false; //no need to capture event
}

bool UiTextBox::mouseDown(int x, int y, int button){
    if(contains(x, y)){
        if(button == SDL_BUTTON_LEFT){
            menu->setFocused(this);
            return true;
        }
    }
    return false;
}

bool UiTextBox::keyPressed(SDL_Keysym key){
    if(focused){
        SDL_Keycode c = key.sym;
        if(key.mod & (KMOD_SHIFT | KMOD_CAPS)){
            if(c >= 'a' && c <= 'z'){
                c += 'A'-'a';
            }
        }
        if((inputMask & ENABLE_LOWERCASE) && c >= 'a' && c <= 'z'){
            changeText(text+(char)c);
            return true;
        }
        if((inputMask & ENABLE_UPERCASE) && c >= 'A' && c <= 'Z'){
            changeText(text+(char)c);
            return true;
        }
        if((inputMask & ENABLE_NUMEBRS) && c >= '0' && c <= '9'){
            changeText(text+(char)c);
            return true;
        }
        if((inputMask & ENABLE_SPACES) && c == ' '){
            changeText(text+(char)c);
            return true;
        }
        if((inputMask & ENABLE_PERIODS) && c == '.'){
            changeText(text+(char)c);
            return true;
        }
        if(key.sym == SDLK_BACKSPACE){
            if(text.length() > 0){
                changeText(text.substr(0, text.length()-1));
            }
            return true;
        }
        return false;
    }
    return false;
}

void UiTextBox::render(){

    Graphics::drawStretchableBox(menu->window, x, y, w, h, image, focused?clipFocused:clipDefault);

    if(text.size() > 0 || focused){
        if(mustUpdateText){
            mustUpdateText = false;
            Graphics::makeImageFromText(menu->window, text+(focused?"_":""), imageText, Graphics::fontSmall, textColor);
            SDL_QueryTexture(imageText->sdlTexture, NULL, NULL, &textW, &textH);
            if(fitToText){
                w = max(textW + padding*2, widthMin);
                h = max(textH + padding*2, heightMin);
            }
        }

        Graphics::drawImage(menu->window, x+padding, y+padding, textW, textH, imageText, NULL);
    }
    
}

void UiTextBox::focusChanged(){
    mustUpdateText = true;
}
