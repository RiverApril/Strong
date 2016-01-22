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
#include "MenuGame.hpp"

#define viewOffsetX (((MenuGame*)client->window->currentMenu)->viewOffsetX)
#define viewOffsetY (((MenuGame*)client->window->currentMenu)->viewOffsetY)



#define client (general->world->client)
#define server (general->world->server)


Unit::Unit(General* general){
    this->general = general;
}

Unit::Unit(General* general, unsigned char* data, size_t& position){
    this->general = general;
    readAllData(data, position);
}

void Unit::setTargetPos(float nx, float ny){
    tx = nx;
    ty = ny;
    tAngle = Math::normalizeAngle(atan2(ty-y, tx-x));
}

void Unit::setTargetAngle(float nAngle){
    tAngle = Math::normalizeAngle(nAngle);
}

void Unit::update(){


    if(x != tx || y != ty || angle != tAngle){

        float dir = Math::mod(((tAngle - angle) + PI), TAU) - PI;

        if(angle != tAngle){
            if(abs(tAngle - angle) <= speedRot){
                angle = tAngle;
            }else{
                angle = Math::normalizeAngle(angle+(dir>0?speedRot:-speedRot));
            }
        }

        if(angle == tAngle){
            float xDif = x-tx;
            float yDif = y-ty;

            if(abs(xDif) <= speedMove){
                x = tx;
            }

            if(abs(yDif) <= speedMove){
                y = ty;
            }

            if(x != tx || y != ty){
                x += speedMove * cos(angle);
                y += speedMove * sin(angle);
            }

            //if(x != tx || y != ty){
                //tAngle = Math::normalizeAngle(atan2(ty-y, tx-x));
            //}
        }

        if(server && x == tx && y == ty && angle == tAngle){
            server->sendPacketToAll(PACKET_TC_UNIT_TARGET_REACHED, this);
        }

    }



}

void Unit::render(){
    Graphics::drawImage(client->window, viewOffsetX+x-(width/2), viewOffsetY+y-(height/2), width, height, image, &clip);
}

void Unit::writeAllData(vector<unsigned char>& data){
    Network::addDataNumber(data, uid);
    Network::addDataNumber(data, x);
    Network::addDataNumber(data, y);
    Network::addDataNumber(data, tx);
    Network::addDataNumber(data, ty);
    Network::addDataNumber(data, angle);
    Network::addDataNumber(data, tAngle);
    Network::addDataNumber(data, speedMove);
    Network::addDataNumber(data, speedRot);
    Network::addDataNumber(data, width);
    Network::addDataNumber(data, height);
    Network::addDataNumber(data, count);
    Network::addDataNumber(data, morale);
    Network::addDataNumber(data, attackPotential);
}

void Unit::readAllData(unsigned char* data, size_t& position){
    Network::readDataNumber(data, position, uid);
    Network::readDataNumber(data, position, x);
    Network::readDataNumber(data, position, y);
    Network::readDataNumber(data, position, tx);
    Network::readDataNumber(data, position, ty);
    Network::readDataNumber(data, position, angle);
    Network::readDataNumber(data, position, tAngle);
    Network::readDataNumber(data, position, speedMove);
    Network::readDataNumber(data, position, speedRot);
    Network::readDataNumber(data, position, width);
    Network::readDataNumber(data, position, height);
    Network::readDataNumber(data, position, count);
    Network::readDataNumber(data, position, morale);
    Network::readDataNumber(data, position, attackPotential);
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



