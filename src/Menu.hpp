//
//  Menu.hpp
//  Strong
//
//  Created by Braeden Atlee on 1/8/16.
//  Copyright © 2016 Braeden Atlee. All rights reserved.
//

#ifndef Menu_hpp
#define Menu_hpp

#include "Global.hpp"
#include "UiObject.hpp"

class Menu{
public:

    Menu(Window* window);
    
    virtual ~Menu();

    virtual void openMenu();

    virtual void closeMenu();

    void addUiObject(UiObject* uio);

    void setFocused(UiObject* uio);

    virtual bool mouseMove(int x, int y);

    virtual bool mouseWheel(int x, int y);

    virtual bool mouseDown(int x, int y, int button);

    virtual bool mouseUp(int x, int y, int button);

    virtual bool keyPressed(SDL_Keysym key);

    virtual bool keyReleased(SDL_Keysym key);

    virtual bool textInput(string s);

    virtual void windowResized();

    virtual void update();

    virtual void render();

    vector<UiObject*> uiObjectList;

    int nextUID = 1;

    Window* window;
    
};


#endif
