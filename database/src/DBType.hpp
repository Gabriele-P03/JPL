/**
 * A DBType extracts database's constraints information about a certain field (i.e. uniqueness, nullable, FK constraints, PK and so on)
 */
#ifndef DBTYPE_JPL
#define DBTYPE_JPL

#include <variant>
#include <jpl/utils/structure/list/LinkedList.hpp>
#include <string>

namespace jpl{
    namespace _db{

        class ModelWrapper;

        class DBType{

            protected:
                using DB_TYPE = std::variant<int*, long*, short*, float*, double*, bool*, std::string*, unsigned char**, bool*>;
                
                const std::string pojoName;
                const std::string columnName;
                bool unique;
                DB_TYPE const type;

                bool primaryKey;
                bool autoIncrement;
                std::unordered_map<ModelWrapper*, jpl::_utils::_collections::_list::LinkedList<DBType*>> foreignKeys;

            public:

                DBType(const std::string &pojoName, const std::string &columnName, bool unique, DB_TYPE type)
                    : pojoName(pojoName), columnName(columnName), unique(unique), type(type){
                }

                std::string getPojoName() const noexcept{
                    return this->pojoName;
                }

                std::string getColumnName() const noexcept{
                    return this->columnName;
                }

                bool isUnique() const noexcept{
                    return this->unique;
                }

                DB_TYPE const getType() const noexcept{
                    return this->type;
                }
        };
    }
}

#endif