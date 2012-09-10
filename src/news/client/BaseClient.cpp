#include "BaseClient.hpp"

namespace Dramaqueen
{

BaseClient::BaseClient( std::string _host, std::string _command ) :
    host( _host ), command( Config::getSingletonPtr()->getSharedSecret().append( _command ) ), shouldRun( true )
{
    initBaseClient();
}

BaseClient::~BaseClient()
{
    ERR_remove_state( 0 );
    destroyBio();
}

std::string BaseClient::run()
{
    if( ! shouldRun )
    {
        Helper::log( "BaseClient: canceling all actions due to previous errors" );
        return "ERROR";
    }
    std::string response;
    response.append( connectToServer() );
    if( !response.empty() )
    {
        Helper::log( "BaseClient: failed to connect to host", host );
        return response;
    }
    response.append( sendToServer() );
    if( !response.empty() )
    {
        Helper::log( "BaseClient: failed to send to host", host );
        return response;
    }

    std::string recv = recvFromServer();
    if( !recv.empty() )
    {
        response.append( intro() );
        response.append( recv.c_str() );
    }
    return response;
}

void BaseClient::initBaseClient()
{
    initCTX();
    initBio();
}

void BaseClient::initCTX()
{
    ctx  = NULL;
    cert = Config::getSingletonPtr()->getSSLCert();
    key  = Config::getSingletonPtr()->getSSLKey();
    ctx = SSL_CTX_new(SSLv3_client_method());
    if( ctx == NULL )
    {
        Helper::log( "BaseClient: failed to create new ctx object" );
        shouldRun = false;
    }
    if( ! SSL_CTX_use_certificate_file( ctx, cert.c_str(), SSL_FILETYPE_PEM ) )
    {
        Helper::log( "BaseClient: failed to load cert file: ", cert );
        shouldRun = false;
    }
    if( ! SSL_CTX_use_PrivateKey_file( ctx, key.c_str(), SSL_FILETYPE_PEM ) )
    {
        Helper::log( "BaseClient: failed to load key file: ", key );
        shouldRun = false;
    }
}

void BaseClient::initBio()
{
    bio = BIO_new_ssl_connect( ctx );

    if( bio == NULL )
    {
        Helper::log( "BaseClient: could not initialize bio to connect to", host );
        shouldRun = false;
    }
    BIO_set_conn_hostname( bio, ( char * )host.c_str() );
}

void BaseClient::destroyBio()
{
    SSL_CTX_free( ctx );
    BIO_free_all( bio );
    ctx = NULL;
    bio = NULL;
}

std::string BaseClient::connectToServer()
{
    std::stringstream answer;
    if( BIO_do_connect( bio ) <= 0 )
    {
        if ( !BIO_set_close( bio, BIO_CLOSE ) )
        {
            Helper::log( "BaseClient: could not set BIO_CLOSE flag while shutting down connection to", host );
        }
        answer  <<  std::endl << intro()
                << "Could not connect to server: "
                <<  host << std::endl;
    }
    return answer.str();
}

std::string BaseClient::sendToServer()
{
    std::stringstream answer;

    if( BIO_write( bio, command.c_str(), command.size() ) <= -1 )
    {
        answer  << std::endl << intro()
                << "send to server "
                << host
                << " failed";
    }

    return answer.str();
}

std::string BaseClient::recvFromServer()
{
    std::string answer = "";
    char rbuffer[4096];
    memset( rbuffer, '\0', sizeof( rbuffer ) );
    int r = -1;
    while( r < 0 )
    {
        r = BIO_read( bio, rbuffer, sizeof( rbuffer ) );
        if( r > 0 )
        {
            answer.append( rbuffer );
        }
        if( r == 0 )
        {
            break;
        }
        if( r < 0 )
        {
            if( !BIO_should_retry( bio ) )
            {
                break;
            }
        }
    }
    return answer;
}

std::string BaseClient::intro()
{
    std::stringstream intro;
    intro << std::endl << "Report from "
          << host      << ":" << std::endl;
    return intro.str();
}

}
