//
//  Math.hpp
//  Strong
//
//  Created by Braeden Atlee on 1/20/16.
//  Copyright © 2016 Braeden Atlee. All rights reserved.
//

#ifndef Math_hpp
#define Math_hpp

#include <cmath>

#define PI (3.1415926535897932384)
#define TAU (PI*2)
#define DEG_PER_RAD (360/TAU)
#define RAD_PER_DEG (TAU/360)

namespace Math {


	template<typename T>
    T min(T a, T b){
        return a < b ? a : b;
    }

    template<typename T>
    T abs(T a){
        return a < 0 ? -a : a;
    }

    float mod(float a, float n);

    bool intersect(int ax1, int ay1, int ax2, int ay2, int bx1, int by1, int bx2, int by2);

    float normalizeAngle(float a);

    float closestToZero(float a, float b);

}

#endif /* Math_hpp */
