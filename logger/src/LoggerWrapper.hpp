/**
 * @file 
 * 
 * This is the wrapper which ensures to include JPL's Logger if you have defined any Logger's macro, which you can read about from documentation.
 * If you have not defined any of them, a print function is defined in order to get compatibility with all JPL's modules which invoke print() function, since
 * they do not check if any of those macro had been defined; indeed they invoke jpl::_logger::print() (not jpl::_logger::Logger::print()).
 * 
 * If you choose to not use JPL's Logger at all, although you can read about some LOG_STATUS below, they are just defined for compatibity
 * and status will not be printed; furthermore no debug messages will be print, since it could be reason of security leak due to no checks
 * performed on jpl::_utils::_debug::isDebugging().
 * 
 * You should always include this header rather than Logger.hpp if you do not wanna force using this module
 * 
 * @author Gabriele-P03
 * @date 2023-08-08
 * @copyright Copyright (c) 2023
 * 
 */

#ifndef LOGGER_WRAPPER_JPL
#define LOGGER_WRAPPER_JPL

    #include <jpl/exception/runtime/RuntimeException.hpp>

    namespace jpl{
        namespace _utils{
            namespace _debug{

                extern bool isDebugging();
            }
        }
    }
    
    //This implicit definition is required only for including headers
    #if ((defined(CUSTOM_LOGGER_JPL) || defined(UFW_LOGGER_JPL) || defined(QUIET_LOGGER_JPL)) && !defined(USE_LOGGER_JPL))
        #define USE_LOGGER_JPL
    #endif

    #ifdef USE_LOGGER_JPL
        #warning "Including Logger.hpp"
        #include "Logger.hpp"

        namespace jpl{
            namespace _logger{
                inline void print(std::string msg, const jpl::_logger::LOG_STATUS status){
                    jpl::_logger::Logger::INSTANCE->print(msg, status);
                }
                inline void info(std::string msg){
                    jpl::_logger::Logger::INSTANCE->print(msg, jpl::_logger::INFO_JPL);
                }
                inline void error(std::string msg){
                    jpl::_logger::Logger::INSTANCE->print(msg, jpl::_logger::ERROR_JPL);
                }
                inline void warning(std::string msg){
                    jpl::_logger::Logger::INSTANCE->print(msg, jpl::_logger::WARNING_JPL);
                }
                inline void debug(std::string msg){
                    jpl::_logger::Logger::INSTANCE->print(msg, jpl::_logger::DEBUG_JPL);
                }
            }
        }
    #else 
        #warning "Not Including Logger.hpp"
        #include <iostream>
        #include <string>
        #include <ctime>
        #include <mutex>

        namespace jpl{

            namespace _logger{

                typedef const char* const LOG_STATUS;

                /**
                * @brief Information message
                */
                const LOG_STATUS INFO_JPL = "INF";
                /**
                * @brief Warning message
                */
                const LOG_STATUS WARNING_JPL = "WAR";
                /**
                * @brief Error message
                */
                const LOG_STATUS ERROR_JPL = "ERR";
                /**
                * @brief Debug message (not visible unless process has a debugger attached) 
                */
                const LOG_STATUS DEBUG_JPL = "DBG";

                extern std::mutex logger_mutex;


                static std::string getTM(){
                    std::time_t now = std::time(0);
                    std::tm *ltm = std::localtime(&now);

                    return  std::to_string((*ltm).tm_mday) + "-" + 
                            std::to_string((*ltm).tm_mon+1) + "-" + 
                            std::to_string((*ltm).tm_year + 1900) + "_" +
                            std::to_string((*ltm).tm_hour) + ":" +
                            std::to_string((*ltm).tm_min) + ":" +
                            std::to_string((*ltm).tm_sec);
                }

                /**
                 * @brief status will be ignored (if DBG, it will not be passed)
                 * 
                 * @param msg status
                 * @param status message status
                 */
                inline void print(std::string msg, const LOG_STATUS status){
                    
                    #ifndef DISABLE_LOGGER_JPL
                        std::lock_guard<std::mutex> lock(logger_mutex);
                        if(status == "DBG" && !_utils::_debug::isDebugging()){
                            return;
                        }

                        std::cout<<"[ "<<getTM()<<" "<<status<<" ] "<<msg<<std::endl;
                    #endif
                }

                inline void info(std::string msg){ jpl::_logger::print(msg, INFO_JPL); }
                inline void error(std::string msg){ jpl::_logger::print(msg, ERROR_JPL); }
                inline void warning(std::string msg){ jpl::_logger::print(msg, WARNING_JPL); }
                inline void debug(std::string msg){ jpl::_logger::print(msg, DEBUG_JPL); }
            }
        } 
    #endif

    #ifdef USE_STACKTRACE_W_EXCEPTION_JPL
    namespace jpl{
        namespace _utils{
            namespace _debug{
                class Stacktrace;
                extern Stacktrace* getStacktrace(unsigned long skipped, unsigned long maxFrame);
                extern std::string stktrc_str(const Stacktrace* stacktrace);
            }
        }
    }
    #endif

    #ifdef AUTO_LOG_EXCEPTION_JPL

    namespace jpl{
        namespace _logger{
            namespace _exceptionhook{

                extern void on_terminate();

                class LoggerExceptionHook{

                    public:
                        
                        LoggerExceptionHook(){
                            jpl::_logger::info("Logger's Exception Hook set");
                            std::set_terminate(on_terminate);
                        }
                         
                };
            }
        }
    }
    
    #endif 

#endif