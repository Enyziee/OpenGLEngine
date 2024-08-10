#pragma once

#include <glad/glad.h>
#include <iostream>

#define ASSERT(x) if (!(x)) __debugbreak();
#define GLCall(x) GLClearError();\
    x;\
    ASSERT(GLLogCall(#x, __FILE__, __LINE__))

inline void GLClearError() {
    while (glGetError() != GL_NO_ERROR);
}

inline bool GLLogCall(const char* function, const char* file, int line) {
    while (GLenum error = glGetError()) {
        std::cerr << "[OpenGL Error] (" << error << ")" << function <<
            " " << file << ":" << line << std::endl;
        return false;
    }
    return true;
}