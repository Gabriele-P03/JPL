/**
 * TLSWrapper provides both SSL_Write and SSL_read functions
 */

#ifndef TLS_WRAPPER_JPL
#define TLS_WRAPPER_JPL

#include <openssl/ssl.h>
#include <openssl/err.h>
#include <jpl/exception/runtime/SocketException.hpp>

namespace jpl{
    namespace _network{
        namespace _ssl{

            extern void send(SSL* ssl, const char* buffer, size_t size);
            extern void receive(SSL* ssl, std::vector<char>** pBuffer, size_t &size);
        }
    }
}

#endif