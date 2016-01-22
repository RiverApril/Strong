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
#include "Graphics.hpp"

class Menu;

class Window{
public:
    
    bool running = true;
    
    Client* client;
    

    SDL_Window* sdlWindow = nullptr;
    SDL_Renderer* sdlRenderer = nullptr;
    SDL_Event inputEvent;

    SDL_Rect windowRect = Graphics::rect(0, 0, 640, 480);

    Menu* currentMenu = nullptr;

    Uint32 currentTime = 0;
    Uint32 previousTime = 0;
    
    Window();
    
    ~Window();


    void changeMenu(Menu* nextMenu, bool deleteOld);
    void wasResized(int width, int height);
    void update();
    void processEvents();
    
};

#include "Menu.hpp"

#endif
