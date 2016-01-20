//
//  Unit.hpp
//  Strong
//
//  Created by Braeden Atlee on 1/19/16.
//  Copyright © 2016 Braeden Atlee. All rights reserved.
//

#ifndef Unit_hpp
#define Unit_hpp

#include "General.hpp"

class Unit{
public:

    Unit(General* general);
    ~Unit();

    void writeData(vector<unsigned char>& data);
    void readData(unsigned char* data, size_t& position);
    Unit(General* general, unsigned char* data, size_t& position);

    void update();
    void render();

    double x = 0;
    double y = 0;

    double xs = 0;
    double ys = 0;

    double angle = 0;

    General* general;
    
};

#endif /* Unit_hpp */
