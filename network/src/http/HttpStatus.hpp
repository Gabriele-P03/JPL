#ifndef HTTP_STATUS_JPL
#define HTTP_STATUS_JPL

namespace jpl{
    namespace _network{
        namespace _http{

            typedef unsigned int HTTP_STATUS;

            constexpr HTTP_STATUS OK = 200;
            
            constexpr HTTP_STATUS FORBIDDEN = 403;
            constexpr HTTP_STATUS NOT_FOUND = 404;

            constexpr HTTP_STATUS INTERNAL_SERVER_ERROR = 500;
        }
    }
}

#endif