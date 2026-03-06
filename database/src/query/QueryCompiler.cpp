#include "QueryCompiler.hpp"

jpl::_db::QueryCompiler::QueryCompiler(const SAClient_t client, const jpl::_db::QUERY_METHOD& method, jpl::_db::ModelWrapper* const model) : client(client), method(method){
    if(model == nullptr){
        throw jpl::_exception::IllegalArgumentException("A model is mandatory for compiling a query: nullptr passed");
    }
    this->model = model;
}

std::string jpl::_db::QueryCompiler::getMethodString() const{
    switch (this->method)
    {
        case jpl::_db::QUERY_METHOD::SELECT:
            return "SELECT";
        case jpl::_db::QUERY_METHOD::INSERT:
            return "INSERT";
        case jpl::_db::QUERY_METHOD::UPDATE:
            return "UPDATE";
        case jpl::_db::QUERY_METHOD::CANCEL:
            return "DELETE";
        default:
            break;
    }
    throw jpl::_exception::RuntimeException("COuld not retrieve a valid query method");
}

std::string jpl::_db::QueryCompiler::getFields() const{
    std::stringstream ss;

    return ss.str();
}

const jpl::_db::Query* const jpl::_db::QueryCompiler::compile() const {
    std::stringstream ss;
    ss << this->getMethodString() << " ";

    return new jpl::_db::Query(ss.str());
}