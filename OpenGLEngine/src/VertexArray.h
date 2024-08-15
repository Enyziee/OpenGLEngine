#pragma once

#include <glad/glad.h>
#include "VertexBuffer.h"
#include "VertexBufferLayout.h"

class VertexArray {
	unsigned int m_RendererID;

public:
	VertexArray();
	~VertexArray();

	void addData(const VertexBuffer& vb, const VertexBufferLayout& layout) const;

	void bind() const;
	void unbind() const;
};