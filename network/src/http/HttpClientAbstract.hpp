/**
 * An HttpClientAbstract is a super class of bith HttpClient and HttpClientAsync.
 * It is used to manage calls to global libcurl's functions such as curl_global_init() and curl_global_cleanup(); since they msut be called once,
 * a static field and counter are implemented in this class
 */

#ifndef HTTP_CLIENT_ABSTRACT_JPL
#define HTTP_CLIENT_ABSTRACT_JPL

#include <curl/curl.h>
#include <jpl/exception/runtime/IllegalStateException.hpp>

namespace jpl{
    namespace _network{
        namespace _http{

            class HttpClientAbstract{

                protected:
                    
                    static bool globalInitialized;
                    static long counter;

                    char errorBuffer[CURL_ERROR_SIZE];

                    HttpClientAbstract();
                    ~HttpClientAbstract();

                public:

                    std::string getLastError() noexcept{
                        return std::string(errorBuffer);
                    }

                    static void globalInit(long flags);
                    
                    static bool isGlobalInitialized() noexcept{
                        return globalInitialized;
                    }
                    static long getCounter() noexcept{
                        return counter;
                    }
            };
                
        }
    }
}

#endif