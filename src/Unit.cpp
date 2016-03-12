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
    orientTarget.x = nx;
    orientTarget.y = ny;
    orientTarget.a = Math::normalizeAngle(atan2(orientTarget.y - orientNow.y, orientTarget.x - orientNow.x));

    orientSubTarget = orientTarget;
}

void Unit::setTargetAngle(float nAngle){
    orientTarget.a = Math::normalizeAngle(nAngle);
    orientSubTarget.a = orientTarget.a;
}

void Unit::teleport(float x, float y){
    orientNow.x = x;
    orientNow.y = y;
    orientTarget = orientNow;
    orientSubTarget = orientNow;
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

    if(orientNow != orientSubTarget){

        if(abs(orientNow.a - orientSubTarget.a) < speedRot){
            orientNow.a = orientSubTarget.a;
            orientSubTarget.a = Math::normalizeAngle(atan2(orientSubTarget.y - orientNow.y, orientSubTarget.x - orientNow.x));
        }else{
            float dir = Math::mod(((orientSubTarget.a - orientNow.a) + PI), TAU) - PI;
            orientNow.a = Math::normalizeAngle(orientNow.a+(dir>0?speedRot:-speedRot));
        }

        if(abs(orientNow.x - orientSubTarget.x) < speedMove && abs(orientNow.y - orientSubTarget.y) < speedMove){
            orientNow.x = orientSubTarget.x;
            orientNow.y = orientSubTarget.y;
        }else{

            for(pair<UID, Unit*> p: general->units){
                if(p.first != uid){
                    if(SQUARE((collisionRadius + p.second->collisionRadius)) < orientNow.distSqr(p.second->orientNow)){
                        orientSubTarget.a = Math::normalizeAngle(((Math::mod(((orientSubTarget.a - orientNow.a) + PI), TAU) - PI) / 2) + PI);
                    }
                }
            }

            orientNow.x += speedMove * cos(orientNow.a);
            orientNow.y += speedMove * sin(orientNow.a);

        }



    }else if(orientSubTarget != orientTarget){
        orientSubTarget = orientTarget;
    }else{
        if(server){
            server->sendPacketToAll(PACKET_TC_UNIT_TARGET_REACHED, this);
        }
    }




    /*float speedRot = baseSpeedRot * speedMod();
    float speedMove = baseSpeedMove * speedMod();

    Unit* moveAway = nullptr;
    float dist = 0;

    for(pair<UID, Unit*> p: general->units){
        if(p.first == uid){
            continue;
        }
        if(p.first < uid){//one way so there isn't duplacate updates

            dist = SQUARE(collisionRadius + p.second->collisionRadius) - Math::distanceSquared(x, y, p.second->x, p.second->y);
            
            if(p.second->general->uid != general->uid){ //different team

            }else{//same team
                if(!moveAway){
                    if(dist >= 0){
                        moveAway = p.second;
                        if(!movingOutOfTheWay){
                            movingOutOfTheWay = true;
                            ttAngle = tAngle;
                            ttx = tx;
                            tty = ty;
                        }
                    }
                }
            }
        }
    }

    if(movingOutOfTheWay){
        if(moveAway){
            tAngle = Math::normalizeAngle(atan2(moveAway->y-y, moveAway->x-x)+PI);
            tx = x + (cos(tAngle) * (collisionRadius + moveAway->collisionRadius));
            ty = y + (sin(tAngle) * (collisionRadius + moveAway->collisionRadius));
        }else{
            tAngle = ttAngle;
            tx = ttx;
            ty = tty;
            movingOutOfTheWay = false;
        }
    }

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

        if((x != tx || y != ty) && (angle == tAngle || Math::distanceSquared(x, y, tx, ty) > SQUARE((TAU/speedRot)*speedMove/TAU))){
            x += speedMove * cos(angle);
            y += speedMove * sin(angle);
            tAngle = Math::normalizeAngle(atan2(ty-y, tx-x));
        }

        if(server && x == tx && y == ty && angle == tAngle){
            server->sendPacketToAll(PACKET_TC_UNIT_TARGET_REACHED, this);
        }

    }*/


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
    Graphics::drawImage(client->window, viewOffsetX+orientNow.x-(width/2), viewOffsetY+orientNow.y-(height/2), width, height, image, &clip);
}

void Unit::writeAllData(vector<unsigned char>& data){
    writePosData(data);
    writeTargetData(data);
    writeStatsData(data);

    Network::addDataNumber(data, baseSpeedMove);
    Network::addDataNumber(data, baseSpeedRot);
    Network::addDataNumber(data, width);
    Network::addDataNumber(data, height);
    Network::addDataNumber(data, collisionRadius);
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
    Network::readDataNumber(data, position, collisionRadius);
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
    orientTarget.write(data);
}

void Unit::readTargetData(unsigned char* data, size_t& position){
    orientTarget.read(data, position);
}

void Unit::writePosData(vector<unsigned char>& data){
    orientNow.write(data);
    orientSubTarget.write(data);
}

void Unit::readPosData(unsigned char* data, size_t& position){
    orientNow.read(data, position);
    orientSubTarget.read(data, position);
}



