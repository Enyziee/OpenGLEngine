#include "VertexArray.h"

static GLenum ShaderDataTypeToGL(ShaderDataType type)  {
	switch (type) {
	case ShaderDataType::Float:  return GL_FLOAT;
	case ShaderDataType::Float2: return GL_FLOAT;
	case ShaderDataType::Float3: return GL_FLOAT;
	case ShaderDataType::Float4: return GL_FLOAT;

	case ShaderDataType::Mat3:   return GL_FLOAT;
	case ShaderDataType::Mat4:   return GL_FLOAT;

	case ShaderDataType::Int:    return GL_INT;
	case ShaderDataType::Int2:   return GL_INT;
	case ShaderDataType::Int3:   return GL_INT;
	case ShaderDataType::Int4:   return GL_INT;
	case ShaderDataType::Bool:   return GL_BOOL;
	}

	assert(false);
	return 0;
}

VertexArray::VertexArray() {
	glCreateVertexArrays(1, &m_RendererID);
}

VertexArray::~VertexArray() {
	glDeleteVertexArrays(1, &m_RendererID);
}

void VertexArray::bind() const {
	glBindVertexArray(m_RendererID);
}

void VertexArray::unbind() const {
	glBindVertexArray(0);
}

void VertexArray::addVertexBuffers(VertexBuffer& vertexBuffer) {
	
	HZ_ASSERT(vertexBuffer.getLayout().getElements().size(), "Vertex Buffer has no layout!");

	glBindVertexArray(m_RendererID);
	vertexBuffer.bind();
	unsigned int index = 0;
	const auto& layout = vertexBuffer.getLayout();
	for (const auto& element : layout) {
		glEnableVertexAttribArray(index);
		glVertexAttribPointer(
			index,
			element.GetComponentCount(),
			ShaderDataTypeToGL(element.Type),
			element.Normalized ? GL_TRUE : GL_FALSE,
			layout.getStride(),
			(const void*)element.Offset
		);

		index++;
	}

	m_VertexBuffer = vertexBuffer;
}

void VertexArray::setIndexBuffer(IndexBuffer& indexBuffer) {
	glBindVertexArray(m_RendererID);
	indexBuffer.bind();

	m_IndexBuffer = indexBuffer;
}
