#include "VertexBuffer.h"

VertexBuffer::VertexBuffer(float *data, int count) {
    GLCall(glGenBuffers(1, &m_RendererID));
    GLCall(glBindBuffer(GL_ARRAY_BUFFER, m_RendererID));
    GLCall(glBufferData(GL_ARRAY_BUFFER, count * sizeof(float), data, GL_STATIC_DRAW));

    std::cout << count << " Elements loaded to Vertex Buffer: " << m_RendererID << std::endl;
}

VertexBuffer::~VertexBuffer() {
    GLCall(glDeleteBuffers(1, &m_RendererID));
}

void VertexBuffer::bind() const {
    GLCall(glBindBuffer(GL_ARRAY_BUFFER, m_RendererID));
}

void VertexBuffer::unbind() {
    GLCall(glBindBuffer(GL_ARRAY_BUFFER, 0));
}