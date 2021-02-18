//
//  DreamLib.cpp
//  DreamLib
//
//  Created by Marc Fervil on 2/17/21.
//

#include <iostream>
#include "DreamLib.hpp"
#include "DreamLibPriv.hpp"

void DreamLib::HelloWorld(const char * s)
{
    DreamLibPriv *theObj = new DreamLibPriv;
    theObj->HelloWorldPriv(s);
    delete theObj;
};

void DreamLibPriv::HelloWorldPriv(const char * s) 
{
    std::cout << s << std::endl;
};

