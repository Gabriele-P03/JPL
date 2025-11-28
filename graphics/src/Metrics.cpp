#include "Metrics.hpp"

unsigned int jpl::_graphics::_metrics::height;
unsigned int jpl::_graphics::_metrics::width;
unsigned int jpl::_graphics::_metrics::viewportHeight;
unsigned int jpl::_graphics::_metrics::viewportWidth;
unsigned int jpl::_graphics::_metrics::x;
unsigned int jpl::_graphics::_metrics::y;
unsigned int jpl::_graphics::_metrics::monitorHeight = jpl::_graphics::_metrics::getMonitorHeight();
unsigned int jpl::_graphics::_metrics::monitorWidth = jpl::_graphics::_metrics::getMonitorWidth();

unsigned int jpl::_graphics::_metrics::getMonitorWidth(){
    #ifdef _WIN32
        return GetSystemMetrics(SM_CXSCREEN);
    #else
    #endif
}
unsigned int jpl::_graphics::_metrics::getMonitorHeight(){
    #ifdef _WIN32
        return GetSystemMetrics(SM_CYSCREEN);
    #else
    #endif
}

void jpl::_graphics::_metrics::windowSizeCallback(GLFWwindow* window, int width, int height){
    jpl::_graphics::_metrics::width = width;
    jpl::_graphics::_metrics::height = height;
    glViewport(0,0,width, height);
}

void jpl::_graphics::_metrics::windowViewportSizeCallback(GLFWwindow* window, int width, int height){
    jpl::_graphics::_metrics::viewportWidth = width;
    jpl::_graphics::_metrics::viewportHeight = height;
}

void jpl::_graphics::_metrics::windowPositionCallback(GLFWwindow* window, int x, int y){
    jpl::_graphics::_metrics::x = x;
    jpl::_graphics::_metrics::y = y;
}