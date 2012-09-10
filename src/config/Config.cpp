#include "Config.hpp"

namespace Dramaqueen
{

Config* Config::singletonPtr = 0;

Config * Config::getSingletonPtr()
{
    while( ! singletonPtr )
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
}

Config::Config( std::string config )
{
    foreignHosts    = new std::vector<std::string>;
    authorizedUsers = new std::vector<std::string>;
    load( config.c_str() );
}

Config::~Config()
{
    delete foreignHosts;
    delete authorizedUsers;
    foreignHosts = NULL;
    authorizedUsers = NULL;
}

void Config::load( const char* fname )
{
    /**
        TODO
        Log failures on loading!
    **/
    L = luaL_newstate();
    if( luaL_loadfile( L, fname ) || lua_pcall( L, 0, 0, 0 ) )
    {
        std::cerr << "could not load config file" << std::endl;
    }
    /**********************
     *    load user       *
     **********************/
    lua_getglobal( L, "user" );
    if( !lua_isstring( L, 1 ) )
    {
    }
    else
    {
        user = lua_tostring( L, 1 );
    }
    lua_pop( L, 1 );

    /**********************
     *    load group      *
     **********************/
    lua_getglobal( L, "group" );
    if( !lua_isstring( L, 1 ) )
    {
    }
    else
    {
        group = lua_tostring( L, 1 );
    }
    lua_pop( L, 1 );

    /**********************
     *    load xmppUser   *
     **********************/
    lua_getglobal( L, "xmppUser" );
    if( !lua_isstring( L, 1 ) )
    {
    }
    else
    {
        xmppUser = lua_tostring( L, 1 );
    }
    lua_pop( L, 1 );

    /**********************
     *    load xmppPasswd *
     **********************/
    lua_getglobal( L, "xmppPasswd" );
    if( !lua_isstring( L, 1 ) )
    {
    }
    else
    {
        xmppPasswd = lua_tostring( L, 1 );
    }
    lua_pop( L, 1 );

    /**********************
     *  load xmpp boolian *
     **********************/
    lua_getglobal( L, "xmpp" );
    if( !lua_isstring( L, 1 ) )
    {
    }
    else
    {
        xmpp = lua_tostring( L, 1 );
    }
    lua_pop( L, 1 );

    /**********************
     *  load bind address *
     **********************/
    lua_getglobal( L, "bind" );
    if( !lua_isstring( L, 1 ) )
    {
    }
    else
    {
        bind = lua_tostring( L, 1 );
    }
    lua_pop( L, 1 );

    /**********************
     *  load ssl cert     *
     **********************/
    lua_getglobal( L, "sslCert" );
    if( !lua_isstring( L, 1 ) )
    {
    }
    else
    {
        sslCert = lua_tostring( L, 1 );
    }
    lua_pop( L, 1 );

    /**********************
     *  load ssl key      *
     **********************/
    lua_getglobal( L, "sslKey" );
    if( !lua_isstring( L, 1 ) )
    {
    }
    else
    {
        sslKey = lua_tostring( L, 1 );
    }
    lua_pop( L, 1 );

    /**********************
     *  load daemondir    *
     **********************/
    lua_getglobal( L, "daemonDir" );
    if( !lua_isstring( L, 1 ) )
    {
    }
    else
    {
        daemonDir = lua_tostring( L, 1 );
    }
    lua_pop( L, 1 );

    /**********************
     *  load scriptdir    *
     **********************/
    lua_getglobal( L, "scriptDir" );
    if( !lua_isstring( L, 1 ) )
    {
    }
    else
    {
        scriptDir = lua_tostring( L, 1 );
    }
    lua_pop( L, 1 );

    /**********************
     *  load logDest      *
     **********************/
    lua_getglobal( L, "logDest" );
    if( !lua_isstring( L, 1 ) )
    {
    }
    else
    {
        logDest = lua_tostring( L, 1 );
    }
    lua_pop( L, 1 );

    /**********************
     *  load sharedSecret *
     **********************/
    lua_getglobal( L, "sharedSecret" );
    if( !lua_isstring( L, 1 ) )
    {
    }
    else
    {
        sharedSecret = lua_tostring( L, 1 );
    }
    lua_pop( L, 1 );

    /***********************
     *  load foreign Hosts *
     ***********************/
    lua_getglobal( L, "foreignHosts" );
    if( !lua_istable( L, 1 ) )
    {
    }
    else
    {
        lua_pushnil( L );

        while( lua_next( L, 1 ) != 0 )
        {
            foreignHosts->push_back( lua_tostring( L, -1 ) );
            lua_pop( L, 1 );
        }

        for( std::vector<std::string>::iterator it = foreignHosts->begin(); it != foreignHosts->end(); ++it )
        {
            std::string foreignHost = *it;
        }
    }
    lua_pop( L, 1 );

    /***********************
     *load authorizedUsers *
     ***********************/
    lua_getglobal( L, "authorizedUsers" );
    if( !lua_istable( L, 1 ) )
    {
    }
    else
    {
        lua_pushnil( L );

        while( lua_next( L, 1 ) != 0 )
        {
            authorizedUsers->push_back( lua_tostring( L, -1 ) );
            lua_pop( L, 1 );
        }

        for( std::vector<std::string>::iterator it = authorizedUsers->begin(); it != authorizedUsers->end(); ++it )
        {
            std::string authorizedUser = *it;
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

std::string Config::getSSLCert()
{
    return sslCert;
}

std::string Config::getSSLKey()
{
    return sslKey;
}

std::string Config::getDaemonDir()
{
    return daemonDir;
}

std::string Config::getScriptDir()
{
    return scriptDir;
}

std::string Config::getLogDest()
{
    return logDest;
}

std::string Config::getSharedSecret()
{
    return sharedSecret;
}

std::vector<std::string> * Config::getForeignHosts()
{
    return foreignHosts;
}

std::vector<std::string> * Config::getAuthorizedUsers()
{
    return authorizedUsers;
}

}
