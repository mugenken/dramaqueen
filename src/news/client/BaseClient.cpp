#include "BaseClient.hpp"

namespace Dramaqueen
{

BaseClient::BaseClient( std::string _host, std::string _command ) :
    host( _host ), command( _command )
{
    initBaseClient();
}

BaseClient::~BaseClient()
{
    destroyBio();
}

std::string BaseClient::run()
{
    std::string response;
    response.append( connectToServer() );
    if( !response.empty() )
    {
        Helper::log( "BaseClient: failed to connect to host", host );
        SSL_CTX_free( ctx );
        return response;
    }
    response.append( sendToServer() );
    if( !response.empty() )
    {
        Helper::log( "BaseClient: failed to send to host", host );
        SSL_CTX_free( ctx );
        return response;
    }
    /**
        TODO:
        add handling if returned message is empty
    **/
    std::string recv = recvFromServer();
    if( !recv.empty() )
    {
        response.append( intro() );
        response.append( recv.c_str() );
    }
    SSL_CTX_free( ctx );
    return response;
}

void BaseClient::initBaseClient()
{
    SSL_library_init();
    initCTX();
    initBio();
}

void BaseClient::initCTX()
{
    /**
        TODO:
        check return values
    **/
    ctx  = NULL;
    cert = Config::getSingletonPtr()->getSSLCert();
    key  = Config::getSingletonPtr()->getSSLKey();
    ctx = SSL_CTX_new(SSLv3_client_method());
    SSL_CTX_use_certificate_file( ctx, cert.c_str(), SSL_FILETYPE_PEM );
    SSL_CTX_use_PrivateKey_file( ctx, key.c_str(), SSL_FILETYPE_PEM );
}

void BaseClient::initBio()
{
    bio = BIO_new_ssl_connect( ctx );

    if( bio == NULL )
    {
        Helper::log( "BaseClient: could not initialize bio to connect to", host );
    }
    /**
        TODO:
        check return values
    **/
    BIO_set_conn_hostname( bio, ( char * )host.c_str() );
}

void BaseClient::destroyBio()
{
    BIO_free_all( bio );
}

std::string BaseClient::connectToServer()
{
    std::stringstream answer;
    if( BIO_do_connect( bio ) <= 0 )
    {
        /**
        TODO: remove this crap

        FILE * mystream = fopen( "/tmp/rand", "w+");
        fclose( mystream );
        **/
        ERR_print_errors_fp( stderr );

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
