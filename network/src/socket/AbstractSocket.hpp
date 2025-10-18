/**
 * An abstract socket is the abstraction of any socket type class
 */

#ifndef ABSTRACT_SOCKET_JPL
#define ABSTRACT_SOCKET_JPL

    #ifdef _WIN32
        #include <winsock2.h>
        #include <windows.h>
        #include <WS2tcpip.h>   //For inet_pton
    #elif defined(__linux__)
        #include <sys/socket.h>
        #include <arpa/inet.h>  //For inet_pton
    #endif

#include <string>
#include <jpl/utils/debug/ErrorUtils.hpp>
#include <jpl/logger/LoggerWrapper.hpp>
#include <jpl/exception/runtime/IllegalStateException.hpp>
#include <jpl/exception/runtime/SocketException.hpp>


namespace jpl{

    namespace _network{

        namespace _socket{

            class AbstractSocket{

                protected:
                    const int af;
                    const int type;
                    const int protocol;
                    int _socket_index;
                    std::string address;

                    size_t port;
                    
                    struct addrinfo* resultaddrinfo = NULL;

                    bool started;

                    size_t packetSize;
                    size_t bufferSize;

                    #ifdef _WIN32
                        static bool wsaStarted;
                        static WORD wVersionRequested;
                        static WSADATA wsaData;
                    #endif

                protected:
                    AbstractSocket(int af, int type, int protocol);

                    /**
                     * Pure virtual loop method and must be invoked only after have initialized socket
                     */
                    virtual void loop(){
                        if(!this->started){
                            throw jpl::_exception::IllegalStateException("Initialize method must be called before loop");
                        }
                    }

                    /**
                     * Close socket
                     */
                    virtual void stop() {
                        #ifdef _WIN32
                            closesocket(this->_socket_index);
                        #else
                            close(this->_socket_index);
                        #endif
                    }

                    virtual void setPacketSize(size_t value){this->packetSize = value;}
                    virtual void setBufferSize(size_t value){this->bufferSize = value;}

                public:

                    int getFamily() const noexcept{return this->af;}
                    int getType() const noexcept{return this->type;}
                    int getProtocol() const noexcept{return this->protocol;}
                    int getSocketIndex() const noexcept{return this->_socket_index;}
                    std::string getAddress() const noexcept{return this->address;}
                    bool isStarted() const noexcept{return this->started;}

                    virtual void addSocketOption(size_t level, size_t optionName, const char* optionValue, size_t optionLength){
                        if( setsockopt(this->_socket_index, level, optionName, optionValue, optionLength) != 0){
                            throw jpl::_exception::SocketException(this->_socket_index);
                        }
                    }

                
                    /**
                     * 
                     */
                    virtual void initialize(unsigned short port, unsigned long in_addr, const std::string &address);

                    virtual void send(const char* data, size_t len, int flags);

                    virtual void receive(std::vector<char>** pBuffer, size_t &size, int flags);
            };
        }
    }
} 


#endif