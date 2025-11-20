#include "src/shaders/Shader.hpp"
#include <jpl/logger/LoggerWrapper.hpp>
#include "src/utils/Hints.hpp"
#include "src/Metrics.hpp"
#include "src/input/Keyboard.hpp"
#include "src/input/Mouse.hpp"
#include "src/shaders/ProgramManager.hpp"
#include <jpl/parser/bmp/BMP.hpp>
#include "src/texture/Texture.hpp"
#include "src/engine/Painter.hpp"
#include "src/shapes/Cube.hpp"
#include "src/engine/camera/PerspectiveCamera.hpp"
#include <glm/gtc/type_ptr.hpp>

#include "src/engine/text/TextRender.hpp"
#include "src/Metrics.hpp"

#include "src/engine/button/Button.hpp"

jpl::_graphics::_engine::_camera::PerspectiveCamera* camera = new jpl::_graphics::_engine::_camera::PerspectiveCamera();


bool firstMouse = true;
float lastX = 0.0f, lastY = 0.0f;
void cursor_callback(GLFWwindow* w, double x, double y){
    if(firstMouse){
        lastX = x;
        lastY = y;
        firstMouse = false;
    }
    float xoffset = x - lastX;
    float yoffset = lastY - y;
    lastX = x;
    lastY = y;
    xoffset *= 0.1f;
    yoffset *= 0.1f;
    camera->rotate(xoffset, yoffset);
}

void button_callback(GLFWwindow* w, int k, int s, int a, int m){
    if(k == GLFW_KEY_F11 && a == GLFW_PRESS){
        if(jpl::_graphics::_states::fullscreen){
            jpl::_graphics::_utils::setCurrentWindowToWindowMode();
            std::cout<<"Windowed";
        }else{
            jpl::_graphics::_utils::setCurrentWindowToFullScreen();
            std::cout<<"FS";
        }
    }
    if(k == GLFW_KEY_W){
        camera->translate(0.0f, 0.0f, 0.5f);
    }
    if(k == GLFW_KEY_S){
        camera->translate(0.0f, 0.0f, -0.5f);
    }
    if(k == GLFW_KEY_D){
        camera->translate(0.5f, 0.0f, 0.0f);
    }
    if(k == GLFW_KEY_A){
        camera->translate(-0.5f, 0.0f, 0.0f);
    }
    if(k == GLFW_KEY_SPACE){
        camera->translate(0.0f, 0.5f, 0.0f);
    }
    if(k == GLFW_KEY_LEFT_SHIFT){
        camera->translate(0.0f, -0.5f, 0.0f);
    }

    glm::vec3 pos = *camera->getPos();
    std::cout<<"Camera Pos: "<<pos.x<<" "<<pos.y<<" "<<pos.z<<std::endl;
}

int main(){
    jpl::_graphics::_states::fullscreen = false;
    if(!glfwInit()){
        throw jpl::_exception::RuntimeException(jpl::_graphics::_error::getLastErrorAsString());
    }

    jpl::_graphics::_hints::HintBase** hints = new jpl::_graphics::_hints::HintBase*[2];
    hints[0] = new jpl::_graphics::_hints::Hint<int>(GLFW_RESIZABLE, GLFW_TRUE);
    jpl::_graphics::_hints::setHints(hints, 1);

    GLFWwindow* w = glfwCreateWindow(
        jpl::_graphics::_metrics::monitorWidth,
        jpl::_graphics::_metrics::monitorHeight,
        "Test",
        NULL, NULL
    );

    if(!w){
        throw jpl::_exception::RuntimeException(jpl::_graphics::_error::getLastErrorAsString());
    }

    glfwSetKeyCallback(w, button_callback);
    glfwSetCursorPosCallback(w, cursor_callback);
    glfwMakeContextCurrent(w);

    if(GLEW_OK != glewInit()){
        jpl::_logger::error("GLEW could not be initialized");
        return 0;
    }

    //glfwSetInputMode(w, GLFW_CURSOR, GLFW_CURSOR_HIDDEN);
    glfwShowWindow(w);
    int w1, h1;
    glfwGetWindowSize(w, &w1, &h1);
    jpl::_graphics::_metrics::width = w1;
    jpl::_graphics::_metrics::height = h1;
    glfwSetWindowSizeCallback(w, jpl::_graphics::_metrics::windowSizeCallback);

    jpl::_graphics::_engine::Painter* painter = new jpl::_graphics::_engine::Painter();
    painter->pushData(jpl::_graphics::_mesh::CUBE, GL_STATIC_DRAW);

    jpl::_graphics::_shaders::ProgramManager* manager = new jpl::_graphics::_shaders::ProgramManager();
    jpl::_graphics::_engine::_text::ARIALS = new jpl::_graphics::_engine::_text::Font("ascii.bmp", jpl::_graphics::_engine::_text::CHARSET::ASCII, 16, 8, 128);
    jpl::_graphics::_engine::_text::TextRender* tr = new jpl::_graphics::_engine::_text::TextRender(0, 0, 100, 200);
    tr->initializeProgramShaders(manager, "vertex_tr.glsl", "fragment_tr.glsl", new jpl::_graphics::_engine::Painter());
    tr->setFont(jpl::_graphics::_engine::_text::ARIALS);
    tr->updateCoords();
    tr->setColors(1.0f, 0.0f, 0.5f, 0.6f);

    jpl::_graphics::_engine::_button::Button* button = new jpl::_graphics::_engine::_button::Button(300, 100, 400, 100);
    button->setTextRender(tr);
    button->setText("Premi!");
    button->setOnClick( [&](){
        button->setText("Sto cazzo");
    });

    while(!glfwWindowShouldClose(w)){
        glfwPollEvents();
        glfwSwapBuffers(w);
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
        //glUseProgram(programShaders->getProgramIndex());
        if(jpl::_graphics::_input::_keyboard::isKeyPressed(GLFW_KEY_ESCAPE)){
            glfwSetWindowShouldClose(w, 1);
        }

        if(jpl::_graphics::_input::_mouse::isLeftButtonPressed()){
            double x = 0.0d, y = 0.0d;
            glfwGetCursorPos(w, &x, &y);
            if(x >= button->getPosX() && x <= button->getPosX()+button->getWidth() && y >= button->getPosY() && y <= button->getPosY()+button->getHeight()){
                button->click();
            }
        }

        tr->setDim(0, 0, 300,50);
        tr->updateCoords();
        tr->setText("0ARctL]no");
        tr->render();
        button->render();
    }

    glfwTerminate();
}