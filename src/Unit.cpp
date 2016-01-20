//
//  Unit.cpp
//  Strong
//
//  Created by Braeden Atlee on 1/19/16.
//  Copyright © 2016 Braeden Atlee. All rights reserved.
//

#include "Unit.hpp"
#include "Network.hpp"


Unit::Unit(General* general){
    this->general = general;
}

Unit::Unit(General* general, unsigned char* data, size_t& position){
    this->general = general;
    readData(data, position);
}

void Unit::update(){
    x += xs;
    y += ys;
}

void Unit::render(){
    
}

void Unit::writeData(vector<unsigned char>& data){
    Network::addDataNumber(data, x);
    Network::addDataNumber(data, y);
    Network::addDataNumber(data, xs);
    Network::addDataNumber(data, ys);
    Network::addDataNumber(data, angle);
}

void Unit::readData(unsigned char* data, size_t& position){
    Network::readDataNumber(data, position, x);
    Network::readDataNumber(data, position, y);
    Network::readDataNumber(data, position, xs);
    Network::readDataNumber(data, position, ys);
    Network::readDataNumber(data, position, angle);
}

