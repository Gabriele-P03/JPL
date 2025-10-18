/**
 * Server Socket is able to provide either single or multi connection accepting.
 * 
 * Multi Connection mode is done via loop listening and a thread for each new connection.
 */

#ifndef SERVER_SOCKET_JPL
#define SERVER_SOCKET_JPL

#include "AbstractSocket.hpp"
#include "clientmanager/ClientManager.hpp"

namespace jpl{

    namespace _network{

        namespace _socket{

            class ServerSocket : public AbstractSocket{

                protected:
                    bool listening;
                    unsigned long in_addr;

                    _clientmanager::ClientManager* manager;

                public: 

                    ServerSocket(int type, _clientmanager::ClientManager* manager);
                    ServerSocket(int af, int type, int protocol, _clientmanager::ClientManager* manager);
                    
                    virtual void initialize(unsigned short port, unsigned long in_addr, const std::string &address) override;

                    virtual void start(size_t backlog);

                    virtual void loop() override;

                    virtual long acceptNewClient(sockaddr* client);


            };
        }
    }
}

#endif
