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

namespace Math {

    double min(double a, double b);
    double abs(double a);

    double clampAngle(double a);

    double closestToZero(double a, double b);

}

#endif /* Math_hpp */
