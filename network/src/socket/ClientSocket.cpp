#include "ClientSocket.hpp"

jpl::_network::_socket::ClientSocket::ClientSocket(int type) : 
    jpl::_network::_socket::ClientSocket(AF_INET, type, 0){
}

jpl::_network::_socket::ClientSocket::ClientSocket(int af, int type, int protocol) : 
    jpl::_network::_socket::AbstractSocket(af, type, protocol){
        this->connected = false;
}

void jpl::_network::_socket::ClientSocket::connectToServer() const{
    if(connect(this->_socket_index, this->resultaddrinfo->ai_addr, this->resultaddrinfo->ai_addrlen) != 0){
        throw jpl::_exception::SocketException(this->_socket_index);
    }else{
        jpl::_logger::info("Connection estabilished with " + this->address);
    }
}