/**
 * @file
 * 
 * A logger provides writing log-program on terminal and even on a file.
 * You should never include directly this header file unless you wanna force using this module.
 * 
 * A reading of its documentation is suggested before using this module since there are some mode of using it
 *  
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
                     * @param quiet quiet mode 
                     */
                    Logger(std::string pathToFile, bool quiet){
                        
                        this->flag = false;

                        if(!quiet){
                            this->file = new std::ofstream();
                            this->file->open(pathToFile);

                            if(file->fail()){
                                #ifndef UFW_LOGGER_JPL   
                                    std::cout<<"UFW Mode is not enabled and Logger could not create output file: "<<pathToFile<<std::endl;
                                    throw new _exception::IOException("Logger file could not be created and OD has not been enabled. I have to exit...");
                                    exit(EXIT_FAILURE);
                                #else
                                    #undef USE_LOGGER_JPL
                                    try{
                                        throw new _exception::IOException("Logger file could not be created but OD has been enabled!" + _utils::_error::_GetLastErrorAsString());
                                    }catch(const _exception::IOException* ex){

                                    }
                                    //std::cout<<"Logger file could not be created but OI has been enabled"<<std::endl;
                                #endif
                            }else{
                                this->flag = true;
                            }
                        }
                    }

                    static Logger* INSTANCE;

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
            };
        }
    } 

#endif