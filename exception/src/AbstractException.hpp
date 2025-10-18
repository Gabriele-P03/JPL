/**
 * @file
 * 
 * An AbstractException is a template of a basic exception used by JPL to have theirs to be thrown.
 * 
 * @date 2023-04-16
 * @copyright Copyright (c) 2023
 * @author Gabriele-P03
 */

#ifndef ABSTRACT_EXCEPTION_JPL
#define ABSTRACT_EXCEPTION_JPL

    #include <iostream>
    #include <vector>
    #include <string.h>

    namespace jpl{

        namespace _exception{

            class AbstractException;


            class AbstractException : protected std::exception{

                protected:
                    
                    /**
                     * This is a description of the exception which has been thrown. It is passed by constructor
                     * and it is as the exception name
                     * 
                     */
                    std::string type_ex;

                    /**
                     * It is a description of what has just happened. For some exception it may be a template
                     * of stuff (usually when constructor does not need any msg)
                     */
                    std::string msg;   

                    AbstractException(std::string type_ex, unsigned long skip) : AbstractException(type_ex, "", skip){}
                    AbstractException(std::string type_ex, std::string msg, unsigned long skip) : type_ex(type_ex), msg(msg){
                    }
                    
                    
                public:
                    
                    /**
                     * @brief Print the type of the exception which has been thrown with the message that has been passed, too
                     * 
                     * @return const char* 
                     */
                    inline virtual const char* what() const _GLIBCXX_TXN_SAFE_DYN _GLIBCXX_NOTHROW override{
                        std::string buffer = 
                            std::string(this->type_ex) + ": " + std::string(this->msg);
                        char* c_buffer = new char[buffer.size()];
                        strcpy(c_buffer, buffer.c_str());
                        return c_buffer;
                    }

                    /**
                     * @brief 
                     * 
                     * @param os 
                     * @param iae 
                     * @return std::ostream& 
                     */
                    inline friend std::ostream& operator<<(std::ostream& os, const AbstractException &ex){

                        const char* buffer = ex.what();
                        os<<buffer<<std::endl;
                        delete[] buffer;

                        return os;
                    }
            };
        }
    }
#endif