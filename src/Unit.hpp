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
#include "Graphics.hpp"

class Unit{
public:

    Unit(General* general);
    ~Unit();

    void writeAllData(vector<unsigned char>& data);
    void readAllData(unsigned char* data, size_t& position);
    Unit(General* general, unsigned char* data, size_t& position);

    void writeTargetData(vector<unsigned char>& data);
    void readTargetData(unsigned char* data, size_t& position);

    void writePosData(vector<unsigned char>& data);
    void readPosData(unsigned char* data, size_t& position);

    void update();
    void render();

    UID uid = 0;

    double x = 0;
    double y = 0;

    double tx = 0;
    double ty = 0;

    double angle = 0;
    double tAngle = 0;

    void setTargetPos(double nx, double ny);
    void setTargetAngle(double nAngle);

    General* general;

    Image* image = Graphics::imageGame;

    SDL_Rect clip = Graphics::rect(0, 0, 12, 12);
    
};

#endif /* Unit_hpp */
