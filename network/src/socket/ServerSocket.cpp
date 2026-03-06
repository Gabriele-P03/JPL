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
    if(this->withTLS){
        this->sslCtx = jpl::_network::_ssl::initSSLContext(TLS_server_method());
    }
}

void jpl::_network::_socket::ServerSocket::loadCertificate(const std::string &certPath, int certType, const std::string &keyPath, int keyType){
    if(SSL_CTX_use_certificate_file(this->sslCtx, certPath.c_str(), certType) <= 0){
        ERR_print_errors_fp(stderr);
        throw jpl::_exception::SocketException(this->_socket_index);
    }
    if(SSL_CTX_use_PrivateKey_file(this->sslCtx, keyPath.c_str(), keyType) <= 0){
        ERR_print_errors_fp(stderr);
        throw jpl::_exception::SocketException(this->_socket_index);
    }
    if(SSL_CTX_check_private_key(this->sslCtx) <= 0){
        throw jpl::_exception::RuntimeException("Private Key and Certificate does not match");
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
        jpl::_network::_clientmanager::Client* client = this->acceptNewClient();
        this->manager->addNewClient(client);
    }
}

jpl::_network::_clientmanager::Client* jpl::_network::_socket::ServerSocket::acceptNewClient(){
    jpl::_logger::info("Waiting for new client...");
    struct sockaddr* address = new sockaddr;
    int sockaddrlen = sizeof(*address);
    memset(address, 0, sockaddrlen);
    long res = accept(this->_socket_index, address, &sockaddrlen);
    if(res <= 0){
        throw jpl::_exception::SocketException(this->_socket_index);
    }
    SSL* ssl = nullptr;
    if(this->withTLS){
        ssl = jpl::_network::_ssl::instanceNewSSL(this->sslCtx, res);
        long sslAcceptRes = SSL_accept(ssl);
        if(sslAcceptRes <= 0){
            ERR_print_errors_fp(stderr);
        }
    }
    jpl::_network::_clientmanager::Client* client = new jpl::_network::_clientmanager::Client(res, address, ssl);
    return client;
}
