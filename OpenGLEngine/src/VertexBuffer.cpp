#include "VertexBuffer.h"

VertexBuffer::VertexBuffer() {
    m_Count = 0;

    glGenBuffers(1, &m_RendererID);
    glBindBuffer(GL_ARRAY_BUFFER, m_RendererID);
    std::cout << "Created Vertex Buffer ID: " << m_RendererID << std::endl;
}

VertexBuffer::VertexBuffer(float *data, int count) {
    glGenBuffers(1, &m_RendererID);
    glBindBuffer(GL_ARRAY_BUFFER, m_RendererID);
    std::cout << "Created Vertex Buffer ID: " << m_RendererID << std::endl;

    loadData(data, count);
}

VertexBuffer::~VertexBuffer() {
    glDeleteBuffers(1, &m_RendererID);
}

void VertexBuffer::loadData(float* data, int count) {
    m_Count = count;
    
    glBindBuffer(GL_ARRAY_BUFFER, m_RendererID);
    glBufferData(GL_ARRAY_BUFFER, count * sizeof(float), data, GL_STATIC_DRAW);

    std::cout << count << " Elements loaded to Vertex Buffer: " << m_RendererID << std::endl;
}

void VertexBuffer::bind() const {
    glBindBuffer(GL_ARRAY_BUFFER, m_RendererID);
}

void VertexBuffer::unbind() const {
    glBindBuffer(GL_ARRAY_BUFFER, 0);
}