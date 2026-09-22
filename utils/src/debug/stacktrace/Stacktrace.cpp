#include "Stacktrace.hpp"

jpl::_utils::_debug::Stacktrace::Stacktrace(unsigned long skipped, unsigned long maxSize){
    this->skipped = skipped;
    this->maxSize = maxSize;
    this->frames = queryStacktrace(skipped, maxSize);
    this->size = this->frames->getSize();
}
jpl::_utils::_debug::Stacktrace::Stacktrace(unsigned long skipped) : jpl::_utils::_debug::Stacktrace(skipped, MAX_STACKFRAMES_JPL){}
jpl::_utils::_debug::Stacktrace::Stacktrace() : jpl::_utils::_debug::Stacktrace(0){}

const jpl::_utils::_debug::Frame jpl::_utils::_debug::Stacktrace::getFrameAt(unsigned long i) const{
    if(i >= this->size)
        throw jpl::_exception::IndexOutOfBoundsException(this->size, i);
    return *this->frames->get(i);
}

const jpl::_utils::_collections::_list::LinkedList<jpl::_utils::_debug::Frame*>* jpl::_utils::_debug::Stacktrace::getFrames() const {
     return this->frames; 
}

const unsigned long jpl::_utils::_debug::Stacktrace::getSkippedFrames() const{
    return this->skipped;
}


jpl::_utils::_debug::Stacktrace::~Stacktrace(){
    this->frames->clear();
    delete this->frames;
}

//NON MEMBER FUNCTION

jpl::_utils::_debug::Stacktrace* jpl::_utils::_debug::getStacktrace(unsigned long skipped, unsigned long maxFrame){
    return new jpl::_utils::_debug::Stacktrace(skipped, maxFrame);
}

std::string jpl::_utils::_debug::stktrc_str(const Stacktrace* stacktrace){
    std::string buffer = "";
    for(unsigned int i = 0; i < stacktrace->getSize(); i++){
        _utils::_debug::Frame frame = stacktrace->getFrameAt(i);
        std::string tmp = "#" + std::to_string(i) + " Invalid Frame";
        if(frame.frame_valid){
            tmp = "#" + std::to_string(i) + " " + frame.function_name + " of " + frame.file_name + " at line " + std::to_string(frame.line) + ". Address " + frame.address;
        }
        buffer += "\n\t" + tmp;
    }
    return buffer;
}