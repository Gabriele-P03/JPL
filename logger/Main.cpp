#define AUTO_LOG_EXCEPTION_JPL
#define USE_STACKTRACE_W_EXCEPTION_JPL
#define USE_LOGGER_JPL

#include "src/LoggerWrapper.hpp"
#include <jpl/exception/runtime/SocketException.hpp>


void t(){
throw jpl::_exception::SocketException(1, 1);
}

int main(){

    jpl::_logger::_exceptionhook::LoggerExceptionHook();

    t();
}