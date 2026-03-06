/**
 * This is an enumerate class where all DB actions are defined
 */
#ifndef QUERY_METHOD_DB_JPL
#define QUERY_METHOD_DB_JPL

namespace jpl{
    namespace _db{

        enum QUERY_METHOD{
            SELECT,
            INSERT,
            UPDATE,
            CANCEL
        };
    }
}

#endif