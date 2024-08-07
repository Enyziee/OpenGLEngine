#pragma once 

#include <glad/glad.h>
#include <iostream>

#include "IndexBuffer.h";

#define ASSERT(x) if (!(x)) __debugbreak();
#define GLCall(x) GLClearError();\
    x;\
    ASSERT(GLLogCall(#x, __FILE__, __LINE__))

static void GLClearError() {
    while (glGetError() != GL_NO_ERROR);
}

static bool GLLogCall(const char* function, const char* file, int line) {
    while (GLenum error = glGetError()) {
        std::cout << "[OpenGL Error] (" << error << ")" << function <<
            " " << file << ":" << line << std::endl;
        return false;
    }
    return true;
}

IndexBuffer::IndexBuffer(uint32_t* indexes, int count) {
        GLCall(glGenBuffers(1, &m_RendererID));
        GLCall(glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, m_RendererID));
        GLCall(glBufferData(GL_ELEMENT_ARRAY_BUFFER, count, indexes, GL_STATIC_DRAW));

        m_Count = count;

        std::cout << count << " Elements loaded to Index Buffer: " << m_RendererID << std::endl;
}

IndexBuffer::~IndexBuffer() {
    glDeleteBuffers(1, &m_RendererID);
}

void IndexBuffer::bind() {
    GLCall(glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, m_RendererID));
}

void IndexBuffer::unbind() {
    GLCall(glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0));
}