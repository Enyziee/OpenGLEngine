#pragma once 

#include "IndexBuffer.h";

IndexBuffer::IndexBuffer() {
    m_Count = 0;
    
    glGenBuffers(1, &m_RendererID);

    std::cout << "Created Index Buffer ID: " << m_RendererID << std::endl;
}

IndexBuffer::IndexBuffer(int* indexes, size_t count) {
    glGenBuffers(1, &m_RendererID);
    
    loadData(indexes, count);
}

IndexBuffer::~IndexBuffer() {
    glDeleteBuffers(1, &m_RendererID);
}

void IndexBuffer::loadData(int* indexes, size_t count) {
    m_Count = count;
    
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, m_RendererID);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, count * sizeof(unsigned int), indexes, GL_STATIC_DRAW);
    std::cout << count << " Indexes loaded, to Index Buffer: " << m_RendererID << std::endl;
}

void IndexBuffer::bind() const {
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, m_RendererID);
}

void IndexBuffer::unbind() const {
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);
}

int IndexBuffer::getCount() const {
    return m_Count;
}