#pragma once

#include <glad/glad.h>

#include "pch.h"
#include "Buffer.h"

class VertexArray {
private:
	unsigned int m_RendererID;
	//std::vector<VertexBuffer> m_VertexBuffers;
	VertexBuffer m_VertexBuffer;
	IndexBuffer m_IndexBuffer;

public:
	VertexArray();
	~VertexArray();
	
	void bind() const;
	void unbind() const;

	void addVertexBuffers(VertexBuffer& vertexBuffer);
	void setIndexBuffer(IndexBuffer& indexBuffer);

	const VertexBuffer& getVertexBuffers() const { return m_VertexBuffer; };
	const IndexBuffer& getIndexBuffer() const { return m_IndexBuffer; };


};
