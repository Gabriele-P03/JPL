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

void jpl::_network::_ssl::connectSSL(SSL* ssl){
    const int status = SSL_connect(ssl);
    if(status != 1){
        throw new jpl::_exception::RuntimeException("Could not enstabilish SSL connection: " + std::to_string(status));
    }
    jpl::_logger::info("SSL enstabilished with cipher: " + std::string(SSL_get_cipher(ssl)));
}