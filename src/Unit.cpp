//
//  Unit.cpp
//  Strong
//
//  Created by Braeden Atlee on 1/19/16.
//  Copyright © 2016 Braeden Atlee. All rights reserved.
//

#include "Unit.hpp"
#include "Network.hpp"
#include "Graphics.hpp"
#include "Math.hpp"
#include "Server.hpp"

#define WIDTH 12
#define HEIGHT 12
#define HALF_WIDTH (WIDTH/2)
#define HALF_HEIGHT (HEIGHT/2)


#define CLOSE_ENOUGH 1

#define SPEED_ROTATION 2
#define SPEED_MOVEMENT .5

#define client (general->world->client)
#define server (general->world->server)


Unit::Unit(General* general){
    this->general = general;
}

Unit::Unit(General* general, unsigned char* data, size_t& position){
    this->general = general;
    readAllData(data, position);
}

void Unit::setTargetPos(double nx, double ny){
    tx = nx;
    ty = ny;
    tAngle = Math::clampAngle(atan2(ty-y, tx-x));
}

void Unit::setTargetAngle(double nAngle){
    tAngle = Math::clampAngle(nAngle);
}

void Unit::update(){


    if(x != tx || y != ty || angle != tAngle){

        double angleDif = Math::clampAngle(angle - tAngle);

        if(angle != tAngle){
            if(abs(angleDif) <= CLOSE_ENOUGH){
                angle = tAngle;
            }else{
                angle += Math::closestToZero(angleDif, SPEED_ROTATION);
            }
        }

        if(angle == tAngle){
            double xDif = x-tx;
            double yDif = y-ty;

            if(abs(xDif) <= CLOSE_ENOUGH){
                x = tx;
            }else if(x != tx){
                x += SPEED_MOVEMENT * cos(angle);
            }

            if(abs(yDif) <= CLOSE_ENOUGH){
                y = ty;
            }else if(y != ty){
                y += SPEED_MOVEMENT * sin(angle);
            }

            if(x != tx && y != ty){
                tAngle = Math::clampAngle(atan2(ty-y, tx-x));
            }
        }

        if(server && x == tx && y == ty && angle == tAngle){
            server->sendPacketToAll(PACKET_TC_UNIT_TARGET_REACHED, this);
        }

    }



}

void Unit::render(){
    Graphics::drawImage(client->window, x-HALF_WIDTH, y-HALF_HEIGHT, WIDTH, HEIGHT, image, &clip);
}

void Unit::writeAllData(vector<unsigned char>& data){
    Network::addDataNumber(data, uid);
    Network::addDataNumber(data, x);
    Network::addDataNumber(data, y);
    Network::addDataNumber(data, tx);
    Network::addDataNumber(data, ty);
    Network::addDataNumber(data, angle);
    Network::addDataNumber(data, tAngle);
}

void Unit::readAllData(unsigned char* data, size_t& position){
    Network::readDataNumber(data, position, uid);
    Network::readDataNumber(data, position, x);
    Network::readDataNumber(data, position, y);
    Network::readDataNumber(data, position, tx);
    Network::readDataNumber(data, position, ty);
    Network::readDataNumber(data, position, angle);
    Network::readDataNumber(data, position, tAngle);
}

void Unit::writeTargetData(vector<unsigned char>& data){
    Network::addDataNumber(data, uid);
    Network::addDataNumber(data, tx);
    Network::addDataNumber(data, ty);
    Network::addDataNumber(data, tAngle);
}

void Unit::readTargetData(unsigned char* data, size_t& position){
    //Reads uid before
    Network::readDataNumber(data, position, tx);
    Network::readDataNumber(data, position, ty);
    Network::readDataNumber(data, position, tAngle);
}

void Unit::writePosData(vector<unsigned char>& data){
    Network::addDataNumber(data, uid);
    Network::addDataNumber(data, x);
    Network::addDataNumber(data, y);
    Network::addDataNumber(data, angle);
}

void Unit::readPosData(unsigned char* data, size_t& position){
    //Reads uid before
    Network::readDataNumber(data, position, x);
    Network::readDataNumber(data, position, y);
    Network::readDataNumber(data, position, angle);
}



