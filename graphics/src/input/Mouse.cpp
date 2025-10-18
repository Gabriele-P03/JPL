#include "Mouse.hpp"

double jpl::_graphics::_input::_mouse::xPos;
double jpl::_graphics::_input::_mouse::yPos;
double jpl::_graphics::_input::_mouse::xOffset;
double jpl::_graphics::_input::_mouse::yOffset;

int jpl::_graphics::_input::_mouse::button;
int jpl::_graphics::_input::_mouse::action;
int jpl::_graphics::_input::_mouse::mode;

void jpl::_graphics::_input::_mouse::mouseCursorPosCallback(GLFWwindow* w, double xPos, double yPos){
    jpl::_graphics::_input::_mouse::xPos = xPos;
    jpl::_graphics::_input::_mouse::yPos = yPos;
}

void jpl::_graphics::_input::_mouse::mouseScrollOffsetCallback(GLFWwindow* w, double xOffset, double yOffset){
    jpl::_graphics::_input::_mouse::xOffset = xOffset;
    jpl::_graphics::_input::_mouse::yOffset = yOffset;
}

void jpl::_graphics::_input::_mouse::mouseButtonCallback(GLFWwindow* w, int button, int action, int mode){
    jpl::_graphics::_input::_mouse::button = button;
    jpl::_graphics::_input::_mouse::action = action;
    jpl::_graphics::_input::_mouse::mode = mode;
}