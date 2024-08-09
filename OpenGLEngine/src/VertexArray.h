#pragma once

#include "glcommon.h"
#include "VertexBuffer.h"
#include "VertexBufferLayout.h"

class VertexArray {
	uint32_t m_RendererID;

public:
	VertexArray();
	~VertexArray();

	void addData(const VertexBuffer& vb, const VertexBufferLayout& layout) const;

	void bind() const;
	void unbind() const;

private:

};