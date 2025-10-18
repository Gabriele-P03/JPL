/**
 * In this header file will be declared some useful function in order to manage in an abstract 
 * way HTML and its derivated language, such as XML and XSD
 * 
 * @date 09-07-2025
 * @author Gabriele-P03
 */

#ifndef HTML_COMMON_JPL
#define HTML_COMMON_JPL

#include <string>
#include "../dtd/DTDException.hpp"
#include <jpl/utils/StringUtils.hpp>

namespace jpl{

    namespace _parser{

        namespace _common{

            /**
             * It removes comment from line.
             * It is smart enough to remove even when it is a multi-line comment
             * 
             * @param line
             * @param flagComment true if the previous line was comment which is still opened
             * 
             * @return flagComment which can be changed during func execution
             */
            bool removeHTMLComment(std::string &line, bool flagComment) noexcept;
        }
    }
}

#endif