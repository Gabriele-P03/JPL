/**
 * Since c++ does not support Reflection, an another way to provides wrapping between models and automatic population of a query is to insert into
 * an unordered_map all fields which must be included
 */

#ifndef MODEL_WRAPPER_DB_JPL
#define MODEL_WRAPPER_DB_JPL

#include <unordered_map>
#include "DBType.hpp"

namespace jpl{
    namespace _db{


        class ModelWrapper{

            protected:

                std::unordered_map<std::string, DBType> fields;

                ModelWrapper() = default;

                void registerField(DBType field);

                virtual void registerModel() = 0;

        };
        
    }
}

#endif