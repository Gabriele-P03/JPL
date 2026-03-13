#include <winsock2.h>
#include <jpl/logger/Logger.hpp>
#include "src/http/HttpClient.hpp"

int main(){

    jpl::_logger::Logger::initStaticLogger();
    jpl::_logger::_exceptionhook::LoggerExceptionHook();
    
    jpl::_network::_http::HttpClient::globalInit(CURL_GLOBAL_ALL);
    jpl::_network::_http::HttpClient* client = new jpl::_network::_http::HttpClient(); 

    //only to debug, it prevents from checking certs
    curl_easy_setopt(client->getCurl(), CURLOPT_SSL_VERIFYPEER, 0L);
    curl_easy_setopt(client->getCurl(), CURLOPT_SSL_VERIFYHOST, 0L);

    std::vector<jpl::_network::_http::Parameter> pars = std::vector<jpl::_network::_http::Parameter>();
    jpl::_network::_http::Response* res = client->post("https://localhost:8080/version/all", pars, "",  "Content-Type: application/json", NULL);

    return 0;
}