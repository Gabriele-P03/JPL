#include "Scene.hpp"

#include "../text/TextInput.hpp"

jpl::_graphics::_engine::Scene::Scene(){
    this->focusedElement = nullptr;
    this->setDeleteOnDiscard(false);
}

void jpl::_graphics::_engine::Scene::clickCallback(GLFWwindow* window, int button, int action, int mods){
    if(this->clickables.empty())
        return;
    if (button == GLFW_MOUSE_BUTTON_LEFT && action == GLFW_PRESS) {
        double mouseX, mouseY;
        glfwGetCursorPos(window, &mouseX, &mouseY);
        mouseY =  ((float)jpl::_graphics::_metrics::height)-mouseY;
        mouseX *= ((float)jpl::_graphics::_metrics::monitorWidth/(float)jpl::_graphics::_metrics::width);
        mouseY *= ((float)jpl::_graphics::_metrics::monitorHeight/(float)jpl::_graphics::_metrics::height);
        for( long i = 0; i < this->clickables.size(); i++ ){
            if (this->focusedElement != nullptr) {
                this->focusedElement->release();
                if (auto* ti = dynamic_cast<_input::TextInput*>(this->focusedElement); ti != nullptr) {
                    ti->onMouseUp();
                }
            }
            if(IClickable* cr = this->clickables.at(i); mouseX >= cr->getX() && mouseX <= cr->getX()+cr->getW() && mouseY >= cr->getY() && mouseY <= cr->getY()+cr->getH()  ){
                jpl::_logger::info("New element focused");
                if (auto* ti = dynamic_cast<_input::TextInput*>(cr); ti != nullptr) {
                    ti->onMouseDown(this->tr, mouseX, mouseY, false);
                    jpl::_logger::info("Which is a textinput");
                }
                cr->click();
                cr->setFocus(true);
                this->focusedElement = cr;
                return;
            }
        }
        //Actually, click has been done on an empty space 
        this->focusedElement = nullptr;
    }
}

void jpl::_graphics::_engine::Scene::keyCallback(GLFWwindow* window, int key, int scancode, int action, int mods){
    if(this->focusedElement != nullptr && action != GLFW_RELEASE){
        if(key == GLFW_KEY_BACKSPACE){
            if(const auto cr = dynamic_cast<_input::TextInput*>(this->focusedElement); cr != nullptr){
                cr->onKey(window, key, scancode, action);
            }
        }
    }
}

void jpl::_graphics::_engine::Scene::charCallback(GLFWwindow* window, unsigned int codepoint){
    if(this->focusedElement != nullptr){
        if(codepoint >= 32 && codepoint <= 126){
            if(const auto cr = dynamic_cast<_input::TextInput*>(this->focusedElement); cr != nullptr){
                cr->onChar(codepoint);
            }
        }
    }
}

void jpl::_graphics::_engine::Scene::scrollCallback(GLFWwindow* window, double xoffset, double yoffset){}

jpl::_graphics::_engine::Scene::~Scene(){
    this->clickables.clear();
}

