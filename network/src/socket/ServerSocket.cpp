#include "ServerSocket.hpp"

jpl::_network::_socket::ServerSocket::ServerSocket(int type, jpl::_network::_clientmanager::ClientManager* manager) : 
    jpl::_network::_socket::ServerSocket(AF_INET, type, 0, manager){
}

jpl::_network::_socket::ServerSocket::ServerSocket(int af, int type, int protocol, jpl::_network::_clientmanager::ClientManager* manager) : 
    jpl::_network::_socket::AbstractSocket(af, type, protocol){
        this->manager = manager;
}

void jpl::_network::_socket::ServerSocket::initialize(unsigned short port, unsigned long in_addr, const std::string &address){
    jpl::_network::_socket::AbstractSocket::initialize(port, in_addr, address);
    if(in_addr == 1){
        this->in_addr = INADDR_ANY;
    }else{
        this->in_addr = in_addr;
    }
}

void jpl::_network::_socket::ServerSocket::start(size_t backlog){
    if(this->resultaddrinfo == nullptr){
        throw jpl::_exception::IllegalStateException("Initialize method has not been called yet");
    }
    struct sockaddr_in a;
    a.sin_family = this->af;
    a.sin_port = htons(this->port);
    a.sin_addr.s_addr = this->in_addr;
    if(bind(this->_socket_index, (struct sockaddr*)&a, sizeof(a)) != 0){
        throw jpl::_exception::SocketException(this->_socket_index);
    }else{
        if(listen(this->_socket_index, backlog) != 0){
            throw jpl::_exception::SocketException(this->_socket_index);
        }else{
            this->listening = true;
        }
    }
}

void jpl::_network::_socket::ServerSocket::loop(){
    while(listening){
        struct sockaddr* address = new sockaddr;
        size_t socket = this->acceptNewClient(address);
        jpl::_network::_clientmanager::Client* client = new jpl::_network::_clientmanager::Client(socket, address);
        this->manager->addNewClient(client);
    }
}

long jpl::_network::_socket::ServerSocket::acceptNewClient(sockaddr* client){
    jpl::_logger::info("Waiting for new client...");
    struct sockaddr clientBuffer;
    int sockaddrlen = sizeof(clientBuffer);
    memset(&clientBuffer, 0, sockaddrlen);
    long res = accept(this->_socket_index, &clientBuffer, &sockaddrlen);
    if(res <= 0){
        throw jpl::_exception::SocketException(this->_socket_index);
    }
    jpl::_logger::info("Client connected with socket index: " + std::to_string(res));
    *client = clientBuffer;
    return res;
}
