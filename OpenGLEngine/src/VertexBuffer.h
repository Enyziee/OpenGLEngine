#pragma once

#include <glad/glad.h>
#include <iostream>

class VertexBuffer {
	unsigned int m_RendererID;

public:
	VertexBuffer(float *data, int size);
	~VertexBuffer();

	void bind() const;
	void unbind() const;
};