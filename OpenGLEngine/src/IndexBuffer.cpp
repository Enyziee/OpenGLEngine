#pragma once 

#include <glad/glad.h>
#include "IndexBuffer.h";

IndexBuffer::IndexBuffer(float indexes[]) {
        glGenBuffers(1, &m_BufferId);
        glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, m_BufferId);

        uint32_t data_size = sizeof(indexes);
        glBufferData(GL_ELEMENT_ARRAY_BUFFER, data_size, indexes, GL_STATIC_DRAW);
        glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);
    }

void IndexBuffer::bind() {

}

void IndexBuffer::unbind() {

}