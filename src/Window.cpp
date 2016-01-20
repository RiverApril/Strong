//
//  Window.cpp
//  Strong
//
//  Created by Braeden Atlee on 1/7/16.
//  Copyright © 2016 Braeden Atlee. All rights reserved.
//

#include "Window.hpp"
#include "MenuMain.hpp"
#include "Graphics.hpp"
#include "Settings.hpp"

Window::Window(){
    client = new Client();

    sdlWindow = SDL_CreateWindow("Strong", SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED, width, height, SDL_WINDOW_SHOWN | SDL_WINDOW_RESIZABLE);
    if(!sdlWindow){
        fprintf(stderr, "SDL_CreateWindow: %s\n", SDL_GetError());
        running = false;
    }

    sdlRenderer = SDL_CreateRenderer(sdlWindow, -1, SDL_RENDERER_ACCELERATED);
    if(!sdlRenderer){
        fprintf(stderr, "SDL_CreateRenderer: %s\n", SDL_GetError());
        running = false;
    }


    Graphics::loadImages(this);
    
    changeMenu(new MenuMain(this), true);
}

void Window::changeMenu(Menu* nextMenu, bool deleteOld){
    if(currentMenu){
        currentMenu->closeMenu();
        if(deleteOld){
        	delete currentMenu;
        }
    }
    currentMenu = nextMenu;
    currentMenu->openMenu();
}

Window::~Window(){
    SDL_DestroyWindow(sdlWindow);
}

void Window::update(){

    currentTime = SDL_GetTicks();

    processEvents();

    while(previousTime + TIME_PER_UPDATE < currentTime){
        previousTime += TIME_PER_UPDATE;

        client->update();

        if(currentMenu){
            currentMenu->update();
        }

        SDL_RenderClear(sdlRenderer);

        if(currentMenu){
            currentMenu->render();
        }

        SDL_RenderPresent(sdlRenderer);
    }
}

void Window::processEvents(){
    while(SDL_PollEvent(&inputEvent)){

        switch(inputEvent.type){
            case SDL_QUIT:{
                running = false;
                break;
            }

            case SDL_KEYDOWN:{
                if(currentMenu){
                    currentMenu->keyPressed(inputEvent.key.keysym);
                }
                break;
            }

            case SDL_KEYUP:{
                if(currentMenu){
                    currentMenu->keyReleased(inputEvent.key.keysym);
                }
                break;
            }

            case SDL_TEXTINPUT:{
                if(currentMenu){
                    currentMenu->textInput(inputEvent.text.text);
                }
            }

            case SDL_MOUSEMOTION:{
                if(currentMenu){
                    currentMenu->mouseMove(inputEvent.motion.x, inputEvent.motion.y);
                }
                break;
            }

            case SDL_MOUSEWHEEL:{
                if(currentMenu){
                    currentMenu->mouseWheel(inputEvent.wheel.x, inputEvent.wheel.y);
                }
                break;
            }

            case SDL_MOUSEBUTTONDOWN:{
                if(currentMenu){
                    currentMenu->mouseDown(inputEvent.button.x, inputEvent.button.y, inputEvent.button.button);
                }
                break;
            }

            case SDL_MOUSEBUTTONUP:{
                if(currentMenu){
                    currentMenu->mouseUp(inputEvent.button.x, inputEvent.button.y, inputEvent.button.button);
                }
                break;
            }

            case SDL_WINDOWEVENT:{
                switch (inputEvent.window.event) {
                    case SDL_WINDOWEVENT_RESIZED:
                        width = inputEvent.window.data1;
                        height = inputEvent.window.data2;
                        SDL_RenderSetLogicalSize(sdlRenderer, width, height);
                        if(currentMenu){
                            currentMenu->windowResized();
                        }
                        break;
                        
                    default:
                        break;
                }
                break;
            }
        }
        
    }
}






