#include "VertexArray.h"

VertexArray::VertexArray() {
	glGenVertexArrays(1, &m_RendererID);
	glBindVertexArray(m_RendererID);
	std::cout << "Created Vertex Array ID: " << m_RendererID << std::endl;
}

VertexArray::~VertexArray() {
	glDeleteVertexArrays(1, &m_RendererID);
}

void VertexArray::addData(const VertexBuffer& vb, const VertexBufferLayout& layout) const {
	this->bind();
	vb.bind();
	const auto& elements = layout.getElements();
	unsigned int  offset = 0;

	for (unsigned int  i = 0; i < elements.size(); i++) {
		const auto& element = elements[i];
		const auto stride = layout.getStride();

		glEnableVertexAttribArray(i);
		glVertexAttribPointer(i, element.count, element.type, element.normalized, stride, (const void*)offset);

		offset += element.count * VertexBufferElement::GetSizeOfType(element.type);
	}
}

void VertexArray::bind() const {
	glBindVertexArray(m_RendererID);
}

void VertexArray::unbind() const {
	glBindVertexArray(0);
}