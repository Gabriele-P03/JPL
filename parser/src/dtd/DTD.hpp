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

            };

            static std::string parseSingleTag(std::istream* s);

            DTD* parse(std::istream* s);

        }
    }
}

#endif