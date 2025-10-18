/**
 * 
 * The element presence does store amount of time an element may occurr in a content model 
 * 
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
                const Presence presence;

                ElementPresence(Element* e, const Presence p) : presence(p){
                    this->element = e;
                }

                ~ElementPresence(){
                    
                }
            };
            
        }
    }
}

#endif