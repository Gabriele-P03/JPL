#include "Scene.hpp"

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
        jpl::_logger::debug("Looking for focused element");
        for( long i = 0; i < this->clickables.size(); i++ ){
            jpl::_graphics::_engine::IClickable* cr = this->clickables.at(i);
            if( mouseX >= cr->getX() && mouseX <= cr->getX()+cr->getW() && mouseY >= cr->getY() && mouseY <= cr->getY()+cr->getH()  ){
                cr->click();
                jpl::_logger::debug("New focused element");
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
            jpl::_graphics::_engine::ITextEditable* cr = dynamic_cast<jpl::_graphics::_engine::ITextEditable*>(this->focusedElement);
            if(cr != nullptr){
                std::string tmp = cr->getText();
                if (!tmp.empty()){
                    tmp.pop_back();
                    cr->setText(tmp);
                }
            }
        }
    }
}

void jpl::_graphics::_engine::Scene::charCallback(GLFWwindow* window, unsigned int codepoint){
    if(this->focusedElement != nullptr){
        if(codepoint >= 32 && codepoint <= 126){
            jpl::_graphics::_engine::ITextEditable* cr = dynamic_cast<jpl::_graphics::_engine::ITextEditable*>(this->focusedElement);
            if(cr != nullptr){
                std::string tmp = cr->getText();
                tmp += (char)codepoint;
                cr->setText(tmp);
            }
        }
    }
}

jpl::_graphics::_engine::Scene::~Scene(){
    this->clickables.clear();
}

