//
//  Settings.cpp
//  Strong
//
//  Created by Braeden Atlee on 1/7/16.
//  Copyright © 2016 Braeden Atlee. All rights reserved.
//

#include "Settings.hpp"

namespace Settings {



    Setting::Setting(string name) {
        this->name = name;
    }

    Setting::~Setting(){}

    string Setting::getValueAsString(){
        return "ERROR";
    }

    void Setting::setValueFromString(string s){

    }



    SettingInt::SettingInt(string name, int* value) : Setting(name) {
        this->value = value;
    }

    SettingInt::~SettingInt(){}

    string SettingInt::getValueAsString(){
        return to_string(*value);
    }

    void SettingInt::setValueFromString(string s){
        *value = stoi(s);
    }



    SettingString::SettingString(string name, string* value) : Setting(name) {
        this->value = value;
    }

    SettingString::~SettingString(){}

    string SettingString::getValueAsString(){
        return *value;
    }

    void SettingString::setValueFromString(string s){
        *value = s;
    }




    SettingBool::SettingBool(string name, bool* value) : Setting(name) {
        this->value = value;
    }

    SettingBool::~SettingBool(){}

    string SettingBool::getValueAsString(){
        return *value?"true":"false";
    }

    void SettingBool::setValueFromString(string s){
        *value = s.compare("true") == 0;
    }

    namespace Client {
        string connectAddress = "localhost";
        int connectPort = 1337;
        string connectUsername = "username";

        vector<Setting*> settingList = {
            new SettingString("IP Address", &connectAddress),
            new SettingInt("Port", &connectPort),
            new SettingString("Username", &connectUsername),
        };
    }

    namespace Server {
        int hostPort = -1;

        vector<Setting*> settingList = {
            new SettingInt("Port", &hostPort),
        };
    }

    void saveSettings(bool isServer){

        vector<Setting*>* settingList;
        string fileName = "";

        if(isServer){
            settingList = &(Server::settingList);
            fileName = "serverSettings.txt";
        }else{
            settingList = &(Client::settingList);
            fileName = "settings.txt";
        }

        FILE* file = fopen(fileName.c_str(), "w");
        if(file){
            for(Setting* setting: *settingList){
                fputs((setting->name+":"+setting->getValueAsString()+"\n").c_str(), file);
            }
            fclose(file);
        }else{
            fprintf(stderr, "Failed to save file: %s\n", fileName.c_str());
        }
    }

    void loadSettings(bool isServer){
        vector<Setting*>* settingList;
        string fileName = "";

        if(isServer){
            settingList = &(Server::settingList);
            fileName = "serverSettings.txt";
        }else{
            settingList = &(Client::settingList);
            fileName = "settings.txt";
        }

        FILE* file = fopen(fileName.c_str(), "r");
        if(file){
            int c;
            string s = "";
            string key = "";
            string value = "";
            do{
                c = fgetc(file);
                if(c == '\n'){
                    if(key.size()>0){
                        value = s;

                        for(Setting* s : *settingList){
                            if(s->name.compare(key) == 0){
                                s->setValueFromString(value);
                            }
                        }
                    }
                    s = "";
                    key = "";
                    value = "";
                }else if(c == ':' && key.size()==0){
                    key = s;
                    s = "";
                }else{
                    s += (char)c;
                }
            }while(c != EOF);
            fclose(file);
        }else{
            fprintf(stderr, "Failed to load file: %s\n", fileName.c_str());
        }
    }
    
}