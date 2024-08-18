#include "Object.h"

bool Object::loadFile() {
	m_Result = rapidobj::ParseFile(m_Path);
	bool success = rapidobj::Triangulate(m_Result);
	return success;
}

void Object::recalculateModelMatrix() {
	m_ModelMatrix = glm::translate(glm::mat4(1.0f), m_Position);
	// m_ModelMatrix = glm::rotate(m_ModelMatrix, m_Position);
}
