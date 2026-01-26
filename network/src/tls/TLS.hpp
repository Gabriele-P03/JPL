#ifndef TLS_JPL
#define TLS_JPL

#ifdef _WIN32
    #include <winsock2.h>
#endif

#include <openssl/ssl.h>
#include <jpl/exception/runtime/RuntimeException.hpp>
#include <jpl/utils/debug/ErrorUtils.hpp>
#include <jpl/logger/LoggerWrapper.hpp>

namespace jpl{
    namespace _network{
        namespace _ssl{

            /**
             * Initialize a new SSL context
             * 
             * @param method Client or Server method. Generated via TLS_client_method() or TLS_server_method()
             * @throw RuntimeException if SSL_CTX could not be instanced
             */
            SSL_CTX* initSSLContext(const SSL_METHOD* method);

            /**
             * Instance new SSL connection
             * 
             * @param ctx
             * @param fd socket descriptor
             * 
             * @throw RuntimeException if SSL could not be instanced
             */
            SSL* instanceNewSSL(SSL_CTX* ctx, long fd);

            /**
             * Connect via SSL
             * 
             * @param ssl 
             * 
             * @throw RuntimeException if connection could not be enstabilished
             */
            void connectSSL(SSL* ssl);
        }
    }
}

#endif