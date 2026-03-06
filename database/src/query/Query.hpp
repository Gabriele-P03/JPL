/**
 * A DB Query can be written either by-hand or as QueryCompiler's compile(void) output.
 * It is recommended to let QueryCompiler to generate queries since it has a wrapper module for any DBMS and also provides security checks
 * about SQLInjection  
 */
#ifndef QUERY_DB_JPL
#define QUERY_DB_JPL

#include <string>

namespace jpl{
    namespace _db{

        class Query{

            private:
                const std::string query;

            public:
                Query(const std::string &query)
                    : query(query){

                }

                std::string getQuery() const noexcept{
                    return this->query;
                }
        };
    }
}

#endif