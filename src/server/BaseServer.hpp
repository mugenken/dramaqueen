#ifndef DRAMAQUEEN_BASESERVER_HPP
#define DRAMAQUEEN_BASESERVER_HPP

#include <openssl/bio.h>
#include <openssl/ssl.h>
#include <stdlib.h>
#include <sys/time.h>
#include <sys/types.h>
#include <unistd.h>
#include <iostream>
#include <errno.h>
#include <sstream>
#include <pwd.h>

namespace Dramaqueen
{

class BaseServer
{
private:
    SSL_CTX* ctx;
    char *cert;
    char *key;
    BIO *abio;
    BIO *client;
    SSL *ssl;
    char *host;
    passwd *pw;

    void dropRights();
public:
    BaseServer();
    ~BaseServer();
    void drama();

};

}

#endif
