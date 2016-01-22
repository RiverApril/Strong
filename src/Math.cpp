//
//  Math.cpp
//  Strong
//
//  Created by Braeden Atlee on 1/20/16.
//  Copyright © 2016 Braeden Atlee. All rights reserved.
//

#include "Math.hpp"

float Math::mod(float a, float n){
    return a - floor(a/n) * n;
}

bool Math::intersect(int ax, int ay, int aw, int ah, int bx, int by, int bw, int bh){
    return (abs(ax - bx) * 2 <= (aw + bw)) &&
           (abs(ay - by) * 2 <= (ah + bh));
}

float Math::normalizeAngle(float a){
    while(a > PI){
        a -= TAU;
    }
    while(a < -PI){
        a += TAU;
    }
    return a;
}

float Math::closestToZero(float a, float b){
    return min(abs(a), abs(b));
}