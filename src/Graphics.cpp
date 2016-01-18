//
//  Graphics.cpp
//  Strong
//
//  Created by Braeden Atlee on 1/7/16.
//  Copyright © 2016 Braeden Atlee. All rights reserved.
//

#include "Graphics.hpp"

namespace Graphics{

    Image* imageGui;

    TTF_Font* fontSmall;
    
    void init(){
        if(SDL_Init(SDL_INIT_VIDEO) == -1){
            fprintf(stderr, "SDL_Init: %s\n", SDL_GetError());
            exit(EXIT_FAILURE);
        }
        if(IMG_Init(IMG_INIT_PNG) == -1){
            fprintf(stderr, "IMG_Init: %s\n", SDL_GetError());
            exit(EXIT_FAILURE);
        }
        if(TTF_Init() == -1){
            fprintf(stderr, "TTF_Init: %s\n", SDL_GetError());
            exit(EXIT_FAILURE);
        }
    }
    
    void cleanup(){
        IMG_Quit();
        SDL_Quit();
    }

    SDL_Rect* newRect(int x, int y, int w, int h){
        SDL_Rect* rect = new SDL_Rect();

        rect->x = x;
        rect->y = y;
        rect->w = w;
        rect->h = h;

        return rect;
    }


    void loadImages(Window* window){

        imageGui = loadImage(window, "img/gui.png");

        fontSmall = loadFont(window, "img/Arial.ttf", 16);

    }

    Image* loadImage(Window* window, string path){
        SDL_Surface* surface = IMG_Load(path.c_str());
        if(surface) {
            SDL_Texture* texture = SDL_CreateTextureFromSurface(window->sdlRenderer, surface);
            SDL_FreeSurface(surface);
            if(texture){
                Image* image = new Image();
                image->sdlTexture = texture;
            	printf("Loaded image: \"%s\"\n", path.c_str());
            	return image;
            }else{
                printf("Failed to make texture from surface for image: \"%s\"\n", path.c_str());
                return nullptr;
            }
        }else{
            printf("Failed to load image: \"%s\"\n", path.c_str());
            return nullptr;
        }

    }

    TTF_Font* loadFont(Window* window, string path, int size){
        TTF_Font* font = TTF_OpenFont(path.c_str(), size);
        if(font){
            printf("Loaded font: \"%s\"[%d]\n", path.c_str(), size);
            return font;
        }else{
            printf("Failed to load font: \"%s\"[%d]\n", path.c_str(), size);
            return nullptr;
        }
    }

    void drawImage(Window* window, int x, int y, int w, int h, Image* img, SDL_Rect* clip){
        SDL_Rect r;
        r.x = x;
        r.y = y;
        r.w = w;
        r.h = h;
        SDL_RenderCopy(window->sdlRenderer, img->sdlTexture, clip, &r);
    }

    void makeImageFromText(Window* window, string text, Image* img, TTF_Font* font, SDL_Color color){
        if(img){
            SDL_Surface* surface = TTF_RenderText_Solid(font, text.c_str(), color);
            if(surface){
                SDL_Texture* texture = SDL_CreateTextureFromSurface(window->sdlRenderer, surface);
                SDL_FreeSurface(surface);
                if(texture){
                    img->sdlTexture = texture;
                }else{
                    printf("Failed to convert surface to texture.\n");
                }
            }else{
                printf("Failed to draw text to surface.\n");
            }
        }else{
            printf("Trying to save to null image.\n");
        }
    }
    
}



