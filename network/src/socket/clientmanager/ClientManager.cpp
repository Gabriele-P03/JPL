#include "ClientManager.hpp"

jpl::_network::_clientmanager::ClientManager::ClientManager(size_t maxClientAmount)
    : jpl::_network::_clientmanager::ClientManager::ClientManager(true, maxClientAmount){
}

jpl::_network::_clientmanager::ClientManager::ClientManager(bool asyncClient, size_t maxClientAmount)
    : maxClientAmount(maxClientAmount), asyncClient(asyncClient){

        this->clients = new jpl::_utils::_collections::_list::LinkedList<jpl::_network::_clientmanager::Client*>();
}

void jpl::_network::_clientmanager::ClientManager::addNewClient(jpl::_network::_clientmanager::Client* &client){
    if(client == nullptr){
        throw jpl::_exception::IllegalArgumentException("client cannot be nullptr");
    }
    if(this->clients->getSize() >= this->maxClientAmount && this->maxClientAmount > 0){ //maxClientAmount == 0 means unlimited
        throw jpl::_exception::IllegalStateException("This ClientManager instance is able to handle only " + std::to_string(this->maxClientAmount) + " clients");
    }
    for(size_t i = 0; i < this->clients->getSize(); i++){
        const jpl::_network::_clientmanager::Client* cr = this->clients->get(i);
        if(cr == client || cr->getSocket() == client->getSocket()){
            throw jpl::_exception::IllegalArgumentException("Socket " + std::to_string(cr->getSocket()) + " has been already added to list");
        }
    }
    this->clients->add(client);
    this->handleClientAfterInsert(client);
}

void jpl::_network::_clientmanager::ClientManager::handleClientAfterInsert(jpl::_network::_clientmanager::Client* client){
    if(this->asyncClient){
        std::thread sendThread(&jpl::_network::_clientmanager::ClientManager::send, this, client);
        sendThread.detach();
        std::thread receiveThread(&jpl::_network::_clientmanager::ClientManager::receive, this, client); 
        receiveThread.detach();
    }
}


jpl::_network::_clientmanager::ClientManager::~ClientManager(){
    delete this->clients;
}