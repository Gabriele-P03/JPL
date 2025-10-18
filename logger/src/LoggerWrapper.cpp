#include "LoggerWrapper.hpp"

#ifndef USE_LOGGER_JPL
std::mutex jpl::_logger::logger_mutex;
#endif

#ifdef AUTO_LOG_EXCEPTION_JPL



void jpl::_logger::_exceptionhook::on_terminate(){
    std::exception_ptr ptr = std::current_exception();
    try{
        if (ptr){//Program is really going to be closed due to an unhandled exception 
            std::rethrow_exception(ptr);
        }
    }catch(const jpl::_exception::RuntimeException &e){
        const char* tmp = e.what();
        std::string buffer = std::string(tmp);
        delete[] tmp;

        #ifdef USE_STACKTRACE_W_EXCEPTION_JPL
            _utils::_debug::Stacktrace* stacktrace = _utils::_debug::getStacktrace(2, 512);
            buffer += _utils::_debug::stktrc_str(stacktrace);
            _logger::error(buffer);
        #endif
    }catch(...){

    }
}

#endif