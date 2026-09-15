#include "Scene.hpp"

jpl::_graphics::_engine::Scene::Scene(){
    this->focusedElement = nullptr;
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
            jpl::_graphics::_engine::IClickable* cr = this->clickables.at(i);
            if( mouseX >= cr->getX() && mouseX <= cr->getX()+cr->getW() && mouseY >= cr->getY() && mouseY <= cr->getY()+cr->getH()  ){
                cr->click();
                this->focusedElement = cr;
                break;
            }
        }
        //Actually, click has been done on an empty space 
        this->focusedElement = nullptr;
    }
}

void jpl::_graphics::_engine::Scene::keyCallback(GLFWwindow* window, int key, int scancode, int action, int mods){
    if(this->focusedElement != nullptr){
        if(mods != GLFW_RELEASE && key >= 32 && key <= 126){
            jpl::_graphics::_engine::ITextEditable* cr = dynamic_cast<jpl::_graphics::_engine::ITextEditable*>(this->focusedElement);
            if(cr != nullptr){
                //ITextEditable#append(const &String) is not called since it would not call derived setText(const &String)
                cr->setText(cr->getText()+(char)key);
            }
        }
    }
}

jpl::_graphics::_engine::Scene::~Scene(){
    this->clickables.clear();
}

