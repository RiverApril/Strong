//
//  Graphics.cpp
//  Strong
//
//  Created by Braeden Atlee on 1/7/16.
//  Copyright © 2016 Braeden Atlee. All rights reserved.
//

#include "Global.hpp"

namespace Graphics{
    
    void init(){
        if(SDL_Init(SDL_INIT_EVERYTHING) == -1){
            fprintf(stderr, "SDL_Init: %s\n", SDL_GetError());
            exit(EXIT_FAILURE);
        }
    }
    
    void cleanup(){
        SDL_Quit();
    }
    
}
