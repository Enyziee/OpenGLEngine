#pragma once

#include "rapidobj/rapidobj.hpp"

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

#include "Buffer.h"
#include "VertexArray.h"

class Object {
	glm::vec3 m_Position;
	glm::vec3 m_Rotation;
	
	VertexArray m_VertexArray;

	glm::mat4 m_ModelMatrix;

	rapidobj::Result m_Result;
	std::filesystem::path m_Path;

	
public:
	Object() : m_Position(glm::vec3(0.0f)), m_Rotation(glm::vec3(0.0f)) { recalculateModelMatrix(); };
	Object(const VertexArray& va) : m_Position(glm::vec3(0.0f)), m_Rotation(glm::vec3(0.0f)), m_VertexArray(va) { recalculateModelMatrix(); };
	~Object();


	VertexArray& getVertexArray() { return m_VertexArray; }

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
