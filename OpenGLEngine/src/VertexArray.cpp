#include "VertexArray.h"

VertexArray::VertexArray() {
	GLCall(glGenVertexArrays(1, &m_RendererID));
}

VertexArray::~VertexArray() {
	GLCall(glDeleteVertexArrays(1, &m_RendererID));
}

void VertexArray::addData(const VertexBuffer& vb, const VertexBufferLayout& layout) const {
	this->bind();
	vb.bind();
	const auto& elements = layout.getElements();
	uint32_t offset = 0;

	for (uint32_t i = 0; i < elements.size(); i++) {
		const auto& element = elements[i];
		const auto stride = layout.getStride();

		GLCall(glEnableVertexAttribArray(i));
		GLCall(glVertexAttribPointer(i, element.count, element.type, element.normalized, stride, (const void*)offset));

		offset += element.count * VertexBufferElement::GetSizeOfType(element.type);
	}
}

void VertexArray::bind() const {
	GLCall(glBindVertexArray(m_RendererID));
}

void VertexArray::unbind() const {
	GLCall(glBindVertexArray(0));
}