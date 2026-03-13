/**
 * A response http packet
 */
#ifndef RESPONSE_HTTP_JPL
#define RESPONSE_HTTP_JPL

#include "Parameter.hpp"
#include <vector>
#include <jpl/utils/StringUtils.hpp>
#include "HttpStatus.hpp"

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

                    HTTP_STATUS getStatus() const noexcept{
                        return std::atoi(this->headers.at(0)->getValue().c_str());
                    }

                    bool isError() const noexcept{
                        HTTP_STATUS s = this->getStatus();
                        return s >= 400 && s <= 599;
                    }

                    bool isSuccess() const noexcept{
                        HTTP_STATUS s = this->getStatus();
                        return s >= 200 && s <= 299;
                    }

                    ~Response();
                };
                
                

        }
    }
}

#endif