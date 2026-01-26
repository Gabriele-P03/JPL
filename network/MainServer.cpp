#include <winsock2.h>
#include <jpl/logger/LoggerWrapper.hpp>
#include "src/socket/ServerSocket.hpp"
#include "src/tls/TLS.hpp"
#include "src/socket/clientmanager/TestClientManager.hpp"

int main(){

    #ifdef AUTO_LOG_EXCEPTION_JPL
        jpl::_logger::_exceptionhook::LoggerExceptionHook();
    #endif

    jpl::_network::_socket::ServerSocket ss = jpl::_network::_socket::ServerSocket(SOCK_STREAM, new jpl::_network::_clientmanager::TestClientManager());
    ss.initialize(8080, 0x01, "");
    ss.start(5);

    jpl::_network::_ssl::instanceNewSSL(jpl::_network::_ssl::initSSLContext(TLS_server_method()), ss.getSocketIndex());
    SSL_Server
    return 0;
}