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
#include "Math.hpp"

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
    void setTargetPos(float nx, float ny);
    void setTargetAngle(float nAngle);

    General* general;

    Image* image = Graphics::imageGame;

    SDL_Rect clip = Graphics::rect(0, 0, 12, 12);

    UID uid = 0;

    //Changes Frequently:
    float x = 0;
    float y = 0;

    float tx = 0;
    float ty = 0;

    float angle = 0;
    float tAngle = 0;

    //Mostly constant:
    float speedMove = 1;
    float speedRot = TAU/90;

    float width = 12;
    float height = 12;

    int count = 500;
    float morale = 1;
    float attackPotential = 1;

    
};

#endif /* Unit_hpp */
