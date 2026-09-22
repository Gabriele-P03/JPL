#include "AbstractSocket.hpp"

#ifdef _WIN32

bool jpl::_network::_socket::AbstractSocket::wsaStarted = false;
WORD jpl::_network::_socket::AbstractSocket::wVersionRequested = MAKEWORD(2,2);
WSADATA jpl::_network::_socket::AbstractSocket::wsaData = WSADATA();

#endif

jpl::_network::_socket::AbstractSocket::AbstractSocket(int af, int type, int protocol) : 
        af(af), type(type), protocol(protocol)
{                        
    #ifdef _WIN32   //WSAStartup windows call
        if(!wsaStarted){
            if(WSAStartup(wVersionRequested, &wsaData) != 0){
                throw jpl::_exception::SocketException(this->_socket_index);
            }else{
                wsaStarted = true;
            }
        }
    #endif
    this->_socket_index = socket(af, type, protocol);
    if(this->_socket_index < 0){
        throw jpl::_exception::SocketException(this->_socket_index);
        this->started = false;
    }else{
        this->started = true;
    }
    this->withTLS = false;
    this->ssl = nullptr;
    this->sslCtx = nullptr;
}

void jpl::_network::_socket::AbstractSocket::initialize(unsigned short port, unsigned long in_addr, const std::string &address){
    if(in_addr == 1){    //Most of all time in server case
        this->resultaddrinfo = new addrinfo;
        this->resultaddrinfo->ai_addr = INADDR_ANY;
    }else{  //Most of all time in client case
        struct addrinfo hints;
        memset(&hints, 0, sizeof(hints));   //Clearing hints will prevent error 11003 by getaddrinfo
        std::string port_s = std::to_string(port);
        int res = getaddrinfo(address.c_str(), port_s.c_str(), &hints, &this->resultaddrinfo);
        if(res != 0){
            throw jpl::_exception::SocketException(this->_socket_index);
        }
    }
    this->port = port;
}

void jpl::_network::_socket::AbstractSocket::send(size_t socket, const char* data, size_t len, int flags){
    size_t sent = 0;
    int res = ::send(socket, data, len, flags);
    if(res == -1){
        throw jpl::_exception::SocketException(socket);
    }
    if(data[len-1] != '\0'){    //checking whereas last char is \0
        if(::send(socket, "\0", 1, flags) == -1){  //Sending '\0' as term char
            throw jpl::_exception::SocketException(socket);
        }
    }
}

void jpl::_network::_socket::AbstractSocket::receive(size_t socket, std::vector<char>** pBuffer, int flags){
    std::vector<char>* buffer = *pBuffer;
    while(true){
        char packet[2048];
        int read = recv(socket, packet, 2048, flags);
        if(read <= 0){
            throw jpl::_exception::SocketException(socket);
        }
        buffer->insert(buffer->end(), packet, packet+read);
        if(!buffer->empty()){
            if(buffer->back() == '\0'){
                break;
            }
        }
    }
}

