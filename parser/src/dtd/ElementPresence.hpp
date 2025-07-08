/**
 * @file Element.hpp
 * @author Gabriele-P03
 * @date 2025-06-30
 * 
 */

#ifndef DTD_ELEMENTPRESENCE_JPL
#define DTD_ELEMENTPRESENCE_JPL

namespace jpl{
    
    namespace _parser{
    
        namespace _dtd{

            class Element;

            enum Presence{

                BINARY = '?',
                ANY = '*',
                POSITIVE = '+'
            };

            struct  ElementPresence
            {
                const Element* element;
                const Presence* presence;
            };
            
        }
    }
}

#endif