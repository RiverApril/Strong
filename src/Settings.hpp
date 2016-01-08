//
//  Settings.hpp
//  Strong
//
//  Created by Braeden Atlee on 1/7/16.
//  Copyright © 2016 Braeden Atlee. All rights reserved.
//

#ifndef Settings_hpp
#define Settings_hpp

#include "Global.hpp"

namespace Settings {


    class Setting {
    public:
        Setting(string name);

        virtual ~Setting();

        virtual string getValueAsString();
        virtual void setValueFromString(string s);

        string name;

    };

    class SettingInt : public Setting {
    public:

        SettingInt(string name, int* value);

        virtual ~SettingInt();

        virtual string getValueAsString();

        virtual void setValueFromString(string s);

        int* value;

    };

    class SettingString : public Setting {
    public:

        SettingString(string name, string* value);

        virtual ~SettingString();

        virtual string getValueAsString();

        virtual void setValueFromString(string s);
        
        string* value;
        
    };

    class SettingBool : public Setting {
    public:

        SettingBool(string name, bool* value);

        virtual ~SettingBool();

        virtual string getValueAsString();

        virtual void setValueFromString(string s);
        
        bool* value;
        
    };

    namespace Client {
        extern string connectAddress;
        extern int connectPort;
        extern string connectUsername;

        extern vector<Setting*> settingList;
    }

    namespace Server {
        extern int hostPort;

        extern vector<Setting*> settingList;
    }

    void saveSettings(bool isServer);

    void loadSettings(bool isServer);

}


#endif
