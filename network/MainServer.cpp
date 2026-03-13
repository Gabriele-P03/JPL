#define AUTO_LOG_EXCEPTION_JPL
#include <winsock2.h>
#include <jpl/logger/Logger.hpp>
#include "src/socket/ServerSocket.hpp"
#include "src/tls/TLS.hpp"
#include "src/socket/clientmanager/TestClientManager.hpp"
#include <jpl/utils/FilesUtils.hpp>

int main(){

    #ifdef AUTO_LOG_EXCEPTION_JPL
        jpl::_logger::_exceptionhook::LoggerExceptionHook();
    #endif

    jpl::_network::_socket::ServerSocket ss = jpl::_network::_socket::ServerSocket(SOCK_STREAM, new jpl::_network::_clientmanager::TestClientManager());
    ss.setTLS(true);
    ss.initialize(8080, 0x01, "");
    ss.loadCertificate(
        jpl::_utils::_files::getLocalPath("certificate.crt"),
        SSL_FILETYPE_PEM,
        jpl::_utils::_files::getLocalPath("private.key"),
        SSL_FILETYPE_PEM
    );
    ss.start(5);
    ss.loop();
    //
    return 0;
}