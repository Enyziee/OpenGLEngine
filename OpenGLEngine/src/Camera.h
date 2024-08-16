#pragma once

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

class Camera {
private:
	glm::vec3 m_Position;
	glm::vec3 m_Rotation;

	glm::mat4 m_ViewMatrix;
	glm::mat4 m_ProjectionMatrix;
	glm::mat4 m_ViewProjectionMatrix;

public:
	Camera(float width, float heigth, float near, float far);

	void setPosition(const glm::vec3& position) { m_Position = position; recalculateViewMatrix(); }
	void setRotation(const glm::vec3& rotation) { m_Rotation = rotation; recalculateViewMatrix(); }

	const glm::vec3& getPosition() { return m_Position; }
	const glm::vec3& getRotation() { return m_Rotation; }
	
	glm::vec3& getDirection() const;

	glm::mat4& getViewMatrix() { return m_ViewMatrix; }
	glm::mat4& getProjectionMatrix () { return m_ProjectionMatrix; }
	glm::mat4& getViewProjectionMatrix() { return m_ViewProjectionMatrix; }
private:
	void recalculateViewMatrix();
};