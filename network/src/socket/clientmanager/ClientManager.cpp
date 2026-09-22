#include "ClientManager.hpp"

jpl::_network::_clientmanager::ClientManager::ClientManager(size_t maxClientAmount) : maxClientAmount(maxClientAmount){
}

void jpl::_network::_clientmanager::ClientManager::addNewClient(const std::string& identifier, jpl::_network::_clientmanager::Client* client){
    if(client == nullptr){
        throw jpl::_exception::IllegalArgumentException("client cannot be nullptr");
    }
    if(this->clients.size() >= this->maxClientAmount && this->maxClientAmount > 0){ //maxClientAmount == 0 means unlimited
        throw jpl::_exception::IllegalStateException("This ClientManager instance is able to handle only " + std::to_string(this->maxClientAmount) + " clients");
    }
    for (auto& [idt, cr] : this->clients) {
        if(cr == client || cr->getSocket() == client->getSocket()){
            throw jpl::_exception::IllegalArgumentException("Socket " + std::to_string(cr->getSocket()) + " has been already added to list");
        }
        if(strcmp(idt.c_str(), identifier.c_str()) == 0){
            throw jpl::_exception::IllegalArgumentException("Client with identifier " + identifier + " has been already inserted");
        }
    }
    this->clients.insert( std::make_pair(identifier, client));
    this->handleClientAfterInsert(client);
    jpl::_logger::info("Connection enstabilished with client: ");
}