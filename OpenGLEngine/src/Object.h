#pragma once

#include "rapidobj/rapidobj.hpp"
#include "IndexBuffer.h"
#include "VertexBuffer.h"
#include "VertexArray.h"

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

class Object {
	glm::vec3 m_Position;
	glm::vec3 m_Rotation;
	glm::mat4 m_ModelMatrix;

	rapidobj::Result m_Result;
	std::filesystem::path m_Path;

	VertexArray m_VertexArray;
	IndexBuffer m_IndexBuffer;
	VertexBuffer m_VertexBuffer;

public:
	Object(const char* path);
	~Object();
	
	VertexArray& getVertexArray() { return m_VertexArray; };
	IndexBuffer& getIndexBuffer() { return m_IndexBuffer; }

	glm::vec3& getPosition() { return m_Position; }
	glm::vec3& getRotation() { return m_Rotation; }

	void setPosition(glm::vec3 position) {
		m_Position = position; recalculateModelMatrix();
	}
	void setRotation(glm::vec3 rotation) {
		m_Rotation = rotation; recalculateModelMatrix();
	}

	const glm::mat4& getModelMatrix() { return m_ModelMatrix; }

private:
	bool loadFile();
	void recalculateModelMatrix();
};
