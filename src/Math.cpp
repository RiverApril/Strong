//
//  Math.cpp
//  Strong
//
//  Created by Braeden Atlee on 1/20/16.
//  Copyright © 2016 Braeden Atlee. All rights reserved.
//

#include "Math.hpp"

namespace Math {

    float mod(float a, float n){
        return a - floor(a/n) * n;
    }

    bool intersect(int ax, int ay, int aw, int ah, int bx, int by, int bw, int bh){
        return (abs(ax - bx) * 2 <= (aw + bw)) &&
               (abs(ay - by) * 2 <= (ah + bh));
    }

    float normalizeAngle(float a){
        while(a > PI){
            a -= TAU;
        }
        while(a < -PI){
            a += TAU;
        }
        return a;
    }

    float closestToZero(float a, float b){
        return min(abs(a), abs(b));
    }


}

