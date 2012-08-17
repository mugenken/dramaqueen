#include "Config.hpp"

namespace Dramaqueen
{

Config* Config::singletonPtr = 0;

Config * Config::getSingletonPtr()
{
    if ( !singletonPtr )
    {
        singletonPtr = new Config ();
    }
    return singletonPtr;
}

Config::Config()
{
    logger = Logger::getSingletonPtr();
    load( "/home/roa/programming/dramaqueen/config/init.lua" );
}

Config::~Config()
{

}

void Config::load( const char* fname )
{
    L = luaL_newstate();
    if( luaL_loadfile( L, fname ) || lua_pcall( L, 0, 0, 0 ) )
    {
        logger->log( "cannot run config file: ", lua_tostring( L, -1 ) );
    }

    /**********************
     *    load port  *
     **********************/
    lua_getglobal( L, "user" );
    if( !lua_isstring( L, 1 ) )
    {
        logger->log( "user is not a string" );
    }
    else
    {
        user = lua_tostring( L, 1 );
        logger->log( "set user to: ", user );
    }
    lua_pop( L, 1 );

    lua_close( L );
}

std::string Config::getUser()
{
    return user;
}

}
