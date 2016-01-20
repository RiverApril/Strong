//
//  UiTextBox.cpp
//  Strong
//
//  Created by Braeden Atlee on 1/19/16.
//  Copyright © 2016 Braeden Atlee. All rights reserved.
//

#include "UiTextBox.hpp"
#include "Graphics.hpp"


UiTextBox::UiTextBox(int x, int y, string text, function<void(UiTextBox*)> onEnter, function<void(UiTextBox*)> onChange) : UiTextBox(x, y, 100, 20, text, onEnter, onChange){
    fitToText = true;
}

UiTextBox::UiTextBox(int x, int y, int w, int h, string text, function<void(UiTextBox*)> onEnter, function<void(UiTextBox*)> onChange) : UiObject(){
    this->x = x;
    this->y = y;

    widthMin = w;
    heightMin = h;

    this->text = text;

    this->onEnter = onEnter;
    this->onChange = onChange;

    mustUpdateText = true;
}

UiTextBox::~UiTextBox(){
    delete imageText;
}

void UiTextBox::changeText(string newText, bool force){
    if(force || text.compare(newText) != 0){
        text = newText;
        mustUpdateText = true;
        if(onChange){
            onChange(this);
        }
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
        if(key.sym == SDLK_BACKSPACE){
            if(text.size() > 0){
        		changeText(text.substr(0, text.size()-1));
            }
        	return true;
        }else if(key.sym == SDLK_RETURN){
            if(onEnter){
            	onEnter(this);
            }
            return true;
        }
    }
    return false;
}


bool UiTextBox::textInput(string s){
    if(focused){
        changeText(text+s);
        return true;
    }
    return false;
}

void UiTextBox::render(){

    Graphics::drawStretchableBox(menu->window, x, y, w, h, image, enabled?(focused?&clipFocused:&clipDefault):&clipDisabled);

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
