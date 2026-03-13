#include "HttpClientAbstract.hpp"

bool jpl::_network::_http::HttpClientAbstract::globalInitialized = false;
long jpl::_network::_http::HttpClientAbstract::counter = 0;

jpl::_network::_http::HttpClientAbstract::HttpClientAbstract(){
    if(!jpl::_network::_http::HttpClientAbstract::globalInitialized){
        throw jpl::_exception::IllegalStateException("globalInit() must be called before instance any HttpClient");
    }
    memset(this->errorBuffer, 0, CURL_ERROR_SIZE);
}

jpl::_network::_http::HttpClientAbstract::~HttpClientAbstract(){
    jpl::_network::_http::HttpClientAbstract::counter--;
    memset(this->errorBuffer, 0, CURL_ERROR_SIZE);
    if(jpl::_network::_http::HttpClientAbstract::counter <= 0){
        curl_global_cleanup();
        jpl::_network::_http::HttpClientAbstract::globalInitialized = false;
    }
}

void jpl::_network::_http::HttpClientAbstract::globalInit(long flags){
    if(jpl::_network::_http::HttpClientAbstract::globalInitialized){
        throw jpl::_exception::IllegalStateException("globalInit() has been already called");
    }
    CURLcode code = curl_global_init(flags);
    if(code != CURLcode::CURLE_OK){
        throw jpl::_exception::RuntimeException("Could not initialized curl globally");
    }
    jpl::_network::_http::HttpClientAbstract::counter = 0;
    jpl::_network::_http::HttpClientAbstract::globalInitialized = true;
    jpl::_logger::info("Curl's Global Initializer called succefully");
}