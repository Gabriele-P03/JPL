/**
 * ClientManager handles several connections (Clients' instances) and manage theirs send/receive calls
 */

#ifndef CLIENTMANAGER_HPP
#define CLIENTMANAGER_HPP

#include <cstring>
#include "Client.hpp"
#include <jpl/utils/structure/list/LinkedList.hpp>
#include <jpl/exception/runtime/IllegalStateException.hpp>
#include "../../tls/TLS.hpp"
#include "../../tls/TLSWrapper.hpp"

#include <thread>


namespace jpl{
    namespace _network{
        namespace _clientmanager{

            class ClientManager{

                protected:

                    const size_t maxClientAmount; //0 unlimited
                    std::unordered_map<std::string, Client*> clients;

                    /**
                     * This function is called everytime a new client is added into clients list.
                     * In this function, you could start indipendent threads to communicate with client's socket  
                     */
                    virtual void handleClientAfterInsert(Client* client) = 0;

                public:
                    ClientManager(size_t maxClientAmount);

                    size_t getMaxClientAmount() const noexcept{
                        return this->maxClientAmount;
                    }

                    /**
                     * Add new client to the list
                     * 
                     * @throw IllegalStateException if list is full
                     * @throw IllegalArgumentException if client is nullptr
                     * @throw IllegalArgumentException if a client with the same socket index has been already added
                     */
                    virtual void addNewClient(const std::string& identifier, Client* client);

                    ~ClientManager() = default;

            };
        }
    }
}

#endif