#include <jpl/parser/png/PNG.h>

#include "src/shaders/Shader.hpp"


#include <jpl/logger/Logger.hpp>
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
#include "src/engine/VAOManager.hpp"
#include "src/engine/text/TextRender.hpp"
#include "src/Metrics.hpp"

#include "src/mesh/MeshLoader.hpp"

#include "src/engine/button/Button.hpp"
#include "src/engine/progress/ProgressBar.hpp"

void clickCallback(GLFWwindow* window, int button, int action, int mods){
    jpl::_logger::info("Mouse clicked: " + std::to_string(button) + " - " + std::to_string(action));
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
    }
}



jpl::_graphics::_texture::Texture* loadTexture(const std::string &path);

int main(){

    jpl::_logger::Logger::initStaticLogger();
    jpl::_logger::_exceptionhook::LoggerExceptionHook();

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
    glfwSetMouseButtonCallback(w, clickCallback);
    jpl::_graphics::_engine::Painter* painter = new jpl::_graphics::_engine::Painter();
    
    std::fstream* file = new std::fstream;
    jpl::_utils::_files::getInternalFile("shaders/vertex_tr.glsl", std::fstream::in | std::fstream::binary, &file);
    jpl::_graphics::_shaders::Shader* vertexShader = new jpl::_graphics::_shaders::Shader(file);
    vertexShader->probeShader(GL_VERTEX_SHADER);
    delete file;
    file = new std::fstream;
    jpl::_utils::_files::getInternalFile("shaders/fragment_tr.glsl", std::fstream::in | std::fstream::binary, &file);
    jpl::_graphics::_shaders::Shader* fragmentShader = new jpl::_graphics::_shaders::Shader(file);
    fragmentShader->probeShader(GL_FRAGMENT_SHADER);
    delete file;
    jpl::_graphics::_shaders::ProgramManager* manager = new jpl::_graphics::_shaders::ProgramManager();
    jpl::_graphics::_shaders::ProgramShaders* programShaders = new jpl::_graphics::_shaders::ProgramShaders();
    programShaders->addNewShader("test_v", vertexShader);
    programShaders->addNewShader("test_f", fragmentShader);
    manager->addProgramShaders("test", programShaders);

    programShaders->link();
    programShaders->use();

    jpl::_graphics::_engine::VAOManager vaom = jpl::_graphics::_engine::VAOManager();
    jpl::_graphics::_engine::VAO* vao = vaom.addNewVAO("test");
    jpl::_graphics::_engine::VBO* vbo = vao->addVBO();
    jpl::_graphics::_engine::EBO* ebo = vao->addEBO();

    jpl::_graphics::_engine::_text::TextRender* tr = new jpl::_graphics::_engine::_text::TextRender(programShaders->getProgramIndex(), 1, 1, 1000, 289);
    jpl::_graphics::_engine::_text::Font* font = new jpl::_graphics::_engine::_text::Font(
        loadTexture("fonts/arials.png"), jpl::_graphics::_engine::_text::ASCII, 16,8, 128);
    tr->setFont(font);
    vao->bind();
    vbo->bind();
    ebo->bind();
    jpl::_graphics::_engine::_button::Button* button = new jpl::_graphics::_engine::_button::Button(programShaders->getProgramIndex(), 0, 0, 257, 96, nullptr, loadTexture("test.png"));
    button->setVAOTextRenderer(vao);

    tr->setRGBA(1.0f, 0.6f, 0.4f, 0.5f);
    //tr->setText("JoinS");
    
    glEnable(GL_BLEND);
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

    while(!glfwWindowShouldClose(w)){
        glfwPollEvents();
        
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
        glClearColor(0.5f, 0.0f, 0.1f, 0.5f);
        //glUseProgram(programShaders->getProgramIndex());
        if(jpl::_graphics::_input::_keyboard::isKeyPressed(GLFW_KEY_ESCAPE)){
            glfwSetWindowShouldClose(w, 1);
        }

        vao->bind();
        button->render();

        glfwSwapBuffers(w);
    }

    glfwTerminate();
}

jpl::_graphics::_texture::Texture* loadTexture(const std::string &path){
    jpl::_graphics::_texture::Texture* texture = nullptr;
    if(path.find(".bmp") != std::string::npos){
        std::fstream* file = new std::fstream;
        jpl::_utils::_files::getInternalFile(path, std::ios_base::in | std::ios_base::binary, &file);
        jpl::_parser::_bmp::BMP* bmp = jpl::_parser::_bmp::read(file);
        size_t size;
        char* buffer = new char[bmp->imageSize];
        memcpy(buffer, bmp->data, bmp->imageSize);
        texture = new jpl::_graphics::_texture::Texture(
            bmp->width, bmp->height, 3, buffer, GL_BGR, GL_TEXTURE_2D
        );
        texture->bind();
        texture->generate();
        delete bmp;
        delete file;
    }else{
        std::string internalPath = jpl::_utils::_files::getInternalPath(path);
        int w,h, nrChannels;
        stbi_set_flip_vertically_on_load(true);
        unsigned char* buffer = stbi_load(internalPath.c_str(), &w, &h, &nrChannels, 4); 
        texture = new jpl::_graphics::_texture::Texture(w, h, nrChannels, reinterpret_cast<const char*>(buffer), GL_RGBA, GL_TEXTURE_2D);
        texture->bind();
        texture->generate();
    }
    return texture;
}
  