#include "Window.hpp"

bool jpl::_graphics::_states::fullscreen;

GLFWwindow* jpl::_graphics::_utils::getCurrentContext(){
    GLFWwindow* cr = glfwGetCurrentContext();
    if(cr == NULL){
        throw jpl::_exception::RuntimeException(jpl::_graphics::_error::getLastErrorAsString());
    }
    return cr;
}

GLFWmonitor* jpl::_graphics::_utils::getCurrentMonitor(){
    return jpl::_graphics::_utils::getMonitorByWindow(jpl::_graphics::_utils::getCurrentContext());
}

GLFWmonitor* jpl::_graphics::_utils::getMonitorByWindow(GLFWwindow* w){
    if(w == NULL){
        throw jpl::_exception::IllegalArgumentException("Given GLFWwindow is null");
    }
    GLFWmonitor* cr = glfwGetWindowMonitor(w);
    if(cr == NULL){
        throw jpl::_exception::RuntimeException(jpl::_graphics::_error::getLastErrorAsString());
    }
    return cr;
}

const GLFWvidmode* jpl::_graphics::_utils::getVidmodeCurrentMonitor(){
    return jpl::_graphics::_utils::getVidmodeByMonitor(jpl::_graphics::_utils::getCurrentMonitor());
}

const GLFWvidmode* jpl::_graphics::_utils::getVidmodeByMonitor(GLFWmonitor* m){
    if(m == NULL){
        throw jpl::_exception::IllegalArgumentException("Given GLFWmonitor is null");
    }
    const GLFWvidmode* cr = glfwGetVideoMode(m);
    if(cr == NULL){
        throw jpl::_exception::RuntimeException(jpl::_graphics::_error::getLastErrorAsString());
    }
    return cr; 
}

void jpl::_graphics::_utils::setCurrentWindowToFullScreen(){
    GLFWwindow* w = jpl::_graphics::_utils::getCurrentContext();
    GLFWmonitor* m = glfwGetPrimaryMonitor();
    const GLFWvidmode* vm = jpl::_graphics::_utils::getVidmodeByMonitor(m);
    glfwSetWindowMonitor(w, m, 0, 0, vm->width, vm->height, vm->refreshRate);//Setting to fullscreen
    _error::GLFWErrorJPL* er;
    std::string errStr = _error::getLastErrorAsString(&er);
    if(er->errorCode != 0){
        throw jpl::_exception::RuntimeException("Passing to fullscreen mode: " + errStr);
    }
    delete er;
    jpl::_graphics::_states::fullscreen = true;
}

void jpl::_graphics::_utils::setCurrentWindowToWindowMode(){
    GLFWwindow* w = jpl::_graphics::_utils::getCurrentContext();
    glfwSetWindowMonitor(w, NULL, 0, 0, jpl::_graphics::_metrics::monitorWidth, jpl::_graphics::_metrics::monitorHeight, 0);
    _error::GLFWErrorJPL* er;
    std::string errStr = _error::getLastErrorAsString(&er);
    if(er->errorCode != 0){
        std::cout<<errStr;
        throw jpl::_exception::RuntimeException("Passing to windowed mode: " + errStr);
    }
    delete er;
    jpl::_graphics::_states::fullscreen = false;
}

void jpl::_graphics::_utils::destroyWindow(GLFWwindow* w){

}