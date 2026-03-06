#include "TLS.hpp"

SSL_CTX* jpl::_network::_ssl::initSSLContext(const SSL_METHOD* method){
    SSL_CTX* ctx = SSL_CTX_new(method);
    if(ctx == nullptr){
        throw new jpl::_exception::RuntimeException(jpl::_utils::_error::_GetLastErrorAsString());
    }
    return ctx;
}

SSL* jpl::_network::_ssl::instanceNewSSL(SSL_CTX* ctx, long fd){

    SSL* ssl = SSL_new(ctx);
    if(ssl == nullptr){
        throw new jpl::_exception::RuntimeException(jpl::_utils::_error::_GetLastErrorAsString());
    }
    SSL_set_fd(ssl, fd);
    return ssl;
}

long jpl::_network::_ssl::connectSSL(SSL* ssl){
    const long status = SSL_connect(ssl);
    if(status != 1){
        int err = SSL_get_error(ssl, status);
        if(err == 5){
            throw new jpl::_exception::RuntimeException("Could not enstabilish SSL connection: " + jpl::_utils::_error::_GetLastErrorAsString());
        }
        ERR_print_errors_fp(stderr);
        
    }
    jpl::_logger::info("SSL enstabilished with cipher: " + std::string(SSL_get_cipher(ssl)));
    return status;
}