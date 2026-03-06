/**
 * A response http packet
 */
#ifndef RESPONSE_HTTP_JPL
#define RESPONSE_HTTP_JPL

#include "Parameter.hpp"
#include <vector>
#include <jpl/utils/StringUtils.hpp>

namespace jpl{
    namespace _network{
        namespace _http{

            typedef Parameter HeaderParameter; 

            class Response{

                private:
                    std::vector<HeaderParameter*> headers;
                    const std::string responseBody;

                public:

                    Response(const std::string &responseBody, const std::string &responseHeader);

                    std::string getHeader(const std::string &headerParkey) const;

                    std::string getData() const noexcept{
                        return this->responseBody;
                    }

                    ~Response();
                };
                
                

        }
    }
}

#endif