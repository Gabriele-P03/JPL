/**
 * @file
 * This implementation of Abstract Column is used to prevent template specialization when
 * instancing a list of Column 
 * 
 * @author Gabriele-P03
 */

#ifndef ABSTRACT_COLUMN_JPL
#define ABSTRACT_COLUMN_JPL

#include <jpl/logger/Logger.hpp>
#include <string>
#include <jpl/exception/runtime/IllegalArgumentException.hpp>
#include <jpl/exception/runtime/IllegalCastException.hpp>
#include "../Tuple.hpp"

namespace jpl{
    namespace _utils{
        namespace _collections{
            
            class AbstractColumn{

                protected:

                    bool mandatory;
                    unsigned int index;
                    std::string name;

                    void setIndex(unsigned int index) noexcept {this->index = index;}
                    
                public:

                    friend class Table;
                    friend class TableWrapper;

                    AbstractColumn(std::string name, bool mandatory){
                        this->name = name;
                        this->mandatory = mandatory;
                        this->index = 0;
                    }

                    

                    void setName(std::string name){
                        if(name.empty())
                            throw new jpl::_exception::IllegalArgumentException("A column cannot have an empty name");
                        this->name = name;
                    }
                    std::string getName() const noexcept{return this->name;}

                    void setMandatory(bool mandatory) noexcept {this->mandatory = mandatory;} 
                    bool isMandatory() const noexcept {return this->mandatory;}
                    size_t getIndex() const noexcept {return this->index;}

                    virtual ~AbstractColumn() = default;
                    
            };
        }
    }
}

#endif