#include "BaseServer.hpp"

namespace Dramaqueen
{

BaseServer::BaseServer()
{
    initServer();
}

BaseServer::~BaseServer()
{
    SSL_CTX_free( ctx );
    ctx = NULL;
}

void BaseServer::initServer()
{
    SSL_library_init();
    ERR_load_BIO_strings();
    ERR_load_SSL_strings();
    cert = Config::getSingletonPtr()->getSSLCert();
    key  = Config::getSingletonPtr()->getSSLKey();
    host = Config::getSingletonPtr()->getBind();
    /**
        TODO:
        check return values
    **/
    ctx = SSL_CTX_new( SSLv3_server_method() );
    SSL_CTX_use_certificate_file(ctx, cert.c_str(), SSL_FILETYPE_PEM );
    SSL_CTX_use_PrivateKey_file(ctx, key.c_str(), SSL_FILETYPE_PEM );
    abio = BIO_new_accept( ( char * ) host.c_str() );
    if( abio == NULL )
    {
        abort();
    }
    BIO_set_nbio( abio, 0 );
}

void BaseServer::handleClient()
{
    int cfd = BIO_get_fd( client, NULL );
    int r;
    char rbuf[4096];
    std::string message;

    memset( rbuf, '\0', sizeof(rbuf) );

    message.append( rbuf );

    do
    {
        r = SSL_read( ssl, rbuf, sizeof( rbuf ) - 1 );
        if( r < 0 )
        {
            break;
        }
        if( r == 0 )
        {
            break;
        }
        else
        {
            rbuf[r] = '\0';
            message.append( rbuf );
        }
    } while( SSL_pending( ssl ) );

    if( !message.empty() )
    {
        message = parseMessage( message );
        std::string dir = Config::getSingletonPtr()->getScriptDir();
        dir.append( message );
        std::ifstream fileCheck( dir );
        if( fileCheck.good() )
        {
            std::string buffer = executeScript( message );
            if( !buffer.empty() )
            {
                int w = SSL_write( ssl, buffer.c_str(), buffer.size() );
                if( w <= 0 )
                {
                    /**
                        TODO:
                        do something useful on failure
                    **/
                    std::cout << SSL_get_error( ssl, w ) << std::endl;
                }
            }
        }
    }
    ERR_remove_state( 0 );
    close( cfd );
}

void BaseServer::run()
{
    if( BIO_do_accept( abio ) <= 0 )
    {
        abort();
    }

    fd_set fds;

    int afd = BIO_get_fd( abio,NULL );

    /**
        TODO:
        replace true with abort condition
        from sighandler
    **/
    while( true )
    {
        FD_ZERO( &fds );
        FD_SET( afd, &fds );

        select( afd+1, &fds, NULL, NULL, NULL );

        if( FD_ISSET( afd, &fds ) && BIO_do_accept( abio ) > 0 )
        {
            client = BIO_pop( abio );
            ssl = SSL_new( ctx );
            SSL_set_accept_state( ssl );
            SSL_set_bio( ssl, client, client );
            SSL_accept( ssl );
            handleClient();
            SSL_free(ssl);
            ssl = NULL;
        }
    }
}

std::string BaseServer::executeScript( std::string script )
{
    std::string myScriptFolder = Config::getSingletonPtr()->getScriptDir();
    myScriptFolder.append( script.c_str() );
    FILE* pipe = popen( myScriptFolder.c_str(), "r" );
    if ( !pipe ) return "ERROR";
    char buffer[128];
    std::string result = "";
    while( !feof( pipe ) )
    {
        if( fgets( buffer, 128, pipe ) != NULL )
            result += buffer;
    }
    pclose( pipe );
    return result;
}

std::string BaseServer::parseMessage( std::string message )
{
    std::string sharedSecret = Config::getSingletonPtr()->getSharedSecret();
    if( ! sharedSecret.empty() )
    {
        size_t found = message.find( sharedSecret );
        if( found != std::string::npos )
        {
            return message.replace( message.find( sharedSecret ), sharedSecret.length(), "" );
        }
        else
        {
            /**
                TODO: failure handling
            **/
        }
    }
    return message;
}

}
