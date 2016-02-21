//
//  Orientation.hpp
//  Strong
//
//  Created by Braeden Atlee on 2/18/16.
//  Copyright © 2016 Braeden Atlee. All rights reserved.
//

#ifndef Orientation_hpp
#define Orientation_hpp

#include "Global.hpp"
#include "Point.hpp"

struct Orientation : public Point{
    float a;

    Orientation(float x, float y, float a) : Point(x, y), a(a){}

    

    void write(vector<unsigned char>& data);

    void read(unsigned char* data, size_t& position);

};

inline bool operator==(const Point& a, const Point& b){
    return a.x==b.x && a.y==b.y;
}

inline bool operator!=(const Point& a, const Point& b){
    return !(a==b);
}

#endif /* Orientation_hpp */
