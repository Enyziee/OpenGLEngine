#include "Camera.h"

Camera::Camera(float width, float heigth, float near, float far) {
	m_Position = glm::vec3(0.0f, 0.0f, 0.0f);
	m_Rotation = glm::vec3(0.0f, 0.0f, -1.0f);

	m_ProjectionMatrix = glm::perspectiveFov(glm::radians(45.0f), width, heigth, near, far);
	recalculateViewMatrix();
}

glm::vec3& Camera::getDirection() const {
	float yaw = m_Rotation.x;
	float pitch = m_Rotation.y;
	float roll = m_Rotation.z;

	glm::vec3 direction{};
	direction.x = cos(glm::radians(yaw)) * cos(glm::radians(pitch));
	direction.y = sin(glm::radians(pitch));
	direction.z = sin(glm::radians(yaw)) * cos(glm::radians(pitch));
	direction = glm::normalize(direction);

	return direction;
};

void Camera::recalculateViewMatrix() {
	
	
	m_ViewMatrix = glm::lookAt(
		m_Position,
		(m_Position + getDirection()),
		glm::vec3(0.0f, 1.0f, 0.0f)
	);

	m_ViewProjectionMatrix = m_ProjectionMatrix * m_ViewMatrix;
}

