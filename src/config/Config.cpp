#include "Config.hpp"

namespace Dramaqueen
{

Config* Config::singletonPtr = 0;

Config * Config::getSingletonPtr()
{
/*
    if ( !singletonPtr )
    {
        singletonPtr = new Config();
    }
*/
    while( !singletonPtr )
    {
        sleep( 1 );
    }
    return singletonPtr;
}

Config * Config::getSingletonPtr( std::string config )
{
    if ( !singletonPtr )
    {
        singletonPtr = new Config( config );
    }
    return singletonPtr;
}

Config::Config()
{
    logger = Logger::getSingletonPtr();
}

Config::Config( std::string config )
{
    logger = Logger::getSingletonPtr();
    load( config.c_str() );
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
     *    load user       *
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

    /**********************
     *    load group      *
     **********************/
    lua_getglobal( L, "group" );
    if( !lua_isstring( L, 1 ) )
    {
        logger->log( "group is not a string" );
    }
    else
    {
        group = lua_tostring( L, 1 );
        logger->log( "set group to: ", group );
    }
    lua_pop( L, 1 );

    /**********************
     *    load xmppUser   *
     **********************/
    lua_getglobal( L, "xmppUser" );
    if( !lua_isstring( L, 1 ) )
    {
        logger->log( "xmppUser is not a string" );
    }
    else
    {
        xmppUser = lua_tostring( L, 1 );
        logger->log( "set xmppUser to: ", xmppUser );
    }
    lua_pop( L, 1 );

    /**********************
     *    load xmppPasswd *
     **********************/
    lua_getglobal( L, "xmppPasswd" );
    if( !lua_isstring( L, 1 ) )
    {
        logger->log( "xmppPasswd is not a string" );
    }
    else
    {
        xmppPasswd = lua_tostring( L, 1 );
        logger->log( "set xmppPasswd to: ", xmppPasswd );
    }
    lua_pop( L, 1 );

    /**********************
     *  load xmpp boolian *
     **********************/
    lua_getglobal( L, "xmpp" );
    if( !lua_isstring( L, 1 ) )
    {
        logger->log( "xmpp is not a string" );
    }
    else
    {
        xmpp = lua_tostring( L, 1 );
        logger->log( "set xmpp to: ", xmpp );
    }
    lua_pop( L, 1 );

    /**********************
     *  load bind address *
     **********************/
    lua_getglobal( L, "bind" );
    if( !lua_isstring( L, 1 ) )
    {
        logger->log( "bind is not a string" );
    }
    else
    {
        bind = lua_tostring( L, 1 );
        logger->log( "set bind to: ", bind );
    }
    lua_pop( L, 1 );

     /**********************
     *  load foreign Hosts *
     **********************/
    lua_getglobal( L, "foreignHosts" );
    if( !lua_istable( L, 1 ) )
    {
        logger->log( "foreignHosts is not a table" );
    }
    else
    {
        foreignHosts = new std::vector<std::string>;
        lua_pushnil( L );

        while( lua_next( L, 1 ) != 0 )
        {
            foreignHosts->push_back( lua_tostring( L, -1 ) );
            lua_pop( L, 1 );
        }

        for( std::vector<std::string>::iterator it = foreignHosts->begin(); it != foreignHosts->end(); ++it )
        {
            std::string foreignHost = *it;
            logger->log( "added host: ", foreignHost );
        }
    }


    lua_close( L );
}

std::string Config::getUser()
{
    return user;
}

std::string Config::getGroup()
{
    return group;
}

std::string Config::getXmppUser()
{
    return xmppUser;
}

std::string Config::getXmppPasswd()
{
    return xmppPasswd;
}

bool Config::getXmpp()
{
    return xmpp.at( 0 ) == '1';
}

std::string Config::getBind()
{
    return bind;
}

std::vector<std::string> * Config::getForeignHosts()
{
    return foreignHosts;
}

}
