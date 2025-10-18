#include <jpl/logger/LoggerWrapper.hpp>
#include "src/socket/ClientSocket.hpp"
#include "src/socket/ServerSocket.hpp"


int main(){

    #ifdef AUTO_LOG_EXCEPTION_JPL
        jpl::_logger::_exceptionhook::LoggerExceptionHook();
    #endif

    jpl::_network::_socket::ClientSocket cs = jpl::_network::_socket::ClientSocket(SOCK_STREAM);
    cs.initialize(8080, 0, "127.0.0.1");
    cs.connectToServer();


    return 0;
}