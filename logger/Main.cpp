
#include "src/LoggerWrapper.hpp"
#include <jpl/exception/runtime/SocketException.hpp>


void t(){
throw jpl::_exception::SocketException(1, 1);
}

int main(){

    jpl::_logger::_exceptionhook::LoggerExceptionHook();

    t();
}