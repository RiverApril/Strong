//
//  Graphics.cpp
//  Strong
//
//  Created by Braeden Atlee on 1/7/16.
//  Copyright © 2016 Braeden Atlee. All rights reserved.
//

#include "Graphics.hpp"
#include "Debug.hpp"

namespace Graphics{

    Image* imageGui;
    Image* imageGame;

    TTF_Font* fontSmall;
    
    void init(){
        if(SDL_Init(SDL_INIT_VIDEO) == -1){
            errorf("SDL_Init: %s", SDL_GetError());
            exit(EXIT_FAILURE);
        }
        if(IMG_Init(IMG_INIT_PNG) == -1){
            errorf("IMG_Init: %s", SDL_GetError());
            exit(EXIT_FAILURE);
        }
        if(TTF_Init() == -1){
            errorf("TTF_Init: %s", SDL_GetError());
            exit(EXIT_FAILURE);
        }

        SDL_StartTextInput();
    }
    
    void cleanup(){
        IMG_Quit();
        SDL_Quit();
    }

    SDL_Rect rect(int x, int y, int w, int h){
        SDL_Rect rect;

        rect.x = x;
        rect.y = y;
        rect.w = w;
        rect.h = h;

        return rect;
    }


    void loadImages(Window* window){

        imageGui = loadImage(window, "img/gui.png");
        imageGame = loadImage(window, "img/game.png");

        fontSmall = loadFont(window, "img/font.ttf", 12);

    }

    Image* loadImage(Window* window, string path){
        SDL_Surface* surface = IMG_Load(path.c_str());
        if(surface) {
            SDL_Texture* texture = SDL_CreateTextureFromSurface(window->sdlRenderer, surface);
            SDL_FreeSurface(surface);
            if(texture){
                Image* image = new Image();
                image->sdlTexture = texture;
            	debugf("Loaded image: \"%s\"", path.c_str());
            	return image;
            }else{
                debugf("Failed to make texture from surface for image: \"%s\"", path.c_str());
                return nullptr;
            }
        }else{
            debugf("Failed to load image: \"%s\"", path.c_str());
            return nullptr;
        }

    }

    TTF_Font* loadFont(Window* window, string path, int size){
        TTF_Font* font = TTF_OpenFont(path.c_str(), size);
        if(font){
            debugf("Loaded font: \"%s\"[%d]", path.c_str(), size);
            return font;
        }else{
            debugf("Failed to load font: \"%s\"[%d]", path.c_str(), size);
            return nullptr;
        }
    }

    void drawRectangleOutline(Window* window, int x1, int y1, int x2, int y2, SDL_Color color){
        SDL_Point p[5];
        p[0].x = x1;
        p[0].y = y1;
        p[1].x = x2;
        p[1].y = y1;
        p[2].x = x2;
        p[2].y = y2;
        p[3].x = x1;
        p[3].y = y2;
        p[4] = p[0];

        SDL_SetRenderDrawColor(window->sdlRenderer, color.r, color.g, color.b, color.a);
        SDL_RenderDrawLines(window->sdlRenderer, p, 5);
    }

    void drawImage(Window* window, int x, int y, int w, int h, Image* img, SDL_Rect* clip){
        SDL_Rect r;
        r.x = x;
        r.y = y;
        r.w = w;
        r.h = h;
        SDL_RenderCopy(window->sdlRenderer, img->sdlTexture, clip, &r);
    }

    void drawStretchableBox(Window* window, int x, int y, int w, int h, Image* img, SDL_Rect* clip){
        SDL_Rect r;

        SDL_Rect rr;
        rr.w = clip->w/3;
        rr.h = clip->h/3;

        //Top Left
        rr.x = clip->x;
        rr.y = clip->y;
        r.x = x;
        r.y = y;
        r.w = rr.w;
        r.h = rr.h;
        SDL_RenderCopy(window->sdlRenderer, img->sdlTexture, &rr, &r);

        //Top Middle
        rr.x = clip->x+rr.w;
        rr.y = clip->y;
        r.x = x+rr.w;
        r.y = y;
        r.w = w-(rr.w*2);
        r.h = rr.h;
        SDL_RenderCopy(window->sdlRenderer, img->sdlTexture, &rr, &r);

        //Top Right
        rr.x = clip->x+(rr.w*2);
        rr.y = clip->y;
        r.x = x+w-rr.w;
        r.y = y;
        r.w = rr.w;
        r.h = rr.h;
        SDL_RenderCopy(window->sdlRenderer, img->sdlTexture, &rr, &r);

        //Middle Left
        rr.x = clip->x;
        rr.y = clip->y+rr.h;
        r.x = x;
        r.y = y+rr.h;
        r.w = rr.w;
        r.h = h-(rr.h*2);
        SDL_RenderCopy(window->sdlRenderer, img->sdlTexture, &rr, &r);

        //Middle Middle
        rr.x = clip->x+rr.w;
        rr.y = clip->y+rr.h;
        r.x = x+rr.w;
        r.y = y+rr.h;
        r.w = w-(rr.w*2);
        r.h = h-(rr.h*2);
        SDL_RenderCopy(window->sdlRenderer, img->sdlTexture, &rr, &r);

        //Middle Right
        rr.x = clip->x+(rr.w*2);
        rr.y = clip->y+rr.h;
        r.x = x+w-rr.w;
        r.y = y+rr.h;
        r.w = rr.w;
        r.h = h-(rr.h*2);
        SDL_RenderCopy(window->sdlRenderer, img->sdlTexture, &rr, &r);

        //Bottom Left
        rr.x = clip->x;
        rr.y = clip->y+(rr.h*2);
        r.x = x;
        r.y = y+h-rr.h;
        r.w = rr.w;
        r.h = rr.h;
        SDL_RenderCopy(window->sdlRenderer, img->sdlTexture, &rr, &r);

        //Bottom Middle
        rr.x = clip->x+rr.w;
        rr.y = clip->y+(rr.h*2);
        r.x = x+rr.w;
        r.y = y+h-rr.h;
        r.w = w-(rr.w*2);
        r.h = rr.h;
        SDL_RenderCopy(window->sdlRenderer, img->sdlTexture, &rr, &r);

        //Bottom Right
        rr.x = clip->x+(rr.w*2);
        rr.y = clip->y+(rr.h*2);
        r.x = x+w-rr.w;
        r.y = y+h-rr.h;
        r.w = rr.w;
        r.h = rr.h;
        SDL_RenderCopy(window->sdlRenderer, img->sdlTexture, &rr, &r);
    }

    void makeImageFromText(Window* window, string text, Image* img, TTF_Font* font, SDL_Color color){
        if(img){
            SDL_Surface* surface = TTF_RenderText_Blended(font, text.c_str(), color);
            if(surface){
                SDL_Texture* texture = SDL_CreateTextureFromSurface(window->sdlRenderer, surface);
                SDL_FreeSurface(surface);
                if(texture){
                    img->sdlTexture = texture;
                }else{
                    debugf("Failed to convert surface to texture.");
                }
            }else{
                debugf("Failed to draw text to surface.");
            }
        }else{
            debugf("Trying to save to null image.");
        }
    }
    
}



