#pragma once

#include <glad/glad.h>
#include <assert.h>
#include <string>
#include <vector>

enum class ShaderDataType {
	None = 0, Float, Float2, Float3, Float4, Mat3, Mat4, Int, Int2, Int3, Int4, Bool
};

static unsigned int ShaderDataTypeSize(const ShaderDataType& type) {
	switch (type) {
		case ShaderDataType::Float:  return 4;
		case ShaderDataType::Float2: return 4 * 2;
		case ShaderDataType::Float3: return 4 * 3;
		case ShaderDataType::Float4: return 4 * 4;

		case ShaderDataType::Mat3:   return 4 * 3 * 3;
		case ShaderDataType::Mat4:   return 4 * 4 * 4;
	
		case ShaderDataType::Int:    return 4;
		case ShaderDataType::Int2:   return 4 * 2;
		case ShaderDataType::Int3:   return 4 * 3;
		case ShaderDataType::Int4:   return 4 * 4;
		case ShaderDataType::Bool:   return 1;
	}

	assert(false);
	return 0;
}

struct BufferElement {
	std::string Name;
	ShaderDataType Type;
	unsigned int Size;
	unsigned int Offset;
	bool Normalized;
	
	BufferElement(const ShaderDataType& type, const std::string& name, bool normalized = false)
		: Name(name), Type(type), Size(ShaderDataTypeSize(type)), Offset(0), Normalized(normalized) {}

	unsigned int GetComponentCount() const {
		switch (Type) {
			case ShaderDataType::Float:  return 1;
			case ShaderDataType::Float2: return 2;
			case ShaderDataType::Float3: return 3;
			case ShaderDataType::Float4: return 4;

			case ShaderDataType::Mat3:   return 3 * 3;
			case ShaderDataType::Mat4:   return 4 * 4;

			case ShaderDataType::Int:    return 1;
			case ShaderDataType::Int2:   return 2;
			case ShaderDataType::Int3:   return 3;
			case ShaderDataType::Int4:   return 4;
			case ShaderDataType::Bool:   return 1;
		}

		assert(false);
		return 0;
	}
};


class BufferLayout {
private:
	std::vector<BufferElement> m_Elements;
	unsigned int m_Stride = 0;

public:
	BufferLayout() {};

	BufferLayout(const std::initializer_list<BufferElement>& elements) 
		: m_Elements(elements) {
		calculateOffsetAndStride();
	};

	inline uint32_t getStride() const { return m_Stride; };
	inline const std::vector<BufferElement>& getElements() const { return m_Elements; };

	std::vector<BufferElement>::iterator begin() { return m_Elements.begin(); };
	std::vector<BufferElement>::iterator end() { return m_Elements.end(); };
	
	std::vector<BufferElement>::const_iterator begin() const { return m_Elements.begin(); };
	std::vector<BufferElement>::const_iterator end() const { return m_Elements.end(); };
private:
	void calculateOffsetAndStride() {
		unsigned int offset = 0;
		m_Stride = 0;

		for (auto& element : m_Elements) {
			element.Offset = offset;
			offset += element.Size;
			m_Stride += element.Size;
		}
	}
};


class VertexBuffer {
private:
	GLsizei m_Count;
	unsigned int m_RendererID;
	BufferLayout m_Layout;

public:
	VertexBuffer() : m_Count(0), m_RendererID(0) {};
	VertexBuffer(float* vertices, unsigned int count);
	~VertexBuffer();

	void bind() const;
	void unbind();

	void setLayout(const BufferLayout& layout) { m_Layout = layout; };
	const BufferLayout& getLayout() { return m_Layout; };

	GLsizei getCount() const { return m_Count; };
};


class IndexBuffer {
private:
	int m_Count;
	unsigned int m_RendererID;

public:
	IndexBuffer() : m_Count(0), m_RendererID(0) {};
	IndexBuffer(float* indices, unsigned int count);
	~IndexBuffer();

	void bind() const;
	void unbind();

	int getCount() const { return m_Count; };
};