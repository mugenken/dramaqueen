#ifndef DRAMAQUEEN_CONFIG_HPP
#define DRAMAQUEEN_CONFIG_HPP

#include <lua.hpp>
#include <iostream>
#include <vector>
#include "../logger/Logger.hpp"

namespace Dramaqueen
{

class Config
{

public:
    static Config* getSingletonPtr();
    static Config* getSingletonPtr( std::string config );

    std::string getUser();
    std::string getGroup();
    std::string getXmppUser();
    std::string getXmppPasswd();
    bool getXmpp();
    std::string getBind();
    std::string getSSLCert();
    std::string getSSLKey();
    std::string getDaemonDir();
    std::vector<std::string> * getForeignHosts();

private:
    Config();
    Config( std::string );
    Config( const Config& );
    ~Config();

    void load( const char* fname );

    static Config* singletonPtr;
    Logger *logger;
    lua_State *L;

    std::string user;
    std::string group;
    std::string xmppUser;
    std::string xmppPasswd;
    std::string xmpp;
    std::string bind;
    std::string sslCert;
    std::string sslKey;
    std::string daemonDir;
    std::vector<std::string>* foreignHosts;

};

}
#endif
