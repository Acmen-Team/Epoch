#include "eppch.h"

#include "SceneCamera.h"

#include <glm/gtc/matrix_transform.hpp>

namespace Epoch {

	SceneCamera::SceneCamera(float fov, float width, float height, float nearPlan, float farPlan)
		: m_Aspectratio(width / height), m_ProjectionMatrix(glm::perspective(glm::radians(fov), m_Aspectratio, nearPlan, farPlan)), m_ViewMatrix(1.0f)
	{
		m_ViewProjectionMatrix = m_ProjectionMatrix * m_ViewMatrix;
	}

	void SceneCamera::SetProjectionMatrix(float fov, float aspectRatio, float nearPlan, float farPlan)
	{
		m_ProjectionMatrix = glm::perspective(glm::radians(fov), aspectRatio, nearPlan, farPlan);
		m_ViewProjectionMatrix = m_ProjectionMatrix * m_ViewMatrix;

		m_Aspectratio = aspectRatio;
	}

	void SceneCamera::SetViewMatrix(const glm::mat4 viewMatrix)
	{
		m_ViewMatrix = viewMatrix;
		m_ViewProjectionMatrix = m_ProjectionMatrix * m_ViewMatrix;
	}

	void SceneCamera::RecalculateViewMatrix()
	{
		glm::mat4 transform = glm::translate(glm::mat4(1.0f), m_Position) * glm::rotate(glm::mat4(1.0f), glm::radians(m_Rotation.x), glm::vec3(1.0f, 0.0f, 0.0f)) * glm::rotate(glm::mat4(1.0f), glm::radians(m_Rotation.y), glm::vec3(0.0f, 1.0f, 0.0f)) * glm::rotate(glm::mat4(1.0f), glm::radians(m_Rotation.z), glm::vec3(0.0f, 0.0f, 1.0f));

		m_ViewMatrix = glm::inverse(transform);
		m_ViewProjectionMatrix = m_ProjectionMatrix * m_ViewMatrix;
	}

}