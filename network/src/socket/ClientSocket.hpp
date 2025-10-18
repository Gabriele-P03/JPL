/**
 * Client Socket abstraction.
 * 
 * Multi Connection mode is done via loop listening and a thread for each new connection.
 */

#ifndef CLIENT_SOCKET_JPL
#define CLIENT_SOCKET_JPL

#include "AbstractSocket.hpp"

namespace jpl{

    namespace _network{

        namespace _socket{

            class ClientSocket : public AbstractSocket{

                private:
                    bool connected;

                public: 
                    ClientSocket(int type);
                    ClientSocket(int af, int type, int protocol);

                    virtual void connectToServer() const;

            };
        }
    }
}

#endif
