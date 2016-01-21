//
//  Debug.cpp
//  Strong
//
//  Created by Braeden Atlee on 1/20/16.
//  Copyright © 2016 Braeden Atlee. All rights reserved.
//

#include "Debug.hpp"
#include "Global.hpp"
#include "Client.hpp"
#include "Server.hpp"


void debugf(string format, ...){
    char buff[256];
    va_list args;
    va_start(args, format);
    vsprintf(buff, format.c_str(), args);
    printf("- %s\n", buff);
    va_end(args);
}

void errorf(string format, ...){
    char buff[256];
    va_list args;
    va_start(args, format);
    vsprintf(buff, format.c_str(), args);
    printf("! %s\n", buff);
    va_end(args);
}