#include "ClientSocket.hpp"

jpl::_network::_socket::ClientSocket::ClientSocket(int type) : 
    jpl::_network::_socket::ClientSocket(AF_INET, type, 0){
}

jpl::_network::_socket::ClientSocket::ClientSocket(int af, int type, int protocol) : 
    jpl::_network::_socket::AbstractSocket(af, type, protocol){
        this->connected = false;
}

void jpl::_network::_socket::ClientSocket::connectToServer(){
    if(connect(this->_socket_index, this->resultaddrinfo->ai_addr, this->resultaddrinfo->ai_addrlen) != 0){
        throw jpl::_exception::SocketException(this->_socket_index);
    } 
    if(this->withTLS){
        this->sslCtx = jpl::_network::_ssl::initSSLContext(TLS_client_method());
        this->ssl = jpl::_network::_ssl::instanceNewSSL(this->sslCtx, this->getSocketIndex());
        long res = jpl::_network::_ssl::connectSSL(this->ssl);
        if(res <= 0){
            throw jpl::_exception::SocketException(this->_socket_index);
        }
    }
    jpl::_logger::info("Connection estabilished with " + this->address);
}