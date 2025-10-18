/**
 * A Group Element rapresents a sub-group of elements in a content-model, it is enclosed by 
 * brackets.
 * 
 * @file GroupElement.hpp
 * @date 06-09-2025
 * @author Gabriele-P03
 */

#ifndef DTD_GROUPELEMENT_HPP
#define DTD_GROUPELEMENT_HPP

#include "Element.hpp"
#include "ElementPresence.hpp"
#include "DTDException.hpp"
#include <jpl/utils/structure/list/LinkedList.hpp>

namespace jpl{
    namespace _parser{
        namespace _dtd{

            enum Amount{

                CHOISE = '|',
                SEQUENCE = ','
            };

            class GroupElement{

                private:

                    const Amount amount;
                    const Presence presence;
                    jpl::_utils::_collections::_list::LinkedList<ElementPresence*> elements;
                    jpl::_utils::_collections::_list::LinkedList<GroupElement*> subGroups;

                public:

                GroupElement(const Amount &amount, const Presence &presence) : amount(amount), presence(presence){}

                /**
                 * Add new element presence to the list
                 * 
                 * @param e
                 * 
                 * @throw NullPointerException if e is nullptr
                 * @throw RuntimeException if list already contains an element named as e
                 */
                void addElement(ElementPresence* e);

                /**
                 * Add new group element to the list
                 * 
                 * @param eg
                 * 
                 * @throw NullPointerException if eg is nullptr
                 * @throw DTDExceptionException if list alreadt contains the same pointer
                 */
                void addGroupElement(GroupElement* eg);

                /**
                 * @param name
                 * @return if this group contains an element named as name
                 */
                bool hasElement(const std::string &name) const noexcept;

                /**
                 * @param name
                 * @throw NotFoundException if no element named as name is found
                 * @return the element named as name which is contained in the list
                 */
                const ElementPresence* getElement(const std::string &name) const;

                size_t getSubGroupElementAmount() const noexcept{
                    return this->subGroups.getSize();
                }

                const Amount getAmount() const noexcept{
                    return this->amount;
                }
                const Presence getPresence() const noexcept{
                    return this->presence;
                }

                ~GroupElement(){
                    this->elements.clear();
                    this->subGroups.clear();
                }
                    
            };
        }
    }
}

#endif