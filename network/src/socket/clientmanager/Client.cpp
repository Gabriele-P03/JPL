#include "Client.hpp"

jpl::_network::_clientmanager::Client::Client(size_t socket, const sockaddr* address, SSL* ssl)
    : socket(socket), address(address){
        this->ssl = ssl;
        this->withTLS = (ssl != nullptr);
}

jpl::_network::_clientmanager::Client::~Client(){
    delete this->address;
    #ifdef _WIN32
        closesocket(this->socket);
    #else
        close(this->socket);
    #endif
    SSL_shutdown(this->ssl);
    SSL_free(this->ssl);
}