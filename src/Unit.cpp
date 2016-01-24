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
    readUid(data, position);
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

float Unit::speedMod(){
    float mod = 1;
    switch (formation) {
        case formationDefault:
            mod = 1;
            break;
        case formationUnformed:
            mod = 2;
            break;

        default:
            break;
    }
    return mod;
}

void Unit::update(){


    float speedRot = baseSpeedRot * speedMod();
    float speedMove = baseSpeedMove * speedMod();

    if(x != tx || y != ty || angle != tAngle){

        float dir = Math::mod(((tAngle - angle) + PI), TAU) - PI;

        if(angle != tAngle){
            if(abs(tAngle - angle) <= speedRot){
                angle = tAngle;
            }else{
                angle = Math::normalizeAngle(angle+(dir>0?speedRot:-speedRot));
            }
        }

        float xDif = x-tx;
        float yDif = y-ty;

        if(abs(xDif) <= speedMove && abs(yDif) <= speedMove){
            x = tx;
            y = ty;
        }

        if(x != tx || y != ty){
            x += speedMove * cos(angle);
            y += speedMove * sin(angle);
            tAngle = Math::normalizeAngle(atan2(ty-y, tx-x));
        }

        if(server && x == tx && y == ty && angle == tAngle){
            server->sendPacketToAll(PACKET_TC_UNIT_TARGET_REACHED, this);
        }

    }


    if(statsChanged){
        statsChanged = false;
        if(server){
        	server->sendPacketToAll(PACKET_TC_UNIT_STATS_UPDATE, this);
        }else if(client){
            client->sendPacket(PACKET_TS_UNIT_STATS_UPDATE, this);
        }
    }



}

void Unit::render(){
    Graphics::drawImage(client->window, viewOffsetX+x-(width/2), viewOffsetY+y-(height/2), width, height, image, &clip);
}

void Unit::writeAllData(vector<unsigned char>& data){
    writePosData(data);
    writeTargetData(data);
    writeStatsData(data);

    Network::addDataNumber(data, baseSpeedMove);
    Network::addDataNumber(data, baseSpeedRot);
    Network::addDataNumber(data, width);
    Network::addDataNumber(data, height);
    Network::addDataNumber(data, maxCount);
}

void Unit::readAllData(unsigned char* data, size_t& position){
    readPosData(data, position);
    readTargetData(data, position);
    readStatsData(data, position);
    
    Network::readDataNumber(data, position, baseSpeedMove);
    Network::readDataNumber(data, position, baseSpeedRot);
    Network::readDataNumber(data, position, width);
    Network::readDataNumber(data, position, height);
    Network::readDataNumber(data, position, maxCount);

}

void Unit::writeUid(vector<unsigned char>& data){
    Network::addDataNumber(data, uid);
}

void Unit::readUid(unsigned char* data, size_t& position){
    Network::readDataNumber(data, position, uid);
}

void Unit::writeStatsData(vector<unsigned char>& data){
    Network::addDataNumber(data, formation);
    Network::addDataNumber(data, count);
    Network::addDataNumber(data, morale);
    Network::addDataNumber(data, attackPotential);
}

void Unit::readStatsData(unsigned char* data, size_t& position){
    Network::readDataNumber(data, position, formation);
    Network::readDataNumber(data, position, count);
    Network::readDataNumber(data, position, morale);
    Network::readDataNumber(data, position, attackPotential);
}

void Unit::writeTargetData(vector<unsigned char>& data){
    Network::addDataNumber(data, tx);
    Network::addDataNumber(data, ty);
    Network::addDataNumber(data, tAngle);
}

void Unit::readTargetData(unsigned char* data, size_t& position){
    Network::readDataNumber(data, position, tx);
    Network::readDataNumber(data, position, ty);
    Network::readDataNumber(data, position, tAngle);
}

void Unit::writePosData(vector<unsigned char>& data){
    Network::addDataNumber(data, x);
    Network::addDataNumber(data, y);
    Network::addDataNumber(data, angle);
}

void Unit::readPosData(unsigned char* data, size_t& position){
    Network::readDataNumber(data, position, x);
    Network::readDataNumber(data, position, y);
    Network::readDataNumber(data, position, angle);
}



