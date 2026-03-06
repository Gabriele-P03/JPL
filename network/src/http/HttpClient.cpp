#include "HttpClient.hpp"

jpl::_network::_http::HttpClient::HttpClient() : jpl::_network::_http::HttpClientAbstract(){
    this->curl = curl_easy_init();
    if(this->curl == NULL){
        throw jpl::_exception::RuntimeException("Could not initialized easy Curl: " + this->getLastError());
    }
    curl_easy_setopt(this->curl, CURLOPT_WRITEFUNCTION, &WriteCallback);
    curl_easy_setopt(this->curl, CURLOPT_WRITEDATA, &responseBody);
    curl_easy_setopt(this->curl, CURLOPT_HEADERFUNCTION, &WriteCallback);
    curl_easy_setopt(this->curl, CURLOPT_HEADERDATA, &responseHeaders);
    curl_easy_setopt(this->curl, CURLOPT_ERRORBUFFER, this->errorBuffer);
}

jpl::_network::_http::HttpClient::~HttpClient(){
    if(this->curl != NULL){
        curl_easy_cleanup(this->curl);
    }
    jpl::_network::_http::HttpClientAbstract::~HttpClientAbstract();
}

void jpl::_network::_http::HttpClient::executeRequest(char* url){
    curl_easy_setopt(this->curl, CURLOPT_URL, url);
    CURLcode res = curl_easy_perform(this->curl);
    if(res != CURLE_OK){
        throw new jpl::_exception::RuntimeException(this->getLastError());
    }
}

size_t jpl::_network::_http::HttpClient::WriteCallback(void* contents, size_t size, size_t nmemb, void* userp){
    size_t totalSize = size * nmemb;
    std::string* s = static_cast<std::string*>(userp);
    s->append(static_cast<char*>(contents), totalSize);
    return totalSize;
}

CURLU* jpl::_network::_http::HttpClient::initUrl(const std::string &url, std::vector<jpl::_network::_http::Parameter> &pars){
    CURLU* curlurl = curl_url();
    if(!curlurl){
        throw jpl::_exception::RuntimeException("Could not create new curl url");
    }
    curl_url_set(curlurl, CURLUPART_URL, url.c_str(), 0);
    for(long i = 0; i < pars.size(); i++){
        const jpl::_network::_http::Parameter cr = pars.at(i);
        std::string par = cr.getKey() + "=" + cr.getValue();
        curl_url_set(curlurl, CURLUPART_QUERY, par.c_str(), CURLU_APPENDQUERY);
    }
    return curlurl;
}

char* jpl::_network::_http::HttpClient::extractUrlStringFromCURLU(CURLU* curlu){
    CURLUcode code;
    char* buffer;
    code = curl_url_get(curlu, CURLUPART_URL, &buffer, 0);
    if(code != CURLUE_OK){
        throw jpl::_exception::RuntimeException("Could not extract url string from CURLU: " + std::to_string(code));
    }
    return buffer;
}

jpl::_network::_http::Response* jpl::_network::_http::HttpClient::get(const std::string &url, std::vector<jpl::_network::_http::Parameter> &pars){
    CURLU* curlu = jpl::_network::_http::HttpClient::initUrl(url, pars);
    char* url_c = jpl::_network::_http::HttpClient::extractUrlStringFromCURLU(curlu);
    curl_easy_setopt(this->curl, CURLOPT_CUSTOMREQUEST, NULL);
    curl_easy_setopt(this->curl, CURLOPT_HTTPGET, 1L);
    jpl::_network::_http::HttpClient::executeRequest(url_c);
    curl_free(url_c);
    curl_url_cleanup(curlu);
    return new jpl::_network::_http::Response(this->responseBody, this->responseHeaders);
}

jpl::_network::_http::Response* jpl::_network::_http::HttpClient::post(const std::string &url, std::vector<jpl::_network::_http::Parameter> &pars, const char* body, const char* ct, struct curl_slist* headers){
    CURLU* curlu = jpl::_network::_http::HttpClient::initUrl(url, pars);
    char* url_c = jpl::_network::_http::HttpClient::extractUrlStringFromCURLU(curlu);
    curl_easy_setopt(this->curl, CURLOPT_CUSTOMREQUEST, NULL);
    curl_easy_setopt(this->curl, CURLOPT_POST, 1L);
    curl_easy_setopt(this->curl, CURLOPT_POSTFIELDS, body);
    headers = curl_slist_append(headers, ct);
    curl_easy_setopt(this->curl, CURLOPT_HTTPHEADER, headers);
    jpl::_network::_http::HttpClient::executeRequest(url_c);
    curl_free(url_c);
    curl_url_cleanup(curlu);
    curl_slist_free_all(headers); 
    return new jpl::_network::_http::Response(this->responseBody, this->responseHeaders);
}