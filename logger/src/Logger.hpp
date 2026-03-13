/**
 * A logger provides writing logs both on terminal and into file as well.
 * 
 * A standard static Logger instance is already available and you can use as you want; just notice that, unless you called initStaticLogger() undefined-behaviour may occurr
 * 
 * If Logger could not create file - if quite has been passed as true - it does not exit program with failure, but just reset flag field
 * 
 * @date 2022-08-01
 * @copyright Copyright (c) 2022
 * @author Gabriele-P03
 */
#ifndef LOGGER_JPL
#define LOGGER_JPL

    #ifdef __linux__
        #include <libgen.h> //For dirname()
        #include <unistd.h> //For readlink()
        #include <linux/limits.h> //For MAX_PATH
    #elif defined(_WIN32)
        #include <windows.h>
    #endif

    #include <string>
    #include <fstream>
    #include <iostream>
    #include <ctime>
    #include <jpl/utils/FilesUtils.hpp>
    #include <jpl/utils/debug/DebugUtils.hpp>
    #include <jpl/exception/runtime/IOException.hpp>
    #include <mutex>

    namespace jpl{

        namespace _utils{
            namespace _debug{
                class Stacktrace;
                extern Stacktrace* getStacktrace(unsigned long skipped, unsigned long maxFrame);
                extern std::string stktrc_str(const Stacktrace* stacktrace);
            }
        }

        namespace _logger{

                typedef const char* const LOG_STATUS;

                /**
                * @brief Information message
                */
                extern const LOG_STATUS INFO_JPL;
                /**
                * @brief Warning message
                */
                extern const LOG_STATUS WARNING_JPL;
                /**
                * @brief Error message
                */
                extern const LOG_STATUS ERROR_JPL;
                /**
                * @brief Debug message (not visible unless process has a debugger attached) 
                */
                extern const LOG_STATUS DEBUG_JPL;

            class Logger{

                protected:

                    std::mutex logger_mutex;

                    /**
                     * @return the file name of today e.g. dd-mm-yyyy_hh-mm-ss.txt
                     */
                    static std::string getFileNameOfInstance();

                    /**
                     * @return an instance of std::tm at the current moment
                     */
                    static inline std::tm* getTM(){
                        std::time_t now = std::time(0);
                        return std::localtime(&now);
                    }

                    /**
                     * Logger's output file
                     */
                    std::ofstream* file;

                    /**
                     * @brief If the fstream could be created, this flag is set to true,
                     * otherwise to false.
                     * Is is used to check, when UFW_LOGGER_JPL macro is defined, if logger
                     * is able to write on file, too.
                     */
                    bool flag;

                    static Logger* INSTANCE;
                    
                public:

                    /**
                     * Create a new instance of Logger associated with the output file at the given path.
                     * Quiet mode is implicitly set to false
                     * 
                     * @param pathToFile aboslute path of the file which is wanted to be used as output one
                     */
                    Logger(std::string pathToFile) : Logger(pathToFile, false){}

                    /**
                     * Create a new instance of Logger associated with the output file at the given path
                     * 
                     * @param pathToFile aboslute path of the file which is wanted to be used as output one
                     * @param quiet quiet mode if you don't want to write on file
                     */
                    Logger(std::string pathToFile, bool quiet){
                        this->flag = false;
                        if(!quiet){
                            this->file = new std::ofstream();
                            this->file->open(pathToFile);

                            if(file->fail()){
                                std::cout<<"Could not create output file: "<<pathToFile<<std::endl;
                            }else{
                                this->flag = true;
                            }
                        }
                    }

                    /**
                     * @brief Print msg on terminal
                     * Write msg on file and flushes it.
                     * 
                     * @param msg message
                     */
                    void print(std::string msg);

                    /**
                     * @brief write msg on terminal
                     * Write msg on file and flushes it.
                     * 
                     * @param msg message
                     * @param status status of the message
                     */
                    void print(std::string msg, const LOG_STATUS status);

                    /**
                     * Close logger and its file
                     */
                    void closeLogger();

                    /**
                     * @brief this check-function is useless to call unless you have defined
                     * UFW_LOGGER_JPL 
                     * 
                     * @return if Logger is able to write on file
                     */
                    bool isWriting(){ return this->flag; }

                    ~Logger();

                    static void initStaticLogger();
                    static Logger* const getLogger(){
                        return const_cast<Logger* const>(INSTANCE);
                    }
            };

                inline void print(std::string msg, const jpl::_logger::LOG_STATUS status){
                    jpl::_logger::Logger::getLogger()->print(msg, status);
                }
                inline void info(std::string msg){
                    jpl::_logger::Logger::getLogger()->print(msg, jpl::_logger::INFO_JPL);
                }
                inline void error(std::string msg){
                    jpl::_logger::Logger::getLogger()->print(msg, jpl::_logger::ERROR_JPL);
                }
                inline void warning(std::string msg){
                    jpl::_logger::Logger::getLogger()->print(msg, jpl::_logger::WARNING_JPL);
                }
                inline void debug(std::string msg){
                    jpl::_logger::Logger::getLogger()->print(msg, jpl::_logger::DEBUG_JPL);
                }

            namespace _exceptionhook{

                inline void on_terminate(){
                    std::exception_ptr ptr = std::current_exception();
                    try{
                        if (ptr){
                            std::rethrow_exception(ptr);
                        }
                    }catch(const jpl::_exception::AbstractException &e){
                        std::string buffer = e.what();
                        auto* stacktrace = _utils::_debug::getStacktrace(2,512);
                        buffer += _utils::_debug::stktrc_str(stacktrace);
                        _logger::error(buffer);
                    }catch(const std::exception& e){
                        _logger::error(std::string("Unhandled std::exception: ") + e.what());
                    }catch(...){
                        _logger::error("Unhandled unknown exception");
                    }
                }

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