#include "Client.hpp"

jpl::_network::_clientmanager::Client::Client(size_t socket, const sockaddr* address)
    : socket(socket), address(address){
}

jpl::_network::_clientmanager::Client::~Client(){
    delete this->address;
    #ifdef _WIN32
        closesocket(this->socket);
    #else
        close(this->socket);
    #endif
}