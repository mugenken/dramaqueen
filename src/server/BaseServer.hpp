/**
    the baseserver object is started via the serverforge object. it is started in a
    separate thread an shares nothing with the messagesystem initialized by the newsforge.
    if dramaqueen is started with xmpp flag as false, the server thread is the only dramaqueen thread.
    it answers requests from baseclients. you have to ensure, that clients and all nodes share the same
    certs.
    all communication with clients is encrypted with the sslv3 protocoll. the server denies unencrypted
    communication.
**/
#ifndef DRAMAQUEEN_BASESERVER_HPP
#define DRAMAQUEEN_BASESERVER_HPP

#include <openssl/bio.h>
#include <openssl/err.h>
#include <openssl/ssl.h>
#include <stdlib.h>
#include <sys/time.h>
#include <sys/types.h>
#include <unistd.h>
#include <iostream>
#include <errno.h>
#include <sstream>
#include <fstream>
#include "../config/Config.hpp"

namespace Dramaqueen
{

class BaseServer
{

public:
    BaseServer();
    ~BaseServer();
    void run();

private:
    SSL_CTX* ctx;
    std::string cert;
    std::string key;
    std::string host;

    BIO *abio;
    BIO *client;
    SSL *ssl;

    void initServer();
    void handleClient();
    std::string executeScript( std::string script );
    //checks message for sharedsecred
    std::string parseMessage( std::string message );

};

}

#endif
