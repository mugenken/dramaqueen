#include "Bot.hpp"

using namespace gloox;

namespace Dramaqueen
{

Bot::Bot()
{
    initXMPP();
}

Bot::~Bot()
{
    delete j;
}

void Bot::connectToXMPP()
{
    if( j->connect( false ) )
    {
        ConnectionError ce = ConnNoError;
        while( ce == ConnNoError )
        {
          ce = j->recv();
        }
    }
}

Client* Bot::getJ()
{
    return j;
}

void Bot::initXMPP()
{
    JID jid( Config::getSingletonPtr()->getXmppUser() );

    j = new Client( jid, Config::getSingletonPtr()->getXmppPasswd() );

    j->registerConnectionListener( this );
    j->registerMessageHandler( this );
    j->logInstance().registerLogHandler( LogLevelDebug, LogAreaAll, this );
}

void Bot::onConnect()
{

}

void Bot::onDisconnect( ConnectionError e )
{

}

bool Bot::onTLSConnect( const CertInfo& info )
{
    return true;
}

void Bot::handleMessage( const Message& stanza, MessageSession* session )
{
    if( !stanza.body().empty() )
    {
        Message::MessageType type = Message::MessageType::Chat;
        Message msg( type, stanza.from(), contactHosts( stanza.body() ) );
        j->send( msg );
    }
}

void Bot::handleLog( LogLevel level, LogArea area, const std::string& message )
{
    //printf("log: level: %d, area: %d, %s\n", level, area, message.c_str() );
}

std::string Bot::contactHosts( std::string command )
{
    std::string results;
    std::vector<std::string> * foreignHosts = Config::getSingletonPtr()->getForeignHosts();
    for( std::vector<std::string>::iterator it = foreignHosts->begin(); it != foreignHosts->end(); ++it )
    {
        std::string currentHost = *it;
        BaseClient baseclient( currentHost, command );
        results.append( baseclient.run() );
    }
    if( results.empty() )
    {
        results.append( "no hits" );
    }
    return results;
}

}
