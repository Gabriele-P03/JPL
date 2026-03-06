/**
 * A parameter represents a classic Http Request parameter into URL in key-value form
 */
#ifndef HTTP_PARAMETER_QUERY_JPL
#define HTTP_PARAMETER_QUERY_JPL

#include <string>
#include <jpl/utils/StringUtils.hpp>
#include <jpl/exception/runtime/IllegalArgumentException.hpp>

namespace jpl{
    namespace _network{
        namespace _http{

            class Parameter{

                private:
                    const std::string key;
                    const std::string value;

                public:
                    Parameter(const std::string &key, const std::string &value)
                        :key(jpl::_utils::_string::trim(key)), value(jpl::_utils::_string::trim(value)){
                        if(this->key.size() <= 0){
                            throw jpl::_exception::IllegalArgumentException("Key cannot be empty");
                        }
                    }

                    const std::string getKey() const noexcept{
                        return this->key;
                    }
                    const std::string getValue() const noexcept{
                        return this->value;
                    }
            };
        }
    }
}

#endif