/**
 * Let you set several hints in a single call
 */
#ifndef HINTS_GRAPHICS_JPL
#define HINTS_GRAPHICS_JPL

#include <GLFW/glfw3.h>
#include "Error.hpp"

namespace jpl{
    namespace _graphics{
        namespace _hints{

            //template<typename T, std::enable_if<std::is_same<int, T>::value || std::is_same<const char*, T>::value>::type = 0 >
            template<typename T>
            concept HintValueType = std::same_as<int,T> || std::same_as<const char*,T>;
            
            struct HintBase{
                const int hint;
                HintBase(int hint) : hint(hint){}
                virtual ~HintBase() = default;  //Declare a virtual destructor in order to allow polymorphism for dynamic_cast
            };

            template<HintValueType HVT>
            struct Hint : HintBase{
                const HVT value;
                Hint(int hint, HVT value) : HintBase(hint), value(value){}
            };

            template<>
            struct Hint<int> : HintBase{
                int value;
                Hint(int hint, int value): HintBase(hint), value(value){}
            };

            template<>
            struct Hint<const char*> : HintBase{
                const char* value;
                Hint(int hint, const char* value): HintBase(hint), value(value){}
            };
            
            inline void setHints(HintBase** hints, int size){
                for(size_t i = 0; i < size; i++){
                    HintBase* cr = hints[i];
                    if(dynamic_cast<Hint<int>*>(cr)){
                        Hint<int>* cr_i = (Hint<int>*)cr;
                        glfwWindowHint(cr_i->hint, cr_i->value);
                    }else{
                        Hint<const char*>* cr_cc = (Hint<const char*>*)cr;
                        glfwWindowHintString(cr_cc->hint, cr_cc->value);
                    }
                    _error::GLFWErrorJPL* er;    //Checking for any error occurred during current hint setting
                    std::string errStr = jpl::_graphics::_error::getLastErrorAsString(&er);
                    if(er->errorCode != 0){
                        throw jpl::_exception::RuntimeException(errStr);
                    }
                    delete er;
                }
            }
        }
    }
}

#endif