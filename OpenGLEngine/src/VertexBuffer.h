#pragma once
#include <iostream>

class VertexBuffer {
	uint32_t m_BufferId;

public:
	VertexBuffer(float data[]);
	void bind();
	void unbind();
};