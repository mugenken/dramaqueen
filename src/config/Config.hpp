/**
    the config object loads the config specified as cli arg.
    the config is a lua config file.
    after all value have been initialized,
    the config object supplies several getter on the public interface.

    the config object is a global unique object ( singletonobject ).
**/

#ifndef DRAMAQUEEN_CONFIG_HPP
#define DRAMAQUEEN_CONFIG_HPP

#include <lua.hpp>
#include <iostream>
#include <vector>

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
    std::string getScriptDir();
    std::string getLogDest();
    std::string getSharedSecret();
    std::vector<std::string> * getForeignHosts();
    std::vector<std::string> * getAuthorizedUsers();

private:
    Config();
    Config( std::string );
    Config( const Config& );
    ~Config();

    void load( const char* fname );

    static Config* singletonPtr;
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
    std::string scriptDir;
    std::string logDest;
    std::string sharedSecret;
    std::vector<std::string>* foreignHosts;
    std::vector<std::string>* authorizedUsers;

};

}
#endif
