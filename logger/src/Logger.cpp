#include "Logger.hpp"

#ifndef CUSTOM_LOGGER_JPL
    #ifdef QUIET_LOGGER_JPL
        #ifdef __linux__
            jpl::_logger::Logger* jpl::_logger::Logger::INSTANCE = new jpl::_logger::Logger(
                jpl::_utils::_files::getLocalPath("logs/" + jpl::_logger::Logger::getFileNameOfInstance() + ".txt"), true
            );
        #elif _WIN32
            jpl::_logger::Logger* jpl::_logger::Logger::INSTANCE = new jpl::_logger::Logger(
                jpl::_utils::_files::getLocalPath("logs\\" + jpl::_logger::Logger::getFileNameOfInstance() + ".txt"), true
            );
        #endif
    #else
        #ifdef __linux__
            jpl::_logger::Logger* jpl::_logger::Logger::INSTANCE = new jpl::_logger::Logger(
                jpl::_utils::_files::getLocalPath("logs/" + jpl::_logger::Logger::getFileNameOfInstance() + ".txt"), false
            );
        #elif _WIN32
            jpl::_logger::Logger* jpl::_logger::Logger::INSTANCE = new jpl::_logger::Logger(
                jpl::_utils::_files::getLocalPath("logs\\" + jpl::_logger::Logger::getFileNameOfInstance() + ".txt"), false
            );
        #endif
    #endif
#endif


jpl::_logger::LOG_STATUS jpl::_logger::INFO_JPL = "INF";
jpl::_logger::LOG_STATUS jpl::_logger::WARNING_JPL = "WAR";
jpl::_logger::LOG_STATUS jpl::_logger::ERROR_JPL = "ERR";
jpl::_logger::LOG_STATUS jpl::_logger::DEBUG_JPL = "DBG";


void jpl::_logger::Logger::print(std::string msg){

    this->print(msg, jpl::_logger::INFO_JPL);
}

void jpl::_logger::Logger::print(std::string msg, const jpl::_logger::LOG_STATUS status){

    if(status == jpl::_logger::DEBUG_JPL && !jpl::_utils::_debug::isDebugging()){
        return;
    }
    logger_mutex.lock();
    msg = "[" + this->getFileNameOfInstance() + " -> " + status + "]: " + msg;
    std::cout<<msg<<std::endl;
    if(flag){
        this->file->write(msg.c_str(), msg.size());
        this->file->flush();
    }
    logger_mutex.unlock();
}

void jpl::_logger::Logger::closeLogger(){
    //In order to write on the file even when logger is closed, flag is set to false after this operation
    this->print("Closing Logger...");
    this->file->close();
    this->flag = false;
}

jpl::_logger::Logger::~Logger(){
    this->closeLogger();
}

std::string jpl::_logger::Logger::getFileNameOfInstance(){

    std::tm *ltm = jpl::_logger::Logger::getTM();

    return  std::to_string((*ltm).tm_mday) + "-" + 
            std::to_string((*ltm).tm_mon+1) + "-" + 
            std::to_string((*ltm).tm_year + 1900) + "_" +
            std::to_string((*ltm).tm_hour) + "-" +
            std::to_string((*ltm).tm_min) + "-" +
            std::to_string((*ltm).tm_sec);
}