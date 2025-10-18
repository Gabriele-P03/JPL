/**
 * Mouse input interface
 * 
 * The mouse button event callback declared in this header may be useless, in case 
 * of it, dev should not pass it to callback setter
 */

#ifndef MOUSE_GRAPHICS_JPL
#define MOUSE_GRAPHICS_JPL

#include <GLFW/glfw3.h>
#include "../utils/Window.hpp"

namespace jpl{
    namespace _graphics{
        namespace _input{
            
            struct GLFWInputMode;

            /**
             * This function is defined in Keyboard.cpp
             * @param w
             * @param ims array of input mode struct
             * @param imsSize size of ims
             * @throw IllegalArgumentException if w is nullptr
             * @throw IllegalArgumentException if ims is nullptr
             * @throw RuntimeException if any input mode is invalid
             */
            extern void setInputModes(GLFWwindow* w, GLFWInputMode* ims, unsigned int imsSize);

            namespace _mouse{

                /**
                 * x-coordinate of cursor relative to left edge of content area
                 */
                extern double xPos;
                /**
                 * y-coordinate of cursor relative to top edge of content area
                 */
                extern double yPos;

                /**
                 * horizontal scroll offset
                 */
                extern double xOffset;
                /**
                 * vertical scroll offset
                 */
                extern double yOffset;

                extern int button;
                extern int action;
                extern int mode;

                extern void mouseCursorPosCallback(GLFWwindow* w, double xPos, double yPos);
                
                extern void mouseScrollOffsetCallback(GLFWwindow* w, double xOffset, double yOffset);

                extern void mouseButtonCallback(GLFWwindow* w, int button, int action, int mode);

                inline bool isRightButtonPressed(GLFWwindow* w){
                    return glfwGetMouseButton(w, GLFW_MOUSE_BUTTON_RIGHT) == GLFW_PRESS;
                }
                inline bool isRightButtonPressed(){
                    return isRightButtonPressed(_utils::getCurrentContext());
                }
                inline bool isLeftButtonPressed(GLFWwindow* w){
                    return glfwGetMouseButton(w, GLFW_MOUSE_BUTTON_LEFT) == GLFW_PRESS;
                }
                inline bool isLeftButtonPressed(){
                    return isLeftButtonPressed(_utils::getCurrentContext());
                }

            }
        }
    }
}

#endif