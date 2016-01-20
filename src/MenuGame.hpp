//
//  MenuGame.hpp
//  Strong
//
//  Created by Braeden Atlee on 1/19/16.
//  Copyright © 2016 Braeden Atlee. All rights reserved.
//

#ifndef MenuGame_hpp
#define MenuGame_hpp

#include "Menu.hpp"

class World;

class MenuGame : public Menu{
public:

    MenuGame(Menu* upperMenu);

    virtual ~MenuGame();

    virtual void openMenu();

    virtual void closeMenu();

    virtual void update();

    virtual void render();

    Menu* upperMenu;
    
};

#endif /* MenuGame_hpp */
