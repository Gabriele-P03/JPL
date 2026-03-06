/**
 * QueryCompiler takes in input a Model instance and where, group-by and limits clauses and - of course - method...  
 * 
 * Copmile() is the core method of this class which performs checks and, once passed, return a well-formatted query string 
 */
#ifndef QUERY_COMPILER_JPL
#define QUERY_COMPILER_JPL

#include "Query.hpp"
#include <jpl/utils/structure/list/LinkedList.hpp>
#include "../ModelWrapper.hpp"
#include "method/QueryMethod.hpp"
#include <SQLAPI.h>
#include <sstream>

namespace jpl{
    namespace _db{

        class QueryCompiler{

            protected:
                const SAClient_t client;
                const QUERY_METHOD method;
                ModelWrapper* const model;

                virtual std::string getMethodString() const;

                /**
                 * Return a comma separated list of fields which have been declares as available for
                 * the given method.
                 * Notice that DELETE method does not need a fields list 
                 */
                virtual std::string getFields() const;

            public:

                QueryCompiler(
                    const SAClient_t client,
                    const QUERY_METHOD& method,
                    jpl::_db::ModelWrapper* const model
                );

                virtual const Query* const compile() const;
        };
    }
}
#endif