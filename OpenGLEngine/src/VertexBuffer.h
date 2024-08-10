#pragma once

#include "GLLog.h"

class VertexBuffer {
	uint32_t m_RendererID;

public:
	VertexBuffer(float *data, int size);
	~VertexBuffer();

	void bind() const;
	void unbind();
};