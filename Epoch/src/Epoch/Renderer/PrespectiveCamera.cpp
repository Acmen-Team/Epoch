#include "eppch.h"

#include "PrespectiveCamera.h"

#include <glm/gtc/matrix_transform.hpp>

namespace Epoch {

  PrespectiveCamera::PrespectiveCamera(float fov, float width, float height, float nearPlan, float farPlan)
	: m_ProjectionMatrix(glm::perspective(glm::radians(fov), width / height, nearPlan, farPlan)), m_ViewMatrix(1.0f)
  {
	m_ViewMatrix = glm::translate(glm::mat4(1.0f), glm::vec3(0.0f, 0.0f, -3.0f));
	m_ViewProjectionMatrix = m_ProjectionMatrix * m_ViewMatrix;
  }

  void PrespectiveCamera::RecalculateViewMatrix()
  {
	glm::mat4 transform = glm::translate(glm::mat4(1.0f), m_Position) * glm::rotate(glm::mat4(1.0f), glm::radians(m_Rotation), glm::vec3(1, 0, 0));

	m_ViewMatrix = glm::inverse(transform);
	m_ViewMatrix = glm::translate(m_ViewMatrix, glm::vec3(0.0f, 0.0f, -3.0f));
	m_ViewProjectionMatrix = m_ProjectionMatrix * m_ViewMatrix;
  }

}