#include "eppch.h"

#include "SceneCamera.h"

#include <glm/gtc/matrix_transform.hpp>

namespace Epoch {

  SceneCamera::SceneCamera()
  {
	m_Aspectratio = (m_PerspectiveWidth / m_PerspectiveHeight);
	m_ProjectionMatrix = glm::perspective(glm::radians(m_PerspectiveFov), m_Aspectratio, m_PerspectiveNear, m_PerspectiveFar);
	m_ViewMatrix = glm::mat4(1.0f);
	m_ViewProjectionMatrix = m_ProjectionMatrix * m_ViewMatrix;
  }

  void SceneCamera::SetPerspective(float fov, float width, float height, float nearPlan, float farPlan)
  {
	m_ProjectionType = ProjectionType::Perspective;

	m_PerspectiveFov = fov;
	m_PerspectiveWidth = width;
	m_PerspectiveHeight = height;
	m_PerspectiveNear = nearPlan;
	m_PerspectiveFar = farPlan;

	RecalculateProjectionMatrix();
  }

  void SceneCamera::SetOrthographic(float size, float nearClip, float farClip)
  {
	m_ProjectionType = ProjectionType::Orthographic;

	m_OrthographicSize = size;
	m_OrthographicNear = nearClip;
	m_OrthographicFar = farClip;

	RecalculateProjectionMatrix();
  }

  void SceneCamera::SetProjectionMatrix(float fov, float aspectRatio, float nearPlan, float farPlan)
  {
	//m_ProjectionMatrix = glm::perspective(glm::radians(fov), aspectRatio, nearPlan, farPlan);
	//m_ViewProjectionMatrix = m_ProjectionMatrix * m_ViewMatrix;

	//m_Aspectratio = aspectRatio;
	//RecalculateProjectionMatrix();
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

  void SceneCamera::RecalculateProjectionMatrix()
  {
	float orthoLeft = -m_OrthographicSize * m_Aspectratio * 0.5f;
	float orthoRight = m_OrthographicSize * m_Aspectratio * 0.5f;
	float orthoBottom = -m_OrthographicSize * 0.5f;
	float orthoTop = m_OrthographicSize * 0.5f;

	m_ProjectionType == ProjectionType::Perspective ?
	  m_ProjectionMatrix = glm::perspective(glm::radians(m_PerspectiveFov), m_Aspectratio, m_PerspectiveNear, m_PerspectiveFar) :
	  m_ProjectionMatrix = glm::ortho(orthoLeft, orthoRight, orthoBottom, orthoTop, m_OrthographicNear, m_OrthographicFar);

	m_ViewProjectionMatrix = m_ProjectionMatrix * m_ViewMatrix;
  }

}