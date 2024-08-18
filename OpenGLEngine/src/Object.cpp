#include "Object.h"

Object::~Object() {
}

bool Object::loadFile() {
	m_Result = rapidobj::ParseFile(m_Path);
	bool success = rapidobj::Triangulate(m_Result);
	return success;
}

void Object::recalculateModelMatrix() {
	m_ModelMatrix = glm::translate(glm::mat4(1.0f), m_Position);
	m_ModelMatrix = glm::rotate(m_ModelMatrix, glm::radians(m_Rotation[0]), glm::vec3(1.0f, 0.0f, 0.0f));
	m_ModelMatrix = glm::rotate(m_ModelMatrix, glm::radians(m_Rotation[1]), glm::vec3(0.0f, 1.0f, 0.0f));
	m_ModelMatrix = glm::rotate(m_ModelMatrix, glm::radians(m_Rotation[2]), glm::vec3(0.0f, 0.0f, 1.0f));
}
