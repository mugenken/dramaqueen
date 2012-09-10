/**
    the bot provides the xmpp interface.
    it get all authentication details from the config singleton.
    the xmpp interface is used a notification system and to
    trigger commands on connected nodes.
**/

#ifndef DRAMAQUEEN_BOT_HPP
#define DRAMAQUEEN_BOT_HPP

#include <iostream>
#include <string.h>
#include <stdio.h>
#include <thread>
#include <vector>

#include <gloox/client.h>
#include <gloox/loghandler.h>
#include <gloox/messagehandler.h>
#include <gloox/connectionlistener.h>
#include <gloox/message.h>

#include "../../config/Config.hpp"
#include "../client/BaseClient.hpp"
#include "../../helper/Helper.hpp"

using namespace gloox;

namespace Dramaqueen
{

class Bot : public MessageHandler, ConnectionListener
{

public:
    Bot();
    ~Bot();

    void connectToXMPP();
    Client* getJ();
    ConnectionError* getCE();

private:
    Client* j;
    ConnectionError ce;

    void initXMPP();
    virtual void onConnect();
    virtual void onDisconnect( ConnectionError e );
    virtual bool onTLSConnect( const CertInfo& info );
    virtual void handleMessage( const Message& stanza, MessageSession* session = 0 );

    std::string contactHosts( std::string command );
    bool checkRecipient( std::string from );

};

}
#endif
