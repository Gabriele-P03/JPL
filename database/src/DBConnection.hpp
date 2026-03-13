/**
 * The main connector to the database.
 * Every connection 
 */

#ifndef DBCONNECTOR_JPL
#define DBCONNECTOR_JPL

#include <SQLAPI.h>
#include <string>
#include <jpl/logger/Logger.hpp>
#include <unordered_map>

namespace jpl{
    namespace _db{

        class DBConnection{

            protected:
                SAConnection* con;


            public:

                /**  
                 * @param server either ip or 'localhost'
                 * @param port  already casted to string (e.g. via std::to_string(int))
                 * @param schema 
                 * @param username
                 * @param password
                 * @param client type of database
                 */
                DBConnection(
                    const std::string &server,
                    const std::string &port, 
                    const std::string &schema,
                    const std::string &username,
                    const std::string &password,
                    SAClient_t client
                );

                SAConnection* const getConnection() const noexcept{
                    return this->con;
                }

                /**
                 * Disconnect from database and free memory
                 */
                ~DBConnection();

            };
        
    }
}

#endif