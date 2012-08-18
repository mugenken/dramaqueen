#include "BaseServer.hpp"

namespace Dramaqueen
{

BaseServer::BaseServer( Client* _j) : j( _j )
{
    config = Config::getSingletonPtr();
    logger = Logger::getSingletonPtr();
}

BaseServer::~BaseServer()
{

}

void BaseServer::dropRights()
{
    pw = getpwnam( config->getUser().c_str() );
    if (getuid() == 0) {
        if (setgid( pw->pw_uid ) != 0)
            logger->log( "setgid: Unable to drop group privileges: ", strerror( errno ) );
        if (setuid( pw->pw_gid ) != 0)
            logger->log( "setuid: Unable to drop user privileges: ", strerror( errno ) );
    }
}

void BaseServer::drama()
{
    dropRights();
    while( true )
    {
        Message::MessageType type = Message::MessageType::Chat;
        std::string helpStr = "Test!";
        std::string roa = "roa@localhost";
        Message msg( type, roa, helpStr );
        j->send( msg );

        sleep(1);
    }
}

}
