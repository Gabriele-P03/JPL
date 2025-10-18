/**
 * @file DTD.hpp
 * @author Gabriele-P03
 * @date 2025-06-30
 * 
 */

#ifndef DTD_JPL
#define DTD_JPL

#include <jpl/utils/StringUtils.hpp>
#include "GroupElement.hpp"

#include "../common/HTMLCommon.hpp"

namespace jpl{

    namespace _parser{

        namespace _dtd{

            class DTD{

                private:
                    std::string name;
                    Element* root;

                    /**
                     * It stores all elements 
                     */
                    _utils::_collections::_list::LinkedList<Element*> elements;
                public:

                    DTD(std::string name){
                        this->name = name;
                        this->root = nullptr;
                    }

                    std::string getName() const noexcept{
                        return this->name;
                    }

                    /**
                     * @param name 
                     * @return element with the given name; if not found, nullptr is returned
                     */
                    Element* getElementByName(const std::string &name) const noexcept{
                        for(size_t i = 0; i < this->elements.getSize(); i++){
                            Element* cr = this->elements.get(i);
                            if (cr->getName() == name){
                                return cr;
                            }
                        }
                        return nullptr;
                    }

                    /**
                     * Adding element to this DTD
                     * 
                     * @param element
                     * @throw DTDException if element has been already added
                     * @throw DTDException if there's already an element with the same name
                     * @throw NullPointer if element is nullptr
                     */
                    void addElement(Element* element);
            };

            static void parseSingleTag(jpl::_parser::_dtd::DTD* &dtd, std::istream &s);
            static void parseElement(jpl::_parser::_dtd::DTD* &dtd, std::string &s);
            static void parseContentModel();
            static GroupElement* parseGroupElement(std::string &e, DTD* &dtd);
            static Presence parsePresence(std::string &e);

            static bool checkValidName(const std::string &name);
            
            DTD* parse(std::istream &s);

        }
    }
}

#endif