/**
 * A client abstracts a socket which connects server to client
 */
#ifndef CLIENT_SOCKET_HPP
#define CLIENT_SOCKET_HPP

#ifdef _WIN32
    #include <winsock2.h>
    #include <windows.h>
    #include <WS2tcpip.h>   //For inet_pton
#elif defined(__linux__)
    #include <sys/socket.h>
    #include <arpa/inet.h>  //For inet_pton
#endif

namespace jpl{
    namespace _network{
        namespace _clientmanager{

            class Client{

                protected:
                    const size_t socket;
                    const sockaddr* address;

                public:
                    Client(size_t socket, const sockaddr* address);

                    const size_t getSocket() const noexcept{
                        return this->socket;
                    }
                    const sockaddr* getAddress() const noexcept{
                        return this->address;
                    }

                    ~Client();
            };
        }
    }
} 

#endif