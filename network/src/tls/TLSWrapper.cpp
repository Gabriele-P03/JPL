#include "TLSWrapper.hpp"

void jpl::_network::_ssl::send(SSL* ssl, const char* buffer, size_t size){
    size_t sent = 0;
    while(size > sent || buffer[sent] != '\0'){
        const char* start = &buffer[sent];
        size_t toSend = (size - sent);
        if(toSend >256){
            toSend = 256;
        }
        int res = SSL_write(ssl, start, toSend);
        if(res <= 0){
            throw jpl::_exception::RuntimeException(jpl::_utils::_error::_GetLastErrorAsString());
        }  
        sent += res; 
    }
    if(SSL_write(ssl, "\0", 1) <= 0){  //Sending '\0' as term char
        throw jpl::_exception::RuntimeException(jpl::_utils::_error::_GetLastErrorAsString());
    }
}

void jpl::_network::_ssl::receive(SSL* ssl, std::vector<char>** pBuffer, size_t &size){
    std::vector<char>* buffer = *pBuffer;
    buffer = new std::vector<char>;
    size = 0;
    while(true){
        char packet[256];
        int read = SSL_read(ssl, packet, 256);
        if(read <= 0){
            delete buffer;
            throw jpl::_exception::RuntimeException(jpl::_utils::_error::_GetLastErrorAsString());
        }
        buffer->insert(buffer->end(), packet, packet+read);
        size += size;
        if(!buffer->empty()){
            if(buffer->back() == '\0'){
                break;
            }
        }
        if(256 <= size){
            break;
        }
    }
}