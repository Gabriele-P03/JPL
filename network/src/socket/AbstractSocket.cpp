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
        this->bufferSize = 65536;
        this->packetSize = 1024;
    }
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

void jpl::_network::_socket::AbstractSocket::send(const char* data, size_t len, int flags){
    size_t sent = 0;
    while(len > sent || data[sent] != '\0'){
        const char* start = &data[sent];
        size_t toSend = (len - sent);
        if(toSend > this->packetSize){
            toSend = this->packetSize;
        }
        int res = ::send(this->_socket_index, (const char*)start, toSend, flags);
        if(res == -1){
            throw jpl::_exception::SocketException(this->_socket_index);
        }   
    }
    if(::send(this->_socket_index, "\0", 1, flags) == -1){
        throw jpl::_exception::SocketException(this->_socket_index);
    }
}

void jpl::_network::_socket::AbstractSocket::receive(std::vector<char>** pBuffer, size_t &size, int flags){
    std::vector<char>* buffer = *pBuffer;
    buffer = new std::vector<char>;
    size = 0;
    while(true){
        char packet[this->packetSize];
        int read = recv(this->_socket_index, packet, this->packetSize, flags);
        if(read <= 0){
            delete buffer;
            throw jpl::_exception::SocketException(this->_socket_index);
        }
        buffer->insert(buffer->end(), packet, packet+read);
        size += size;
        if(!buffer->empty()){
            if(buffer->back() == '\0'){
                break;
            }
        }
        if(this->bufferSize <= size){
            break;
        }
    }
}