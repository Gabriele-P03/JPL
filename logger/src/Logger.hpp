/**
 * This is a thread-safety Logger, providing writing logs both on terminal and into file as well.
 * 
 * Printing on terminal is a on-demand task carried out by the current thread (i.e. one which called print()). 
 * On the other side, writing on file, if enabled, is carried out by a dedicated thread; this approach has been choosen since I/O tasks would have tend to stop
 * threads, also several flush per time would happened. Therefore, the special thread delegated to flush file is being sleeping via Conditional Variable and
 * waken up by a thread when its logging-request make reach the maximum amount of request per flushing.
 * This operation needs a Queue in order to hold request in their arrival order.
 * 
 * MAX_LOG_REQUEST_PER_FLUSH_JPL represents the max amount of log request per flush, you are free to change it
 * 
 * Writing on file is not a mandatory task and you can inibite it passing true as quiet field in constructor.
 * Notice that, in case of error during output file creation, flag field is reset and, although you passed true, delegated thread won't write on file.
 * Delegated thread is spawn only in case of positive result of output file creation.
 * 
 * A standard static Logger instance is already available, in order to instance it you have to call initStaticLogger() 
 * 
 * In the _exceptionhook namespace you can find out an exception hook as well, calling LoggerExceptionHook() it set the hook for every unhandled exception and log its stacktrace automagically
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

    #include <queue>
    #include <string>
    #include <fstream>
    #include <iostream>
    #include <ctime>
    #include <jpl/utils/FilesUtils.hpp>
    #include <jpl/utils/debug/DebugUtils.hpp>

    #include <condition_variable>
    #include <thread>
    #include <mutex>

    #define MAX_LOG_REQUEST_PER_FLUSH_JPL 5

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

                    static void writeOnFile(Logger* const instance);

                    std::mutex logger_mutex;
                    std::thread delegatedThread;
                    std::condition_variable cv;
                    std::queue<std::string> requestQueue;

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

                    //Logger's output file
                    std::ofstream* file;
                    
                    //It represents the result of creating file
                    bool flag;

                    //It is set by destructor in order to let delegated thread knows to close file after have flushed last logger requests left
                    bool close;

                    const size_t size;

                    static Logger* INSTANCE;
                    
                public:

                    /**
                     * Create a new instance of Logger associated with the output file at the given path
                     * 
                     * @param pathToFile aboslute path of the file which is wanted to be used as output one
                     * @param quiet quiet mode if you don't want to write on file
                     * @param size amount of logging request that must be performed before each flushing operation. 0 means flushing on each request
                     */
                    Logger(const std::string& pathToFile, bool quiet, const size_t size);

                    /**
                     * @brief Print msg on terminal
                     * Write msg on file and flushes it.
                     * 
                     * @param msg message
                     */
                    void print(const std::string& msg);

                    /**
                     * @brief write msg on terminal
                     * Write msg on file and flushes it.
                     * 
                     * @param msg message
                     * @param status status of the message
                     */
                    void print(const std::string& msg, const LOG_STATUS status);

                    /**
                     * @return if Logger is able to write on file, specially if it had not been able to create output file
                     */
                    bool isWriting(){ return this->flag; }

                    /**
                     * Forced Waking-Up Delegated Theread
                     */
                    void flush();

                    /**
                     * Close the Logger, it call flush() as well
                     * It is not needed to call destructor then  
                     */
                    void flushAndClose();

                    ~Logger();

                    static void initStaticLogger();
                    static Logger* const getLogger(){
                        return const_cast<Logger* const>(INSTANCE);
                    }
            };

                inline void print(const std::string& msg, const jpl::_logger::LOG_STATUS status){
                    jpl::_logger::Logger::getLogger()->print(msg, status);
                }
                inline void info(const std::string& msg){
                    jpl::_logger::Logger::getLogger()->print(msg, jpl::_logger::INFO_JPL);
                }
                inline void error(const std::string& msg){
                    jpl::_logger::Logger::getLogger()->print(msg, jpl::_logger::ERROR_JPL);
                }
                inline void warning(const std::string& msg){
                    jpl::_logger::Logger::getLogger()->print(msg, jpl::_logger::WARNING_JPL);
                }
                inline void debug(const std::string& msg){
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