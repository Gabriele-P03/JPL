/**
 * Server Socket is able to provide either single or multi connection accepting.
 * Multi Connection mode is done via loop listening and a thread for each new connection.
 * 
 * A ServerSocket should be instances with the given order of istructions
 * 1. initialize(0,1,"")
 * 2. setTLS(true)          //Optional -> TLS
 * 3. loadCertificate()     //Optional -> TLS
 * 4. start()
 * 5. loop()
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

                    /**
                     * This function should be called after either initialize() or setWithTLS()
                     * 
                     * @param certPath path to certificate
                     * @param keyPath path to private key
                     */
                    virtual void loadCertificate(const std::string &certPath, int certType, const std::string &keyPath, int keyType);

                    /**
                     * Start listening for new connection
                     * 
                     * @param backlog max connection attepts in queue
                     */
                    virtual void start(size_t backlog);

                    /**
                     * It is up to programmer to override this pure virtual method.
                     * it should implement a logic to retrieve a unique identifier for client
                     * 
                     * @param client
                     */
                    virtual std::string getIdentifierPostAccept(_clientmanager::Client* client) = 0;

                    virtual void loop() override;

                    virtual _clientmanager::Client* acceptNewClient();


            };
        }
    }
}

#endif
