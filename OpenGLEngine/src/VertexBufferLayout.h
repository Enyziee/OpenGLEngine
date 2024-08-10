#pragma once

#include <vector>
#include <glad/glad.h>

#include "GLLog.h"

struct VertexBufferElement {
	unsigned int type;
	unsigned int count;	
	unsigned char normalized;

	static uint32_t GetSizeOfType(uint32_t type) {
		switch (type) {
			case GL_FLOAT: return sizeof(GLfloat); 
			case GL_UNSIGNED_INT: return sizeof(GLuint); 
			case GL_UNSIGNED_BYTE: return sizeof(GLubyte);
		}
		ASSERT(false);
		return 0;
	}
};

class VertexBufferLayout {

private:
	std::vector<VertexBufferElement> m_Elements;
	unsigned int m_Stride;

public:
	VertexBufferLayout() : m_Stride(0) {}

	template<typename T>
	void Push(int count) {
		static_assert(false);
	}

	template<>
	void Push<float>(int count) {
		m_Elements.push_back({GL_FLOAT, (unsigned int)count, GL_FALSE});
		m_Stride += count * sizeof(GL_FLOAT);
	}

	template<>
	void Push<unsigned int>(int count) {
		m_Elements.push_back({ GL_UNSIGNED_INT, (unsigned int)count, GL_FALSE });
		m_Stride += count * sizeof(GLuint);
	}

	template<>
	void Push<unsigned char>(int count) {
		m_Elements.push_back({ GL_UNSIGNED_BYTE, (unsigned int)count, GL_FALSE });
		m_Stride += count * sizeof(GLubyte);
	}

	inline const std::vector<VertexBufferElement>& getElements() const { return m_Elements; }
	inline const unsigned int getStride() const { return m_Stride; }
};