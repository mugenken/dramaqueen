#include "NewsForge.hpp"

namespace Dramaqueen
{

NewsForge::NewsForge()
{
    Helper::log( "initializing message system..." );
    sleep( 5 );
}

NewsForge::~NewsForge()
{
    Helper::log( "shutting down message system..." );
}

void NewsForge::operator() ()
{
    initNews();
}

void NewsForge::initDaemonForge( std::string daemonDir, Client* _j, ConnectionError* ce )
{
    DIR* dp = opendir( daemonDir.c_str() );
    std::vector<std::string> daemonList;

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
            Helper::log( "DaemonForge: daemon names must not start with a dot: ", currentFile );
            continue;
        }
        if( currentFile.find( ".lua" ) < currentFile.npos )
        {
            daemonList.push_back( currentFile );
        }
        else
        {
            Helper::log( "DaemonForge: by convention daemon names must have \".lua\"-suffix - not starting:", currentFile );
        }
    }

    std::vector<std::thread> threads( daemonList.size() );
    int count = 0;
    for( std::vector<std::thread>::iterator it = threads.begin(); it != threads.end(); ++it )
    {
        *it = std::thread{ DaemonForge( daemonList.at( count ), _j, ce ) };
        ++count;
    }
    for( auto&& i : threads )
    {
        i.join();
    }
    closedir( dp );
}

void NewsForge::initBot()
{
    bot = new Bot();
    j = bot->getJ();
    ce = bot->getCE();
    Helper::log( "MessageSystem: initialized bot" );
}

void NewsForge::startBot()
{
    Helper::log( "MessageSystem: starting bot" );
    bot->connectToXMPP();
    Helper::log( "MessageSystem: bot stopped working" );
}

void NewsForge::destroyBot()
{
    j = NULL;
    ce = NULL;
    delete bot;
    Helper::log( "destoryed bot" );
}

void NewsForge::initNews()
{
    while( true )
    {
        Helper::log( "MessageSystem: entering main loop" );
        int i = 0;
        bool initDaemon = false;
        initBot();
        std::thread botThread( &NewsForge::startBot, *this );
        while( i++ < 3 )
        {
            sleep( 5 );

            if( ! ( j == NULL || ce == NULL ) )
            {
                initDaemon = true;
                break;
            }

        }
        if( initDaemon )
        {
            initDaemonForge( Config::getSingletonPtr()->getDaemonDir(), j, ce );
        }
        botThread.join();
        destroyBot();
    }
}

}
