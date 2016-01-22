//
//  Graphics.hpp
//  Strong
//
//  Created by Braeden Atlee on 1/7/16.
//  Copyright © 2016 Braeden Atlee. All rights reserved.
//

#ifndef Graphics_hpp
#define Graphics_hpp

#include "Global.hpp"
#include <SDL2/SDL_image.h>
#include <SDL2/SDL_ttf.h>

class Image;
class Window;

namespace Graphics{

    extern Image* imageGui;
    extern Image* imageGame;

    extern TTF_Font* fontSmall;
    
    void init();
    void cleanup();

    SDL_Rect rect(int x, int y, int w, int h);

    void loadImages(Window* window);
    Image* loadImage(Window* window, string path);
    TTF_Font* loadFont(Window* window, string path, int size);
    void makeClips(Image* img, int clipW, int clipH);

    void drawRectangleOutline(Window* window, int x, int y, int w, int h, SDL_Color color);
    
    void drawImage(Window* window, int x, int y, int w, int h, Image* img, SDL_Rect* clip);
    void drawImageEx(Window* window, int x, int y, int w, int h, Image* img, SDL_Rect* clip, float angle = 0, int cx = 0, int cy = 0, SDL_RendererFlip flip = SDL_FLIP_NONE);
    void drawStretchableBox(Window* window, int x, int y, int w, int h, Image* img, SDL_Rect* clip);
    
    void makeImageFromText(Window* window, string text, Image* img, TTF_Font* font, SDL_Color color);
    
}

#include "Image.hpp"
#include "Window.hpp"

#endif