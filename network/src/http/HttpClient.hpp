/**
 * An http client manages http request against an http server
 */

#ifndef HTTP_CLIENT_JPL
#define HTTP_CLIENT_JPL

#include <jpl/exception/runtime/SocketException.hpp>
#include "HttpClientAbstract.hpp"
#include "Response.hpp"

namespace jpl{
    namespace _network{
        namespace _http{

            class HttpClient : public HttpClientAbstract{

                protected:
                    CURL* curl;

                    void executeRequest(char* url);

                    static char* extractUrlStringFromCURLU(CURLU* curlu);

                    static CURLU* initUrl(const std::string &url, std::vector<Parameter> &pars);

                    static size_t WriteCallback(void* contents, size_t size, size_t nmemb, void* userp);

                    std::string responseBody, responseHeaders;

                public:
                    HttpClient();
                    ~HttpClient();

                    Response* get(const std::string &url, std::vector<Parameter> &pars);
                    Response* post(const std::string &url, std::vector<Parameter> &pars, const char* body, const char* ct, struct curl_slist* headers);

                    CURL* getCurl() const noexcept{
                        return this->curl;
                    }
                };

                

        }
    }
}

#endif