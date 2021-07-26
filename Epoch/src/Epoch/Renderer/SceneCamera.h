#pragma once

#include <glm/glm.hpp>

namespace Epoch {

  class SceneCamera
  {
  public:
	enum class ProjectionType
	{
	  Perspective = 0, Orthographic = 1
	};
  public:
	SceneCamera();

	// Set ProjectionType value 
	void SetPerspective(float fov, float width, float height, float nearPlan, float farPlan);
	void SetOrthographic(float size, float nearClip, float farClip);

	// Projection Type
	ProjectionType GetProjectionType() { return m_ProjectionType; }
	void SetProjectionType(ProjectionType type) { m_ProjectionType = type; RecalculateProjectionMatrix(); }

	// Camera Setting
	float GetPerspectiveFov() const { return m_PerspectiveFov; }
	void SetPerspectiveFov(float fov) { m_PerspectiveFov = fov; RecalculateProjectionMatrix(); }

	float GetPerspectiveNear() const { return m_PerspectiveNear; }
	void SetPerspectiveNear(float nearPlan) { m_PerspectiveNear = nearPlan; RecalculateProjectionMatrix(); }

	float GetPerspectiveFar() const { return m_PerspectiveFar; }
	void SetPerspectiveFar(float farPlan) { m_PerspectiveFar = farPlan; RecalculateProjectionMatrix(); }

	float GetOrthographicSize() const { return m_OrthographicSize; }
	void SetOrthographicSize(float size) { m_OrthographicSize = size; RecalculateProjectionMatrix(); }
	
	float GetOrthographicNear() const { return m_OrthographicNear; }
	void SetOrthographicNear(float nearClip) { m_OrthographicNear = nearClip; RecalculateProjectionMatrix(); }

	float GetOrthographicFar() const { return m_OrthographicFar; }
	void SetOrthographicFar(float farClip) { m_OrthographicFar = farClip; RecalculateProjectionMatrix(); }

	// Set matrix
	void SetProjectionMatrix(float fov, float aspectRatio, float nearPlan, float farPlan);
	void SetViewMatrix(const glm::mat4 viewMatrix);

	// Set view matrix value
	const glm::vec3& GetPosition() const { return m_Position; }
	void SetPosition(const glm::vec3& position) { m_Position = position; RecalculateViewMatrix(); }
	glm::vec3 GetRotation() const { return m_Rotation; }
	void SetRotation(const glm::vec3& rotation) { m_Rotation = rotation; RecalculateViewMatrix(); }

	// Get M-V-P matrix
	const glm::mat4& GetProjectionMatrix() const { return m_ProjectionMatrix; }
	const glm::mat4& GetViewMatrix() const { return m_ViewMatrix; }
	const glm::mat4& GetViewProjectionMatrix() const { return m_ViewProjectionMatrix; }

	// Transform
	glm::mat4 GetTransform() const;
  private:
	void RecalculateViewMatrix();
	void RecalculateProjectionMatrix();
  private:
	ProjectionType m_ProjectionType = ProjectionType::Perspective;

	float m_Aspectratio;

	glm::mat4 m_ProjectionMatrix;
	glm::mat4 m_ViewMatrix;
	glm::mat4 m_ViewProjectionMatrix;

	float m_OrthographicSize = 10.0f;
	float m_OrthographicNear = 0.1f, m_OrthographicFar = 100.0f;

	float m_PerspectiveFov = 45.0f;
	float m_PerspectiveWidth = 1.6f, m_PerspectiveHeight = 0.9f, m_PerspectiveNear = 0.1f , m_PerspectiveFar = 100.0f;

	glm::vec3 m_Position = { 0.0f, 0.0f, 3.0f };
	glm::vec3 m_Rotation = { 0.0f, 0.0f, 0.0f };
  };

}