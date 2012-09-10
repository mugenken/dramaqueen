#include "Bot.hpp"

using namespace gloox;

namespace Dramaqueen
{

Bot::Bot()
{
    ce = ConnNoError;
    initXMPP();
}

Bot::~Bot()
{
    delete j;
    j = NULL;
    Helper::log( "Bot: an error occured: shutting down bot" );
}

void Bot::connectToXMPP()
{
    if( j->connect( false ) )
    {
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

ConnectionError* Bot::getCE()
{
    return &ce;
}

void Bot::initXMPP()
{
    JID jid( Config::getSingletonPtr()->getXmppUser() );

    j = new Client( jid, Config::getSingletonPtr()->getXmppPasswd() );
    j->registerConnectionListener( this );
    j->registerMessageHandler( this );
    //j->setServer( "talk.google.com" );
    Helper::log( "initialized bot..." );
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
    if( stanza.body().empty() )
    {
        return;
    }
    Message::MessageType type = Message::MessageType::Chat;

    if( checkRecipient( stanza.from().bare() ) )
    {
        Message msg( type, stanza.from(), contactHosts( stanza.body() ) );
        j->send( msg );
    }
    else
    {
        Message msg( type, stanza.from(), "You are not in the recipients list. Contact your admin." );
        j->send( msg );
    }
}

std::string Bot::contactHosts( std::string command )
{
    std::string results;
    std::vector<std::string> * foreignHosts = Config::getSingletonPtr()->getForeignHosts();
    for( std::vector<std::string>::iterator it = foreignHosts->begin(); it != foreignHosts->end(); ++it )
    {
        std::string currentHost = *it;
        /**
            TODO:
            parallelize it!
        **/
        BaseClient baseclient( currentHost, command );
        results.append( baseclient.run() );
    }
    if( results.empty() )
    {
        results.append( "no hits" );
    }
    return results;
}

bool Bot::checkRecipient( std::string from )
{
    std::vector<std::string> * authorizedUsers = Config::getSingletonPtr()->getAuthorizedUsers();
    for( std::vector<std::string>::iterator it = authorizedUsers->begin(); it != authorizedUsers->end(); ++it )
    {
        std::string authorizedUser = *it;
        if( from.find( authorizedUser ) == 0 && from.size() == authorizedUser.size() )
        {
            return true;
        }
    }
    return false;
}

}
