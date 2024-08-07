#pragma once

#include <glad/glad.h>
#include <iostream>
#include "VertexBuffer.h"

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

VertexBuffer::VertexBuffer(float *data, int size) {
    GLCall(glGenBuffers(1, &m_RendererID));
    GLCall(glBindBuffer(GL_ARRAY_BUFFER, m_RendererID));

    std::cout << size << " Bytes loaded to Vertex Buffer: " << m_RendererID << std::endl;

    GLCall(glBufferData(GL_ARRAY_BUFFER, size, data, GL_STATIC_DRAW));
	
    GLCall(glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)0));
    GLCall(glEnableVertexAttribArray(0));

    GLCall(glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)(3 * sizeof(float))));
    GLCall(glEnableVertexAttribArray(1));

    GLCall(glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)(6 * sizeof(float))));
    GLCall(glEnableVertexAttribArray(2));
}

VertexBuffer::~VertexBuffer() {
    glDeleteBuffers(1, &m_RendererID);
}

void VertexBuffer::bind() const {
	glBindBuffer(GL_ARRAY_BUFFER, m_RendererID);
}

void VertexBuffer::unbind() {
	glBindBuffer(GL_ARRAY_BUFFER, 0);
}