#include "Metrics.hpp"

unsigned int jpl::_graphics::_metrics::height;
unsigned int jpl::_graphics::_metrics::width;
unsigned int jpl::_graphics::_metrics::viewportHeight;
unsigned int jpl::_graphics::_metrics::viewportWidth;
unsigned int jpl::_graphics::_metrics::x;
unsigned int jpl::_graphics::_metrics::y;
unsigned int jpl::_graphics::_metrics::monitorHeight = jpl::_graphics::_metrics::getMonitorHeight();
unsigned int jpl::_graphics::_metrics::monitorWidth = jpl::_graphics::_metrics::getMonitorWidth();
glm::mat4 jpl::_graphics::_metrics::ortho = glm::mat4(1.0f);
glm::mat4 jpl::_graphics::_metrics::perspective = glm::mat4(1.0f);
float jpl::_graphics::_metrics::fov = glm::radians(45.0f);
float jpl::_graphics::_metrics::nearPlane = 0.1f;
float jpl::_graphics::_metrics::farPlane = 100.0f;

float jpl::_graphics::_metrics::aspectRatio;

float jpl::_graphics::_metrics::scaleRatioX;
float jpl::_graphics::_metrics::scaleRatioY;

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

void jpl::_graphics::_metrics::windowFrameBufferCallback(GLFWwindow* window, int width, int height){
    glViewport(0,0,width, height);
    jpl::_graphics::_metrics::width = width;
    if (height == 0)
        height = 1; //Prevent division-per-0 in case of window minimized
    jpl::_graphics::_metrics::height = height;
    jpl::_graphics::_metrics::aspectRatio = ((float)width)/((float)height);
    jpl::_graphics::_metrics::scaleRatioX= (float)width/(float)(jpl::_graphics::_metrics::monitorWidth*jpl::_graphics::_metrics::monitorWidth);
    jpl::_graphics::_metrics::scaleRatioY= (float)height/(float)(jpl::_graphics::_metrics::monitorHeight*jpl::_graphics::_metrics::monitorHeight);      
    jpl::_graphics::_metrics::ortho = glm::ortho(0.0f, static_cast<float>(width), 0.0f, static_cast<float>(height), -1.0f, 1.0f);
    jpl::_graphics::_metrics::perspective = glm::perspective(jpl::_graphics::_metrics::fov, jpl::_graphics::_metrics::aspectRatio, jpl::_graphics::_metrics::nearPlane, jpl::_graphics::_metrics::farPlane);
}

void jpl::_graphics::_metrics::windowViewportSizeCallback(GLFWwindow* window, int width, int height){
    jpl::_graphics::_metrics::viewportWidth = width;
    jpl::_graphics::_metrics::viewportHeight = height;
}

void jpl::_graphics::_metrics::windowPositionCallback(GLFWwindow* window, int x, int y){
    jpl::_graphics::_metrics::x = x;
    jpl::_graphics::_metrics::y = y;
}