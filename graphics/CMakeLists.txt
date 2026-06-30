cmake_minimum_required(VERSION 3.15) # Versione minima consigliata per C++23
project(GRAPHICS VERSION 1.0.0)

# --- 1. CONFIGURAZIONI COMPILATORE ---
set(CMAKE_CXX_STANDARD 23)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

# --- 2. RICERCA DIPENDENZE (Portabile) ---
find_package(GLEW REQUIRED)
find_package(OpenGL REQUIRED)
find_package(glfw3 REQUIRED) # Nota: spesso il pacchetto si chiama glfw3 o GLFW

# --- 3. DEFINIZIONE SORGENTI ---
set(SOURCES
    "src/Metrics.cpp"
    "src/utils/Window.cpp"
    "src/shaders/Shader.cpp"
    "src/shaders/ProgramShaders.cpp"
    "src/shaders/ProgramManager.cpp"
    "src/mesh/Mesh.cpp"
    "src/mesh/MeshLoader.cpp"
    "src/input/Keyboard.cpp"
    "src/input/Mouse.cpp"
    "src/engine/Painter.cpp"
    "src/engine/button/Button.cpp"
    "src/engine/camera/BaseCamera.cpp"
    "src/engine/camera/OrthoCamera.cpp"
    "src/engine/camera/PerspectiveCamera.cpp"
    "src/engine/text/Font.cpp"
    "src/engine/text/TextRender.cpp"
    "src/engine/progress/ProgressBar.cpp"
    "src/engine/VAO.cpp"
    "src/engine/VAOManager.cpp"
    "src/engine/scene/Scene.cpp"
    "src/engine/scene/SceneManager.cpp"
)

# --- 4. CREAZIONE TARGET ---

# Libreria STATICA (.a)
add_library(graphicsJPL_st STATIC ${SOURCES})
set_target_properties(graphicsJPL_st PROPERTIES OUTPUT_NAME "graphicsJPL")

# Libreria SHARED (.so / .dll)
add_library(graphicsJPL_sh SHARED ${SOURCES})
set_target_properties(graphicsJPL_sh PROPERTIES OUTPUT_NAME "graphicsJPL" SOVERSION ${PROJECT_VERSION})

# --- 5. GESTIONE DIPENDENZE (Risolve gli Undefined Reference) ---

# Definiamo le librerie comuni a entrambi i target
set(COMMON_LIBS 
    OpenGL::GL 
    GLEW::GLEW 
    glfw 
    parserJPL
    loggerJPL
)

target_link_libraries(graphicsJPL_st PUBLIC ${COMMON_LIBS})

target_link_libraries(graphicsJPL_sh PRIVATE ${COMMON_LIBS})
if(WIN32)
    target_link_libraries(graphicsJPL_sh PRIVATE dbghelp PowrProf pdh)
endif()

target_compile_definitions(graphicsJPL_st PUBLIC STB_IMAGE_IMPLEMENTATION)
target_compile_definitions(graphicsJPL_sh PRIVATE STB_IMAGE_IMPLEMENTATION)

target_include_directories(graphicsJPL_st PUBLIC ${CMAKE_CURRENT_SOURCE_DIR}/src)
target_include_directories(graphicsJPL_sh PRIVATE ${CMAKE_CURRENT_SOURCE_DIR}/src)