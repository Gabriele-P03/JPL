#include "src/Logger.hpp"
#include <jpl/exception/runtime/SocketException.hpp>


void t(){
throw jpl::_exception::SocketException(1, 1);
}

int main(){

    jpl::_logger::Logger::initStaticLogger();
    jpl::_logger::_exceptionhook::LoggerExceptionHook();

    jpl::_logger::info("prova");
    jpl::_logger::info("prova");
    jpl::_logger::info("prova");
    jpl::_logger::info("prova");
    jpl::_logger::info("prova");
    jpl::_logger::info("prova");
    jpl::_logger::info("prova");

    t();
}