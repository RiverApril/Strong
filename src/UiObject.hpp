//
//  UiObject.hpp
//  Strong
//
//  Created by Braeden Atlee on 1/8/16.
//  Copyright © 2016 Braeden Atlee. All rights reserved.
//

#ifndef UiObject_hpp
#define UiObject_hpp

#include "Global.hpp"

class Window;
class Menu;

class UiObject{
public:
    
    UiObject();

    virtual ~UiObject();

    virtual bool contains(int x, int y);

    virtual bool mouseWheel(int x, int y);

    virtual bool mouseMove(int x, int y);

    virtual bool mouseDown(int x, int y, int button);

    virtual bool mouseUp(int x, int y, int button);

    virtual bool keyPressed(SDL_Keysym key);

    virtual bool keyReleased(SDL_Keysym key);

    virtual void windowResized();

    virtual void update();

    virtual void render();


    int x, y, w, h;

    bool enabled = true;
    bool visable = true;
    bool focused = false;
    int uID = -1;

    Menu* menu;
};

#include "Window.hpp"

#endif
