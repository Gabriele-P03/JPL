/**
 * This is an implementation of IEditable.
 * An object which extends this interface is able to receive either a new string via setString(const &String) or a string to append to the current one via append(const &String).
 * 
 * Actually, this interface could be used even by those objects which does not provide any editing to a rendered-string, but it it up to the programmer to implement a logic
 * to prevent either append(const &String) or setText(const &String) 
 */
#ifndef ITEXTEDITABLE_GRAPHICS_JPL
#define ITEXTEDITABLE_GRAPHICS_JPL

#include "IEditable.hpp"
#include <string>

namespace jpl{
    namespace _graphics{
        namespace _engine{

            class ITextEditable : public IEditable{

                protected:
                    std::string text;

                    ITextEditable(const std::string &text) : text(text){}
                
                public:

                    virtual void append(const std::string &text){
                        this->text.append(text);
                    }

                    virtual void setText(const std::string &text){
                        this->text = text;
                    }

                    std::string getText() const noexcept{
                        return this->text;
                    }
            };
            
        }
    }
}

#endif