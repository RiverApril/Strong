//
//  UiButton.hpp
//  Strong
//
//  Created by Braeden Atlee on 1/18/16.
//  Copyright © 2016 Braeden Atlee. All rights reserved.
//

#ifndef UiButton_hpp
#define UiButton_hpp

#include "Menu.hpp"
#include "Image.hpp"

class UiButton : public UiObject {
public:

    UiButton(int x, int y, string text, function<void(UiButton*)> click);
    UiButton(int x, int y, int w, int h, string text, function<void(UiButton*)> click);

    virtual ~UiButton();

    virtual bool mouseMove(int x, int y);

    virtual bool mouseDown(int x, int y, int button);

    virtual bool mouseUp(int x, int y, int button);

    virtual void render();

    virtual void changeText(string newText, bool force = false);

    bool fitToText = false;
    int padding = 5;

    bool clickedDown = false;
    bool hover = false;

    string text;
    bool mustUpdateText;
    SDL_Color textColor = {255, 255, 255, 255};
    int textW, textH;
    function<void(UiButton*)> click;

    Image* image = Graphics::imageGui;
    Image* imageText = new Image();
    SDL_Rect* clipDefault = Graphics::newRect(0, 0, 16, 16);
    SDL_Rect* clipHover = Graphics::newRect(16, 0, 16, 16);
};

#endif /* UiButton_hpp */
