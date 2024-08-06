#pragma once

#include <glad/glad.h>

#include "VertexBuffer.h"

VertexBuffer::VertexBuffer(float data[]) {
	glGenBuffers(1, &m_BufferId);
	glBindBuffer(GL_ARRAY_BUFFER, m_BufferId);

	uint32_t data_s = sizeof(data);
	glBufferData(GL_ARRAY_BUFFER, data_s, data, GL_STATIC_DRAW);
	
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)0);
    glEnableVertexAttribArray(0);

    glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)(3 * sizeof(float)));
    glEnableVertexAttribArray(1);

    glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)(6 * sizeof(float)));
    glEnableVertexAttribArray(2);
}

void VertexBuffer::bind() {
	glBindBuffer(GL_ARRAY_BUFFER, m_BufferId);
}

void VertexBuffer::unbind() {
	glBindBuffer(GL_ARRAY_BUFFER, 0);
}