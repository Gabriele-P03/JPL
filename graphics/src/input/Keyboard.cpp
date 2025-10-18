#include "Keyboard.hpp"

int jpl::_graphics::_input::_keyboard::key;
int jpl::_graphics::_input::_keyboard::scancode;
int jpl::_graphics::_input::_keyboard::action;
int jpl::_graphics::_input::_keyboard::mods;

void jpl::_graphics::_input::_keyboard::keyboardKeyCallback(GLFWwindow* w, int key, int scancode, int action, int mods){
    jpl::_graphics::_input::_keyboard::key = key;
    jpl::_graphics::_input::_keyboard::scancode = scancode;
    jpl::_graphics::_input::_keyboard::action = action;
    jpl::_graphics::_input::_keyboard::mods = mods;
}

int jpl::_graphics::_input::_keyboard::getStateByKeyAndWindow(GLFWwindow* w, int key){
    if(w == nullptr){
        throw jpl::_exception::IllegalArgumentException("Window is nullptr");
    }
    int state = glfwGetKey(w, key);
    if(state != GLFW_PRESS && state != GLFW_RELEASE){
        throw jpl::_exception::RuntimeException(jpl::_graphics::_error::getLastErrorAsString());
    }
    return state;
}

int jpl::_graphics::_input::_keyboard::getStateByKeyFromCurrentWindow(int key){
    return jpl::_graphics::_input::_keyboard::getStateByKeyAndWindow(jpl::_graphics::_utils::getCurrentContext(), key);
}

jpl::_graphics::_input::GLFWInputMode::GLFWInputMode(int mode, int action) : mode(mode), value(value){}

void jpl::_graphics::_input::setInputModes(GLFWwindow* w, GLFWInputMode* ims, unsigned int imsSize){
    if(w == nullptr){
        throw jpl::_exception::IllegalArgumentException("Window is nullptr");
    }
    if(ims == nullptr){
        throw jpl::_exception::IllegalArgumentException("Array of GLFWInputMode is nullptr");
    }
    for(size_t i = 0; i < imsSize; i++){
        GLFWInputMode cr = ims[i];
        glfwSetInputMode(w, cr.mode, cr.value);
        jpl::_graphics::_error::GLFWErrorJPL* er;
        std::string errStr = jpl::_graphics::_error::getLastErrorAsString(&er);
        if(er->errorCode != 0){
            throw jpl::_exception::RuntimeException("Setting " + std::to_string(cr.mode) + " input mode with value " + std::to_string(cr.value) + " throw exception: " + errStr);
        }
    }
    
}