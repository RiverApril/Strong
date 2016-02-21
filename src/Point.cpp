//
//  Point.cpp
//  Strong
//
//  Created by Braeden Atlee on 2/18/16.
//  Copyright © 2016 Braeden Atlee. All rights reserved.
//

#include "Point.hpp"
#include "Network.hpp"
#include "Math.hpp"


void Point::write(vector<unsigned char>& data){
    Network::addDataNumber(data, x);
    Network::addDataNumber(data, y);
}

void Point::read(unsigned char* data, size_t& position){
    Network::readDataNumber(data, position, x);
    Network::readDataNumber(data, position, y);
}

float Point::distSqr(Point& o){
    return (SQUARE(abs(x-o.x)) + SQUARE(abs(y-o.y)));
}