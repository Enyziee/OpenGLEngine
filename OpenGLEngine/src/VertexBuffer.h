#pragma once

#include <glad/glad.h>
#include <iostream>

class VertexBuffer {
	unsigned int m_RendererID;
	int m_Count;
	int m_Size;

public:
	VertexBuffer();
	VertexBuffer(float *data, int size);
	~VertexBuffer();

	void loadData(float* data, int size);

	void bind() const;
	void unbind() const;
};