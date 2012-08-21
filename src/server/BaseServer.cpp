#include "BaseServer.hpp"

namespace Dramaqueen
{

BaseServer::BaseServer()
{
    logger = Logger::getSingletonPtr();
    initServer();
}

BaseServer::~BaseServer()
{

}

void BaseServer::dropRights()
{
    pw = getpwnam( Config::getSingletonPtr()->getUser().c_str() );
    if( getuid() == 0)
    {
        if( setgid( pw->pw_uid ) != 0 )
            logger->log( "setgid: Unable to drop group privileges: ", strerror( errno ) );
        if( setuid( pw->pw_gid ) != 0 )
            logger->log( "setuid: Unable to drop user privileges: ", strerror( errno ) );
    }
}

void BaseServer::initServer()
{
    SSL_library_init();
    //SSL_load_error_strings();
    ERR_load_BIO_strings();
    ERR_load_SSL_strings();
    cert = "/home/roa/programming/examples/ssl_conn/ssl_example/servercert.pem";
    key  = "/home/roa/programming/examples/ssl_conn/ssl_example/private.key";
    host = Config::getSingletonPtr()->getBind();

    ctx = SSL_CTX_new(SSLv3_server_method());
    SSL_CTX_use_certificate_file(ctx, cert.c_str(), SSL_FILETYPE_PEM);
    SSL_CTX_use_PrivateKey_file(ctx, key.c_str(), SSL_FILETYPE_PEM);
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
    std::string tempstr;

    memset( rbuf, '\0', sizeof(rbuf) );

    tempstr.append( rbuf );

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
            tempstr.append( rbuf );
            std::cout << "rbuf " << rbuf << std::endl;
        }
    } while( SSL_pending( ssl ) );

    if( !tempstr.empty() )
    {
        std::string dir = "script/";
        dir.append( tempstr );
        std::ifstream fileCheck( dir );
        if( fileCheck.good() )
        {
            std::string buffer = executeScript( tempstr );
            SSL_write( ssl, buffer.c_str(), buffer.size() );
        }
        else
        {
            logger->log( "did not find file: ", tempstr );
        }

    }

    close( cfd );
}

void BaseServer::run()
{
    dropRights();

    if( BIO_do_accept( abio ) <= 0 )
    {
        abort();
    }

    fd_set fds;

    int afd = BIO_get_fd( abio,NULL );

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
        }
    }
}

std::string BaseServer::executeScript( std::string script )
{
    std::string myScriptFolder = "script/";
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

}
