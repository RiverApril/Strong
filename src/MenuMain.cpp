//
//  MenuMain.cpp
//  Strong
//
//  Created by Braeden Atlee on 1/19/16.
//  Copyright © 2016 Braeden Atlee. All rights reserved.
//

#include "MenuMain.hpp"
#include "MenuGame.hpp"
#include "UiButton.hpp"
#include "UiTextBox.hpp"
#include "Settings.hpp"

MenuMain::MenuMain(Window* window) : Menu(window){

    buttonConnect = new UiButton(20, 20, "Connect to server", [](UiButton* uio){
        if(uio->enabled){
            uio->menu->window->client->connectToServer();
        }
    });


    textAddress = new UiTextBox(20, 50, Settings::Client::connectAddress+":"+to_string(Settings::Client::connectPort), nullptr, [this](UiTextBox* uio){
        updateSettingsFromInputs();
    });

    textUsername = new UiTextBox(20, 80, Settings::Client::connectUsername, nullptr, [this](UiTextBox* uio){
        updateSettingsFromInputs();
    });



    addUiObject(buttonConnect);
    addUiObject(textAddress);
    addUiObject(textUsername);
}

MenuMain::~MenuMain(){

}

void MenuMain::updateSettingsFromInputs(){

    string s = textAddress->text;

    buttonConnect->enabled = true;

    size_t f = s.find(":");
    if(f != string::npos){
        Settings::Client::connectAddress = s.substr(0, f);
        try{
            Settings::Client::connectPort = stoi(s.substr(f+1, s.size()));
        }catch(invalid_argument e){
            buttonConnect->enabled = false;
        };
    }else{
        Settings::Client::connectPort = 1337;
    }
    if(Settings::Client::connectAddress.size() == 0 || Settings::Client::connectPort < 0){
        buttonConnect->enabled = false;
    }

    Settings::Client::connectUsername = textUsername->text;

    if(Settings::Client::connectUsername.size() == 0){
        buttonConnect->enabled = false;
    }

}

void MenuMain::update(){
    Menu::update();

    if(window->client->connected){
        window->changeMenu(new MenuGame(this), false);
    }
    
}