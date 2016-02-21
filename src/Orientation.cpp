//
//  Orientation.cpp
//  Strong
//
//  Created by Braeden Atlee on 2/18/16.
//  Copyright © 2016 Braeden Atlee. All rights reserved.
//

#include "Orientation.hpp"
#include "Network.hpp"


void Orientation::write(vector<unsigned char>& data){
    Point::write(data);
    Network::addDataNumber(data, a);
}

void Orientation::read(unsigned char* data, size_t& position){
    Point::read(data, position);
    Network::readDataNumber(data, position, a);
}