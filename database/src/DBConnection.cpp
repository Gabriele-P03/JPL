#include "DBConnection.hpp"

jpl::_db::DBConnection::DBConnection(const std::string &server, const std::string &port, const std::string &schema, const std::string &username, const std::string &password, SAClient_t client){
    
    try{
        this->con = new SAConnection(new SAPI(client));
    }catch(SAException &e){
        jpl::_logger::error(std::string(e.ErrMessage().GetMultiByteChars()));
        throw e;
    }

    try{
        this->con->Connect(
            _TSA(std::string(server+","+port+"@"+schema).c_str()),
            _TSA(username.c_str()),
            _TSA(password.c_str())
        );
    }catch(SAException &e){
        this->con->Rollback();
        jpl::_logger::error(std::string(e.ErrMessage().GetMultiByteChars()));
        jpl::_logger::info("Since an error occurred during DB connection, a rollback for safety reason has been performed");
        throw e;
    }
    jpl::_logger::info("Connected to " + server + ":" + port + "@" + schema + ". Username: " + username);
}

jpl::_db::DBConnection::~DBConnection(){

}