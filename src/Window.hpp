//
//  Window.hpp
//  Strong
//
//  Created by Braeden Atlee on 1/7/16.
//  Copyright © 2016 Braeden Atlee. All rights reserved.
//

#ifndef Window_hpp
#define Window_hpp

#include "Client.hpp"

class Menu;

class Window{
public:
    
    bool running = true;
    
    Client* client;
    

    SDL_Window* sdlWindow = nullptr;
    SDL_Renderer* sdlRenderer = nullptr;
    SDL_Event inputEvent;

    Menu* currentMenu = nullptr;

    int width = 640;
    int height = 480;


    
    Window();

    void changeMenu(Menu* nextMenu, bool deleteOld);
    
    ~Window();
    
    void update();
    void processEvents();
    
};

#include "Menu.hpp"

#endif
