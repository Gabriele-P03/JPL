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

#include "src/mesh/MeshLoader.hpp"

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
    
    std::fstream* file = new std::fstream;
    jpl::_utils::_files::getInternalFile("shaders/vertex.glsl", std::fstream::in | std::fstream::binary, &file);
    jpl::_graphics::_shaders::Shader* vertexShader = new jpl::_graphics::_shaders::Shader(file);
    vertexShader->probeShader(GL_VERTEX_SHADER);
    delete file;
    file = new std::fstream;
    jpl::_utils::_files::getInternalFile("shaders/fragment.glsl", std::fstream::in | std::fstream::binary, &file);
    jpl::_graphics::_shaders::Shader* fragmentShader = new jpl::_graphics::_shaders::Shader(file);
    fragmentShader->probeShader(GL_FRAGMENT_SHADER);
    delete file;
    jpl::_graphics::_shaders::ProgramManager* manager = new jpl::_graphics::_shaders::ProgramManager();
    jpl::_graphics::_shaders::ProgramShaders* programShaders = new jpl::_graphics::_shaders::ProgramShaders();
    programShaders->addNewShader(vertexShader);
    programShaders->addNewShader(fragmentShader);
    manager->addProgramShaders(programShaders);
    glLinkProgram(programShaders->getProgramIndex());

    std::fstream* f = new std::fstream;
    jpl::_utils::_files::getInternalFile("objs\\test.obj", std::ios_base::in, &f);
    jpl::_graphics::_mesh::Mesh* mesh = jpl::_graphics::_mesh::parse(f);

    //jpl::_graphics::_texture::Texture* texture = jpl::_graphics::

    painter->pushData(mesh, GL_STATIC_DRAW);

    while(!glfwWindowShouldClose(w)){
        glfwPollEvents();
        glfwSwapBuffers(w);
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
        //glUseProgram(programShaders->getProgramIndex());
        if(jpl::_graphics::_input::_keyboard::isKeyPressed(GLFW_KEY_ESCAPE)){
            glfwSetWindowShouldClose(w, 1);
        }

        glUseProgram(programShaders->getProgramIndex());
        int modelMLoc = glGetUniformLocation(programShaders->getProgramIndex(), "modelM");
        glUniformMatrix4fv(modelMLoc, 1, false, glm::value_ptr(glm::mat4(1.0f)));
        int viewMLoc = glGetUniformLocation(programShaders->getProgramIndex(), "viewM");
        glUniformMatrix4fv(viewMLoc, 1, false, glm::value_ptr(camera->getLookAt()));
        int projMLoc = glGetUniformLocation(programShaders->getProgramIndex(), "projM");
        glUniformMatrix4fv(projMLoc, 1, false, glm::value_ptr(camera->getProjection()));
        jpl::_graphics::_engine::drawMesh(painter, mesh);
    }

    glfwTerminate();
}