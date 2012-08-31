#ifndef DRAMAQUEEN_DROPPRIVS_HPP
#define DRAMAQUEEN_DROPPRIVS_HPP

#include <sys/types.h>
#include <pwd.h>
#include <string.h>

#include <iostream>
#include <fstream>
#include <sstream>

#include "../config/Config.hpp"

namespace Dramaqueen
{

namespace Helper
{
    void dropPrivs();
    void log( std::string logMsg );
    void log( std::string logMsg, std::string logMsg2 );
    void log( std::string logMsg, std::string logMsg2, std::string logMsg3 );
    std::string createTimestamp();
}

}

#endif
