
#ifndef GRAPHICS_GLDEBUG_HPP
#define GRAPHICS_GLDEBUG_HPP

#include <string>
#include <iostream>
#include <vector>
#include <GL/glew.h>
#include <GL/gl.h>
#include <jpl/graphics/engine/instancerendering/InstanceRendering.hpp>

namespace jpl::_graphics::_debug {

    inline void glDebugCallback(GLenum source, GLenum type, GLuint id, GLenum severity,
                               GLsizei length, const GLchar* message, const void* userParam) {
        // filtra il rumore: molti driver notificano anche eventi innocui
        if (severity == GL_DEBUG_SEVERITY_NOTIFICATION) return;

        const char* severityStr =
            severity == GL_DEBUG_SEVERITY_HIGH   ? "HIGH" :
            severity == GL_DEBUG_SEVERITY_MEDIUM ? "MEDIUM" : "LOW";

        const char* typeStr =
            type == GL_DEBUG_TYPE_ERROR               ? "ERROR" :
            type == GL_DEBUG_TYPE_DEPRECATED_BEHAVIOR  ? "DEPRECATED" :
            type == GL_DEBUG_TYPE_UNDEFINED_BEHAVIOR    ? "UNDEFINED" :
            type == GL_DEBUG_TYPE_PERFORMANCE          ? "PERFORMANCE" : "OTHER";

        std::cerr << "[GL " << severityStr << "][" << typeStr << "] (id=" << id << "): " << message << std::endl;
    }

    inline void initGLDebug() {
        glEnable(GL_DEBUG_OUTPUT);
        glEnable(GL_DEBUG_OUTPUT_SYNCHRONOUS); // importante: rende il callback sincrono con la chiamata che l'ha causato
        glDebugMessageCallback(glDebugCallback, nullptr);
        glDebugMessageControl(GL_DONT_CARE, GL_DONT_CARE, GL_DONT_CARE, 0, nullptr, GL_TRUE);
    }


    class GLStateInspector {
    public:

        struct TargetBufferPair {
            GLenum target;
            GLuint buffer;
        };

        static void dumpAll(int maxSlots, const std::vector<TargetBufferPair> & tbp, GLuint indirectBuffer) {
            std::cerr << "--- GL State Inspector ---\n";
            dumpActiveProgram();
            dumpBoundBuffers();
            dumpSSBOBindingPoints(maxSlots);
            dumpDrawCommand(indirectBuffer);
            for (size_t i = 0; i < tbp.size(); i++) {
                dumpBufferSize(tbp[i].target, tbp[i].buffer);
            }
        }

        static void dumpBoundBuffers() {
            GLint ssbo, vao, drawIndirect, ebo, vbo;
            glGetIntegerv(GL_SHADER_STORAGE_BUFFER_BINDING, &ssbo);
            glGetIntegerv(GL_VERTEX_ARRAY_BINDING, &vao);
            glGetIntegerv(GL_DRAW_INDIRECT_BUFFER_BINDING, &drawIndirect);
            glGetIntegerv(GL_ELEMENT_ARRAY_BUFFER_BINDING, &ebo);
            glGetIntegerv(GL_ARRAY_BUFFER_BINDING, &vbo);

            std::cerr << "--- GL Bound State ---\n"
                      << "VAO: " << vao << "\n"
                      << "VBO (ARRAY_BUFFER): " << vbo << "\n"
                      << "EBO (ELEMENT_ARRAY_BUFFER): " << ebo << "\n"
                      << "SSBO (generic target): " << ssbo << "\n"
                      << "DRAW_INDIRECT_BUFFER: " << drawIndirect << "\n";
        }

        static void dumpSSBOBindingPoints(int maxSlots = 4) {
            for (int i = 0; i < maxSlots; i++) {
                GLint boundBuffer = 0;
                glGetIntegeri_v(GL_SHADER_STORAGE_BUFFER_BINDING, i, &boundBuffer);
                std::cerr << "SSBO binding point " << i << ": buffer " << boundBuffer << "\n";
            }
        }

        static void dumpActiveProgram() {
            GLint program;
            glGetIntegerv(GL_CURRENT_PROGRAM, &program);
            std::cerr << "Active shader program: " << program << "\n";
        }

        static void dumpBufferSize(GLenum target, GLuint buffer) {
            glBindBuffer(target, buffer);
            GLint size;
            glGetBufferParameteriv(target, GL_BUFFER_SIZE, &size);
            std::cerr << "Buffer " << buffer << " size: " << size << " bytes\n";
        }

        static void checkErrors(const char* label) {
            GLenum err;
            bool found = false;
            while ((err = glGetError()) != GL_NO_ERROR) {
                found = true;
                std::cerr << "[GL ERROR] " << label << ": 0x" << std::hex << err << std::dec << "\n";
            }
            if (!found)
                std::cerr << "[GL OK] " << label << "\n";
        }

        static void dumpDrawCommand(GLuint buffer) {
            glBindBuffer(GL_DRAW_INDIRECT_BUFFER, buffer);
            _engine::ir::DrawElementsIndirectCommand cmd;
            glGetBufferSubData(GL_DRAW_INDIRECT_BUFFER, 0, sizeof(cmd), &cmd);
            std::cerr << "count=" << cmd.count
                      << " instanceCount=" << cmd.instanceCount
                      << " firstIndex=" << cmd.firstIndex
                      << " baseVertex=" << cmd.baseVertex
                      << " baseInstance=" << cmd.baseInstance << "\n";
        }
    };
}

#endif