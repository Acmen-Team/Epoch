#pragma once
#include "Epoch/Core/Timestep.h"

#include "Epoch/Events/ApplicationEvent.h"
#include "Epoch/Events/MouseEvent.h"

#include "Epoch/Graphics/Renderer/SceneCamera.h"

namespace Epoch {

  class SceneCameraController
  {
  public:
	SceneCameraController(float aspectRatio);

	void OnUpdate(Timestep timestep);
	void OnEvent(Event& e);

	SceneCamera& GetCamera() { return m_Camera; }
	const SceneCamera& GetCamera() const { return m_Camera; }

	glm::vec3& GetCameraPosition() { return m_CameraPosition; }
	glm::vec3& GetCameraFront() { return m_CameraFront; }

	void OnViewportResize(uint32_t width, uint32_t height);
  private:
	bool OnMouseMovedEvent(MouseMovedEvent& e);
	bool OnMouseScrolledEvent(MouseScrolledEvent& e);
	bool OnMouseButtonPressedEvent(MouseButtonPressedEvent& e);
	bool OnMouseButtonReleasedEvent(MouseButtonReleasedEvent& e);
	bool OnWindowResizeEvent(WindowResizeEvent& e);

	void ProcessMouseMovement(float xOffset, float yOffset);
	void UpdateCameraVectors();
  private:
	SceneCamera m_Camera;
	glm::vec3 m_CameraPosition = { 0.0f, 0.0f, 3.0f };
	glm::vec3 m_CameraFront = { 0.0f, 0.0f, -1.0f };
	glm::vec3 m_CameraUp = { 0.0f, 1.0f, 0.0f };

	float m_AspectRatio;

	float m_CamearTransformationSpeed = 1.0;
	//glm::vec3 m_CameraTarget = { 0.0f, 0.0f, 0.0f };
	//glm::vec3 m_CameraDirection = glm::normalize(m_CameraPosition - m_CameraTarget);
	//
	//glm::vec3 m_CameraRight = glm::normalize(glm::cross(glm::vec3(0.0f, 1.0f, 0.0f), m_CameraDirection));
	//glm::vec3 m_CameraUp = glm::cross(m_CameraDirection, m_CameraRight);
	bool RightMouseButtonPressed = false;
	float MouseSensitivity = 0.1f;

	bool firstMouse = true;
	float lastX = 1920 / 2;
	float lastY = 1080 / 2;
	float m_Yaw = -90.0f;
	float m_Pitch = 0.0f;
  };

}