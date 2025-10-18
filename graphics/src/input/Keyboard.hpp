/**
 * Keyboard input interface.
 * 
 * In this header, keyboard callback and integer codes are also stored despite
 * they could be useless. In case of it, dev should not pass callback 
 */
#ifndef INPUT_GRAPHICS_JPL
#define INPUT_GRAPHICS_JPL

#include <GLFW/glfw3.h>
#include <jpl/exception/runtime/IllegalArgumentException.hpp>
#include "../utils/Error.hpp"
#include "../utils/Window.hpp"

namespace jpl{
    namespace _graphics{
        namespace _input{

            //These declarations are in common with Mouse.hpp
            /**
             * Struct which contains a pair of mode-value input mode
             */
            struct GLFWInputMode{
                const int mode;
                const int value;
                GLFWInputMode(int mode, int value);
            };

            /**
             * @param w
             * @param ims array of input mode struct
             * @param imsSize size of ims
             * @throw IllegalArgumentException if w is nullptr
             * @throw IllegalArgumentException if ims is nullptr
             * @throw RuntimeException if any input mode is invalid
             */
            extern void setInputModes(GLFWwindow* w, GLFWInputMode* ims, unsigned int imsSize);

            namespace _keyboard{

                extern int key;
                extern int scancode;
                extern int action;
                extern int mods;

                extern void keyboardKeyCallback(GLFWwindow* w, int key, int scancode, int action, int mods);

                /**
                 * 
                 * @param w window
                 * @param key key
                 * @throw IllegalArgumentException if w is nullptr
                 * @throw RuntimeException for any GLFW error
                 * @return state of key in given window
                 */
                extern int getStateByKeyAndWindow(GLFWwindow* w, int key);
                /**
                 * @param key key
                 * @throw RuntimeException for any GLFW error
                 * @return state of given key in the current window
                 */
                extern int getStateByKeyFromCurrentWindow(int key);

            
                /**
                 * @return if left shift is pressed
                 */
                inline bool isLShifting(){
                    return getStateByKeyFromCurrentWindow(GLFW_KEY_LEFT_SHIFT) == GLFW_PRESS;
                }
                /**
                 * @return if right shift is pressed
                 */
                inline bool isRShifting(){
                    return getStateByKeyFromCurrentWindow(GLFW_KEY_RIGHT_SHIFT) == GLFW_PRESS;
                }
                /**
                 * @return if either left or right shift is pressed 
                 */
                inline bool isShifting(){
                    return isLShifting() || isRShifting();
                }

                inline int getKeyState(GLFWwindow* w, int key){
                    int state = glfwGetKey(w, key);
                    jpl::_graphics::_error::GLFWErrorJPL* er;
                    std::string errStr = jpl::_graphics::_error::getLastErrorAsString(&er);
                    if(er->errorCode != 0){
                        throw jpl::_exception::RuntimeException("Getting State of " + std::to_string(key) + " key thrown exception: " + errStr);
                    }
                    return state;
                }
                inline int getKeyState(int key){
                    return getKeyState(_utils::getCurrentContext(), key);
                }

                inline bool isKeyPressed(GLFWwindow* w, int key){
                    return getKeyState(w, key) == GLFW_PRESS;
                }
                inline bool isKeyPressed(int key){
                    return getKeyState(key) == GLFW_PRESS;
                }
            }
        }
    }
}

#endif