//
//  MenuMain.hpp
//  Strong
//
//  Created by Braeden Atlee on 1/19/16.
//  Copyright © 2016 Braeden Atlee. All rights reserved.
//

#ifndef MenuMain_hpp
#define MenuMain_hpp

#include "Menu.hpp"
#include "UiButton.hpp"
#include "UiTextBox.hpp"

class MenuMain : public Menu{
public:
    
    MenuMain(Window* window);

    virtual ~MenuMain();

    UiButton* buttonConnect;
    UiTextBox* textAddress;
    UiTextBox* textUsername;
    
};

#endif /* MenuMain_hpp */
