#include "Scene.hpp"

jpl::_graphics::_engine::Scene::Scene(){
    this->buttons = nullptr;
}

void jpl::_graphics::_engine::Scene::clickCallback(GLFWwindow* window, int button, int action, int mods){
    jpl::_logger::info("Mouse clicked: " + std::to_string(button) + " - " + std::to_string(action));
    if(this->buttons == nullptr){
        return;
    }
    if (button == GLFW_MOUSE_BUTTON_LEFT && action == GLFW_PRESS) {
        double mouseX, mouseY;
        glfwGetCursorPos(window, &mouseX, &mouseY);
        mouseY =  ((float)jpl::_graphics::_metrics::height)-mouseY;
        jpl::_logger::info("Pre-scale -> X: " + std::to_string(mouseX) + " Y: " + std::to_string(mouseY));

        float scaleX = ((float)jpl::_graphics::_metrics::monitorWidth/(float)jpl::_graphics::_metrics::width);
        float scaleY = ((float)jpl::_graphics::_metrics::monitorHeight/(float)jpl::_graphics::_metrics::height);

        float xscale, yscale;
        glfwGetWindowContentScale(window, &xscale, &yscale);
        mouseX /= xscale;
        mouseY /= yscale;

        jpl::_logger::debug("Monitor: " + std::to_string(jpl::_graphics::_metrics::monitorWidth) + " - " + std::to_string(jpl::_graphics::_metrics::monitorHeight) );
        jpl::_logger::debug("Window: " + std::to_string(jpl::_graphics::_metrics::width) + " - " + std::to_string(jpl::_graphics::_metrics::height) );
        jpl::_logger::debug("WindowScale: " + std::to_string(xscale) + " - " + std::to_string(yscale) );
        jpl::_logger::debug("Scale: " + std::to_string(scaleX) + " - " + std::to_string(scaleY) );

        mouseX *= scaleX;
        mouseY *= scaleY;
        jpl::_logger::info("Pst-scale -> X: " + std::to_string(mouseX) + " Y: " + std::to_string(mouseY));
        for( long i = 0; i < this->buttons->size(); i++ ){
            jpl::_graphics::_engine::_button::Button* cr = this->buttons->at(i);
            if( mouseX >= cr->getX() && mouseX <= cr->getX()+cr->getW() && mouseY >= cr->getY() && mouseY <= cr->getY()+cr->getH()  ){
                cr->click();
                break;
            }
        }
    }
}

jpl::_graphics::_engine::Scene::~Scene(){
    if(this->buttons != nullptr){
        delete this->buttons;
    }
}

