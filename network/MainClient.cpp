#include <winsock2.h>
#include <jpl/logger/LoggerWrapper.hpp>
#include "src/socket/ClientSocket.hpp"
#include "src/tls/TLS.hpp"


int main(){

    #ifdef AUTO_LOG_EXCEPTION_JPL
        jpl::_logger::_exceptionhook::LoggerExceptionHook();
    #endif

    jpl::_network::_socket::ClientSocket cs = jpl::_network::_socket::ClientSocket(SOCK_STREAM);
    cs.initialize(8080, 0, "127.0.0.1");
    

    SSL* ssl = jpl::_network::_ssl::instanceNewSSL(jpl::_network::_ssl::initSSLContext(TLS_server_method()), cs.getSocketIndex());
    jpl::_network::_ssl::connectSSL(ssl);
    
    //cs.connectToServer();


    return 0;
}