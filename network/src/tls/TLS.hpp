/**
    TLS mechanism for instance an encrypted end-to-end channel.
    As many papers say, SSL is just a common synonim, despite SSL is the TLS' ancestor, of TLS

    TLS mechanism is based on OpenSSL, thus, you could easily read its official documentation; by the way, in order to
    instance it, you just have to initialize SSL's context via initSSLContext(), pass its result to instanceNewSSL() and then
    connectSSL()
*/

#ifndef TLS_JPL
#define TLS_JPL

#ifdef _WIN32
    #include <winsock2.h>
#endif

#include <openssl/ssl.h>
#include <openssl/err.h>
#include <jpl/exception/runtime/RuntimeException.hpp>
#include <jpl/utils/debug/ErrorUtils.hpp>
#include <jpl/logger/Logger.hpp>

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
             * Connect via SSL.
             * It should be called only by client to connect to server
             * 
             * @param ssl 
             * 
             * @throw RuntimeException if connection could not be enstabilished
             */
            long connectSSL(SSL* ssl);
        }
    }
}

#endif