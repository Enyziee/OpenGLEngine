#pragma once 

#include "IndexBuffer.h";

IndexBuffer::IndexBuffer(uint32_t* indexes, int count) {
        glGenBuffers(1, &m_RendererID);
        glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, m_RendererID);
        glBufferData(GL_ELEMENT_ARRAY_BUFFER, count * sizeof(unsigned int), indexes, GL_STATIC_DRAW);

        m_Count = count;

        std::cout << count << " Indexes loaded, to Index Buffer: " << m_RendererID << std::endl;
}

IndexBuffer::~IndexBuffer() {
    glDeleteBuffers(1, &m_RendererID);
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