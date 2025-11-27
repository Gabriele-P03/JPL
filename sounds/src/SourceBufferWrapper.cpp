#include "SourceBufferWrapper.hpp"

void jpl::_sounds::attachBuffer(jpl::_sounds::Source* s, jpl::_sounds::Buffer* b){
    if(b == nullptr){
        throw jpl::_exception::IllegalArgumentException("Buffer is nullptr");
    }   
    if(s == nullptr){
        throw jpl::_exception::IllegalArgumentException("Source is nullptr");
    }
    for(unsigned int i = 0; i < s->buffers.getSize(); i++){
        jpl::_sounds::Buffer* cr = s->buffers.get(i);
        if(cr == b){
            throw jpl::_exception::RuntimeException("Source " + s->getRegName() + " already contains given buffer");
        }
        if(strcmp(cr->getRegName().c_str(), b->getRegName().c_str()) == 0){
            throw jpl::_exception::RuntimeException("Source " + s->getRegName() + " already contains " + b->getRegName() + " buffer");
        }
    }
    for(unsigned int i = 0; i < b->sources.getSize(); i++){
        jpl::_sounds::Source* cr = b->sources.get(i);
        if(cr == s){
            throw jpl::_exception::RuntimeException("Buffer " + b->getRegName() + " has been already attached to given source");
        }
        if(strcmp(cr->getRegName().c_str(), s->getRegName().c_str()) == 0){
             throw jpl::_exception::RuntimeException("Buffer " + b->getRegName() + " has been already attached to " + s->getRegName() + " source");
        }
    }
    alSourcei(s->getSource(), AL_BUFFER, b->getBuffer());
    jpl::_sounds::_errors::checkForError();
    b->sources.add(s);
    s->buffers.add(b);
}

void jpl::_sounds::deleteBuffer(jpl::_sounds::Buffer* b){
    if(b == nullptr){
        throw jpl::_exception::IllegalArgumentException("Buffer is nullptr");
    }  
    for(unsigned int i = 0; i < b->sources.getSize(); i++){
        jpl::_sounds::Source* cr = b->sources.get(i);
        alSourcei(cr->getSource(), AL_BUFFER, 0);
    }
    unsigned int i = b->getBuffer();
    alDeleteBuffers(1, &i);
}

void jpl::_sounds::deleteSource(jpl::_sounds::Source* s){
    if(s == nullptr){
        throw jpl::_exception::IllegalArgumentException("Source is nullptr");
    } 
    alSourcei(s->getSource(), AL_BUFFER, 0);
    unsigned int i = s->getSource();
    alDeleteSources(1, &i);
}