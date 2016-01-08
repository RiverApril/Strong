//
//  Window.cpp
//  Strong
//
//  Created by Braeden Atlee on 1/7/16.
//  Copyright © 2016 Braeden Atlee. All rights reserved.
//

#include "Window.hpp"

Window::Window(){
    client = new Client();

    sdlWindow = SDL_CreateWindow("Strong", SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED, width, height, SDL_WINDOW_SHOWN | SDL_WINDOW_RESIZABLE);
    if(!sdlWindow){
        fprintf(stderr, "SDL_CreateWindow: %s\n", SDL_GetError());
        running = false;
    }

    sdlRenderer = SDL_CreateRenderer(sdlWindow, -1, 0);
    if(!sdlRenderer){
        fprintf(stderr, "SDL_CreateRenderer: %s\n", SDL_GetError());
        running = false;
    }

    SDL_RenderClear(sdlRenderer);
    SDL_RenderPresent(sdlRenderer);
}

Window::~Window(){
    SDL_DestroyWindow(sdlWindow);
}

void Window::update(){
    client->update();

    processEvents();

    if(currentMenu){
    	currentMenu->update();
    	currentMenu->render(this);
    }

    SDL_UpdateWindowSurface(sdlWindow);
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
                    currentMenu->mouseDown(inputEvent.button.button, inputEvent.button.x, inputEvent.button.y);
                }
                break;
            }

            case SDL_MOUSEBUTTONUP:{
                if(currentMenu){
                    currentMenu->mouseDown(inputEvent.button.button, inputEvent.button.x, inputEvent.button.y);
                }
                break;
            }

            case SDL_WINDOWEVENT:{
                switch (inputEvent.window.event) {
                    case SDL_WINDOWEVENT_RESIZED:
                        width = inputEvent.window.data1;
                        height = inputEvent.window.data2;
                        break;
                        
                    default:
                        break;
                }
                break;
            }
        }
        
    }
}






