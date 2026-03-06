/**
 * A database is the schema abstraction which includes all tables present in it. 
 * 
 * A schema is stored into an unordered_map whose key is the tables' name and value is a list of DBType
 */
#ifndef DATABASE_DB_JPL
#define DATABASE_DB_JPL

#include <unordered_map>
#include <jpl/logger/LoggerWrapper.hpp>
#include <jpl/utils/structure/list/LinkedList.hpp>
#include "DBType.hpp"

namespace jpl{
    namespace _db{

        class Database{

            protected:
                const std::string schema;
                std::unordered_map<std::string, std::unordered_map<std::string, DBType*>> tables;
        
            
            public:
                Database(const std::string &schema);

                /**
                 * @param tn table name
                 * @return fields of table named as @param tn otherwise nullptr
                 */
                std::unordered_map<std::string, DBType*> getFieldsByTableName(const std::string &tn) const; 

                void addNewTable(const std::string &tn, std::unordered_map<std::string, DBType*> fields);
        };

    }
}

#endif