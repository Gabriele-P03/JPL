/**
 * @file DTD.hpp
 * @author Gabriele-P03
 * @date 2025-06-30
 * 
 */

#ifndef DTD_JPL
#define DTD_JPL

#include <jpl/utils/StringUtils.hpp>
#include <jpl/utils/structure/list/LinkedList.hpp>
#include "ElementPresence.hpp"
#include "DTDException.hpp"

namespace jpl{

    namespace _parser{

        namespace _dtd{

            class DTD{

                private:
                    std::string name;
                public:

                    DTD(std::string name){
                        this->name = name;
                    }

                    std::string getName() const noexcept{
                        return this->name;
                    }

            };

            static void parseSingleTag(jpl::_parser::_dtd::DTD* &dtd, std::istream &s);
            static void parseElement(jpl::_parser::_dtd::DTD* &dtd, std::string &s);

            static bool checkValidName(const std::string &name);

            DTD* parse(std::istream &s);

        }
    }
}

#endif