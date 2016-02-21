//
//  Point.hpp
//  Strong
//
//  Created by Braeden Atlee on 2/18/16.
//  Copyright © 2016 Braeden Atlee. All rights reserved.
//

#ifndef Point_hpp
#define Point_hpp

#include "Global.hpp"

struct Point{
    float x;
    float y;

    Point(float x, float y) : x(x), y(y){}

    float dot(Point& o){
        return x*o.x + y*o.y;
    }

    float distSqr(Point& o);

    void write(vector<unsigned char>& data);

    void read(unsigned char* data, size_t& position);
    
};

inline Point operator+(const Point& a, const Point& b){
    return Point(a.x+b.x, a.y+b.y);
}

inline Point operator-(const Point& a, const Point& b){
    return Point(a.x-b.x, a.y-b.y);
}

inline Point operator*(const Point& a, const Point& b){
    return Point(a.x*b.x, a.y*b.y);
}

inline Point operator/(const Point& a, const Point& b){
    return Point(a.x/b.x, a.y/b.y);
}

#endif /* Point_hpp */
