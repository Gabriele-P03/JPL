#include "ClientManager.hpp"

#include <jpl/logger/LoggerWrapper.hpp>

namespace jpl{
    namespace _network{
        namespace _clientmanager{

            class TestClientManager : public ClientManager{

                public:
                    TestClientManager() : ClientManager(5){}

                protected:

                void receive(Client* client) override{
                    jpl::_logger::info("Receiving: ");
                }

                void send(Client* client) override{
                    jpl::_logger::info("Sending: ");
                }
            };
        }
    }
}