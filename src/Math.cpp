//
//  Math.cpp
//  Strong
//
//  Created by Braeden Atlee on 1/20/16.
//  Copyright © 2016 Braeden Atlee. All rights reserved.
//

#include "Math.hpp"

double Math::min(double a, double b){
    return a < b ? a : b;
}

double Math::abs(double a){
    return a < 0 ? -a : a;
}

double Math::clampAngle(double a){
    while(a > PI){
        a -= TAU;
    }
    while(a < -PI){
        a += TAU;
    }
    return a;
}

double Math::closestToZero(double a, double b){
    return min(abs(a), abs(b));
}