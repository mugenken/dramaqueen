#include <iostream>
#include <thread>

#include <sys/types.h>
#include <dirent.h>
#include <pwd.h>

#include "server/BaseServer.hpp"
#include "client/BaseClient.hpp"
#include "logger/Logger.hpp"
#include "config/Config.hpp"
#include "bot/Bot.hpp"
#include "bot/Daemon.hpp"
#include "bot/DaemonForge.hpp"

using namespace Dramaqueen;
using namespace gloox;

Client* j = NULL;

void startBot()
{
    Bot bot;
    j = bot.getJ();
    bot.connectToXMPP();
}

void startServer()
{
    BaseServer *server;
    server = new BaseServer();
    server->run();
    delete server;
}

void initDaemonForge( std::string daemonDir, Client* _j )
{
    sleep( 5 );
    DIR* dp = opendir( daemonDir.c_str() );
    while( true )
    {
        struct dirent* dir = readdir( dp );
        if( dir == NULL )
        {
            break;
        }
        std::string currentFile = dir->d_name;
        if( currentFile.find( "." ) == 0 )
        {
            Logger::getSingletonPtr()->log( "daemon skips file: ", currentFile );
            Logger::getSingletonPtr()->log( "daemon config files must not begin with a dot" );
            continue;
        }
        if( currentFile.find( ".lua" ) < currentFile.npos )
        {
            std::thread daemonThread{ DaemonForge( currentFile, _j ) };
            daemonThread.detach();
        }
        else
        {
            Logger::getSingletonPtr()->log( "cannot add daemon for: ", currentFile );
            Logger::getSingletonPtr()->log( "daemon config files must have a lua suffix" );
        }
    }
    closedir( dp );
}

void dropRights()
{
    passwd *pw = getpwnam( Config::getSingletonPtr()->getUser().c_str() );
    if( getuid() == 0)
    {
        if( setgid( pw->pw_gid ) != 0 )
            Logger::getSingletonPtr()->log( "setgid: Unable to drop group privileges: ", strerror( errno ) );
        if( setuid( pw->pw_uid ) != 0 )
            Logger::getSingletonPtr()->log( "setuid: Unable to drop user privileges: ", strerror( errno ) );
    }
}

int main( int argc, char **argv )
{
    int opt = 0;
    std::string logDest  = "log/dev.log";
    std::string confFile = "/home/roa/programming/dramaqueen/config/init.lua";

    if( argc < 3 )
    {
        std::cerr << "need log and config" << std::endl;
        exit( 0 );
    }

    while( ( opt = getopt( argc, argv, "c:l:") ) != -1)
    {
        switch( opt )
        {
            case 'c':
            {
                confFile = optarg;
                break;
            }
            case 'l':
            {
                logDest = optarg;
                break;
            }
            default:
            {
                /**
                TODO:
                do something useful
                **/
            }
        }
    }
    Logger::getSingletonPtr( logDest )->log( "initialized dramaqueen...");
    Config* config = Config::getSingletonPtr( confFile );

    dropRights();

    if( config->getXmpp() )
    {
        std::thread botThread( startBot );

        while( j == NULL )
        {
            /**
            TODO: abort after some time
            **/
            sleep( 1 );
        }
        Logger::getSingletonPtr()->log( "initialized bot..." );

        initDaemonForge( config->getDaemonDir(), j );

        std::thread srvThread( startServer );

        srvThread.join();
        botThread.join();
    }
    else
    {
        std::thread srvThread( startServer );
        srvThread.join();
    }

    return 0;
}
